// METRPOPOLIS:
#include <metropolis/nm.hpp>
#include <metropolis/raw_program_options.hpp>
#include <metropolis/interpreted_program_options.hpp>
// METRPOPOLIS-ENGINE:
#include <metropolis_engine/metropolis_engine.hpp>
// ENERGY:
#include <energy/energy_getter_ising_doublet.hpp>
#include <energy/energy_getter_ising_multiplet.hpp> //TODO remove
// NUMBOARD:
#include <numboard/numboard.hpp>
// EXTENSIONS:
#include <extensions/range_streamer.hpp>
#include <extensions/stream_fromat_stacker.hpp>
// BOOST:
#include <boost/range/adaptor/indexed.hpp>
#include <boost/range/adaptor/transformed.hpp>
// STD:
#include<iostream>
#include<chrono>
#include<exception>
#include<optional>
#include<memory>

//const double ising_tc = 1 / 2 / std::log(1 + std::sqrt(2));

void print(const std::vector<std::pair<double, metropolis_engine::MetropolisEngineStatisticalAccumulator>>& results) {
    using namespace extension::boost::stream_pragma;
    using namespace boost::adaptors;
    constexpr auto GLOBAL_NM = GLOBAL_N * GLOBAL_M;
    const extension::std::StreamFromatStacker stream_format_stacker(std::cout);
    const auto range_stream_settings = RSS()
            .set_null_sustainer()
            .set_string_separer(", ")
            .in_bracket_square();
    std::cout << "[RESULTS] " << "T: " <<
                 (results |
                  transformed([](const auto& _){return _.first;}) |
                  range_stream_settings )
              << std::endl;
    std::cout << "[RESULTS] " << "E: " <<
                 (results |
                  transformed([](const auto& _){return _.second.get_average_energy() / GLOBAL_NM;}) |
                  range_stream_settings )
              << std::endl;
    std::cout << "[RESULTS] " << "σE: " <<
                 (results |
                  transformed([](const auto& _){return _.second.get_average_sigma_energy() / GLOBAL_NM;}) |
                  range_stream_settings )
              << std::endl;
    std::cout << "[RESULTS] " << "C: " <<
                 (results |
                  transformed([](const auto& _){return _.second.get_specific_heat(1/_.first) / GLOBAL_NM;}) |
                  range_stream_settings )
              << std::endl;
    std::cout << "[RESULTS] " << "|M|: " <<
                 (results |
                  transformed([](const auto& _){return _.second.get_average_abs_magnetization() / GLOBAL_NM;}) |
                  range_stream_settings ) << std::endl;
}

void print(const double temperatue,
           const metropolis_engine::MetropolisEngineStatisticalAccumulator& results,
           const double simulation_time_sec) {
    constexpr auto GLOBAL_NM = GLOBAL_N * GLOBAL_M;
    const extension::std::StreamFromatStacker stream_format_stacker(std::cout);
    const double beta = 1 / temperatue;
    const double average_energy = results.get_average_energy();
    //const double average_sq_energy = results.get_average_sq_energy();
    const double average_sigma_energy = results.get_average_sigma_energy();
    const double specific_heat = results.get_specific_heat(beta);
    //const double average_magnetization = results.get_average_magnetization();
    const double average_abs_magnetization = results.get_average_abs_magnetization();
    std::cout << "[FLOW   ] " << "│├ "
              << std::setw(31) << std::left << "T, β:" << temperatue << ", " << beta << std::endl;
    // std::cout << "[FLOW   ] " << "│├ "
    //           << std::setw(30) << std::left << "steps:" << results.get_steps() << std::endl;
    std::cout << "[FLOW   ] " << "│├ "
              << std::setw(30) << std::left << "E(" + std::to_string(temperatue) + "):" << average_energy / GLOBAL_NM << std::endl;
    //std::cout << "[FLOW   ] " << "│├ "
    //          << std::setw(30) << std::left << "E^2(" + std::to_string(temperatue) + "):" << average_sq_energy / GLOBAL_NM << std::endl;
    std::cout << "[FLOW   ] " << "│├ "
              << std::setw(31) << std::left << "σE(" + std::to_string(temperatue) + "):" << average_sigma_energy / GLOBAL_NM << std::endl;
    std::cout << "[FLOW   ] " << "│├ "
              << std::setw(30) << std::left << "C(" + std::to_string(temperatue) + "):" << specific_heat / GLOBAL_NM << std::endl;
    //std::cout << "[FLOW   ] " << "│├ "
    //          << std::setw(30) << std::left << "M(" + std::to_string(temperatue) + "):" << average_magnetization / GLOBAL_NM << std::endl;
    std::cout << "[FLOW   ] " << "│├ "
              << std::setw(30) << std::left << "|M|(" + std::to_string(temperatue) + "):" << average_abs_magnetization / GLOBAL_NM << std::endl;
    std::cout << "[FLOW   ] " << "│├ "
              << std::setw(30) << std::left << "simulation time:" << simulation_time_sec << "[s]" << std::endl;
}

std::optional<std::unique_ptr<energy::getter::EnergyGetter<GLOBAL_N, GLOBAL_M, 3, 3>>>
get_nn_mm_3_energy_getters(const InterpretedProgramOptions& interpreted_program_options) {
    const ModelType& model_type = interpreted_program_options.model_type;
    const EnergyGetterType& energy_getter_type = interpreted_program_options.energy_getter_type;
    if (energy_getter_type == EnergyGetterType::ZeroNn) {
        std::unique_ptr<energy::getter::ZeroNnPartsEnergyGetter> parts_energy_getter;
        if (model_type == ModelType::IsingDoublet) {
            parts_energy_getter = energy::ising::doublet::ZeroNnPartsEnergyGetter::make();
        } if (model_type == ModelType::IsingMultiplet) {
            parts_energy_getter = energy::ising::multiplet::ZeroNnPartsEnergyGetter::make(interpreted_program_options.ising_multiplicity);
        } if (model_type == ModelType::Parametrized) {
            throw std::logic_error("NOT IMPLEMENTED ERROR -> ModelType::Parametrized");
            assert(false); //TODO implement
        }
        return energy::getter::ZeroNnEnergyGetter<GLOBAL_N, GLOBAL_M>::make(std::move(parts_energy_getter));
    }
    return std::nullopt;
}

std::optional<std::unique_ptr<energy::getter::EnergyGetter<GLOBAL_N, GLOBAL_M, 7, 7>>>
get_nn_mm_7_energy_getters(const InterpretedProgramOptions& interpreted_program_options) {
    const ModelType& model_type = interpreted_program_options.model_type;
    const EnergyGetterType& energy_getter_type = interpreted_program_options.energy_getter_type;
    if (energy_getter_type == EnergyGetterType::FourNn) {
        std::unique_ptr<energy::getter::FourNnPartsEnergyGetter> parts_energy_getter;
        if (model_type == ModelType::IsingDoublet) {
            parts_energy_getter = energy::ising::doublet::FourNnPartsEnergyGetter::make();
        } if (model_type == ModelType::IsingMultiplet) {
            parts_energy_getter = energy::ising::multiplet::FourNnPartsEnergyGetter::make(interpreted_program_options.ising_multiplicity);
        } if (model_type == ModelType::Parametrized) {
            throw std::logic_error("NOT IMPLEMENTED ERROR (ModelType::Parametrized)");
            assert(false); //TODO implement
        }
        return energy::getter::FourNnEnergyGetter<GLOBAL_N, GLOBAL_M>::make(std::move(parts_energy_getter));
    }
    if (energy_getter_type == EnergyGetterType::EightNn) {
        throw std::logic_error("NOT IMPLEMENTED ERROR (EnergyGetterType::EightNn)");
        assert(false); //TODO implement
    }
    return std::nullopt;
}


template<unsigned N, unsigned M, unsigned NN, unsigned MM>
metropolis_engine::MetropolisEngineStatisticalAccumulator
do_calculations(
        const double beta,
        const energy::getter::EnergyGetter<N, M, NN, MM>& energy_getter,
        const unsigned n_thermal_steps,
        const unsigned n_average_steps) {
    metropolis_engine::MetropolisEngine<N, M, NN, MM> engine(beta, energy_getter);
    engine.init_uniform();
    for (unsigned i = 0; i < n_thermal_steps; i++) {
        engine.do_step();
    }
    for (unsigned i = 0; i < n_average_steps; i++) {
        engine.do_step();
        engine.accumulate();
    }
    return engine.get_accumulator();
}

template<unsigned N, unsigned M, unsigned NN, unsigned MM>
std::vector<std::pair<double, metropolis_engine::MetropolisEngineStatisticalAccumulator>>
do_main_temperature_loop(
        const std::vector<double>& temerature_steps,
        const energy::getter::EnergyGetter<N, M, NN, MM>& energy_getter,
        const unsigned n_thermal_steps,
        const unsigned n_average_steps) {
    using IterationResult = std::pair<double, metropolis_engine::MetropolisEngineStatisticalAccumulator>;
    std::vector<IterationResult> all_results;
    std::cout << "[FLOW   ] temperatures loop" << std::endl;
    for (const auto& temperatue_index : temerature_steps | boost::adaptors::indexed(0)) {
        const double temperatue = temperatue_index.value();
        const double index = temperatue_index.index();
        const double beta = 1.0 / temperatue;
        std::cout << "[FLOW   ] " << "├ {#/#}:" << " {" << index << "/" << temerature_steps.size() << "}" <<std::endl;
        const std::chrono::steady_clock::time_point calculation_time_begin = std::chrono::steady_clock::now();
        const auto results = do_calculations<N, M, NN, MM>(beta, energy_getter,
                                                           n_thermal_steps, n_average_steps);
        const std::chrono::steady_clock::time_point calculation_time_end = std::chrono::steady_clock::now();
        const auto simulation_time_sec = std::chrono::duration_cast<std::chrono::microseconds>(calculation_time_end - calculation_time_begin).count() / 1e6;
        print(temperatue, results, simulation_time_sec);
        all_results.push_back({temperatue, results});
    }
    print(all_results);
    return all_results;
}

void print_input_data(const InterpretedProgramOptions& interpreted_program_options) {
    using namespace extension::boost::stream_pragma;
    const extension::std::StreamFromatStacker stream_format_stacker(std::cout);
    std::cout << "[INFO   ] [BOARD SIEZE] N x M                          = " << GLOBAL_N << " x " << GLOBAL_M << std::endl;
    std::cout << "[INFO   ] [PROGRAM_OPTIONS] temerature_steps           = "
              << (interpreted_program_options.temerature_steps | RSS().set_null_sustainer().set_string_separer(", "))
              << std::endl;
    std::cout << "[INFO   ] [PROGRAM_OPTIONS] n_thermal_steps            = "
              << std::left << std::setw(9) << interpreted_program_options.n_thermal_steps
              << " = " << std::setw(9) << (interpreted_program_options.n_thermal_steps / 1e3) << "[k]"
              << " = " << std::setw(9) << (interpreted_program_options.n_thermal_steps / 1e6) << "[M]"
              << std::right << std::endl ;
    std::cout << "[INFO   ] [PROGRAM_OPTIONS] n_average_steps            = "
              << std::left << std::setw(9) << interpreted_program_options.n_average_steps
              << " = " << std::setw(9) << (interpreted_program_options.n_average_steps / 1e3) << "[k]"
              << " = " << std::setw(9) << (interpreted_program_options.n_average_steps / 1e6) << "[M]"
              << std::right << std::endl;
    std::cout << "[INFO   ] [PROGRAM_OPTIONS] model_type                 = " << interpreted_program_options.model_type<< std::endl;
    std::cout << "[INFO   ] [PROGRAM_OPTIONS] energy_getter_type         = " << interpreted_program_options.energy_getter_type<< std::endl;
}

int main(int argc, char** argv) {
    try {
        // ******************************************************************
        const RawProgramOptions raw_program_options = grep_program_options(argc, argv);
        const InterpretedProgramOptions interpreted_program_options = interpret_program_options(raw_program_options);
        // ******************************************************************
        print_input_data(interpreted_program_options);
        // ******************************************************************
        if (auto energy_getter = get_nn_mm_3_energy_getters(interpreted_program_options)) {
            const auto all_results = do_main_temperature_loop<GLOBAL_N, GLOBAL_M, 3, 3>(
                        interpreted_program_options.temerature_steps,
                        **energy_getter,
                        interpreted_program_options.n_thermal_steps,
                        interpreted_program_options.n_average_steps);
        } else if (auto energy_getter = get_nn_mm_7_energy_getters(interpreted_program_options)) {
            const auto all_results = do_main_temperature_loop<GLOBAL_N, GLOBAL_M, 7, 7>(
                        interpreted_program_options.temerature_steps,
                        **energy_getter,
                        interpreted_program_options.n_thermal_steps,
                        interpreted_program_options.n_average_steps);
        }
        // ******************************************************************
        std::cout << "[INFO   ] Normal termination." << std::endl;
        return 0;
    } catch (std::exception& e) {
        std::cerr << "[ERROR  ] Abnormal termination!" << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
