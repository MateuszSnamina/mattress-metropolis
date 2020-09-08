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
// STD:
#include<iostream>
#include<chrono>
#include<exception>
#include<optional>
#include<memory>

//const double ising_tc = 1 / 2 / std::log(1 + std::sqrt(2));

void print(const double temperatue,
           const metropolis_engine::MetropolisEngineStatisticalAccumulator& results,
           const double simulation_time_sec) {
    const double beta = 1 / temperatue;
    const double average_energy = results.get_average_energy();
    const double average_sq_energy = results.get_average_sq_energy();
    const double average_sigma_energy = average_sq_energy - average_energy * average_energy;
    const double specific_heat = average_sigma_energy * beta * beta;
    //const double average_magnetization = results.get_average_magnetization();
    const double average_abs_magnetization = results.get_average_abs_magnetization();
    std::cout << "[TREE]│├ simulation time: " << simulation_time_sec << "[s]" << std::endl;
    // std::cout << "[TREE]│├ steps:        " << results.get_steps() << std::endl;
    std::cout << "[TREE]│├ E(" << temperatue << "):   " << average_energy / (GLOBAL_N * GLOBAL_M) << std::endl;
    //std::cout << "[TREE]│├ E^2(" << temperatue << "): " << average_sq_energy / (N * M) << std::endl;
    std::cout << "[TREE]│├ σE(" << temperatue << "):  " << average_sigma_energy / (GLOBAL_N * GLOBAL_M) << std::endl;
    std::cout << "[TREE]│├ C(" << temperatue << "):   " << specific_heat / (GLOBAL_N * GLOBAL_M) << std::endl;
    //std::cout << "[TREE]│├ M(" << temperatue << "):   " << average_magnetization / (N * M) << std::endl;
    std::cout << "[TREE]│├ |M|(" << temperatue << "): " << average_abs_magnetization / (GLOBAL_N * GLOBAL_M) << std::endl;
}

//TODO: remove
//template<unsigned N, unsigned M, unsigned NN, unsigned MM>
//struct EnergyGetters {
//    EnergyGetters(
//            std::unique_ptr<energy::getter::BoardEnergyGetter<N, M>>&& board_energy_getter,
//            std::unique_ptr<energy::getter::NeighbourhoodEnergyGetter<NN, MM>>&& neighbourhood_energy_getter) :
//        _board_energy_getter(std::move(board_energy_getter)),
//        _neighbourhood_energy_getter(std::move(neighbourhood_energy_getter)) {
//    }
//    std::unique_ptr<energy::getter::BoardEnergyGetter<N, M>> _board_energy_getter;
//    std::unique_ptr<energy::getter::NeighbourhoodEnergyGetter<NN, MM>> _neighbourhood_energy_getter;
//};


std::optional<std::unique_ptr<energy::getter::EnergyGetter<GLOBAL_M, GLOBAL_N, 3, 3>>>
get_nn_mm_3_energy_getters(const InterpretedProgramOptions& interpreted_program_options) {
    //    const ModelType& model_type = interpreted_program_options.model_type;
    //    const EnergyGetterType& energy_getter_type = interpreted_program_options.energy_getter_type;
    //    if (energy_getter_type == EnergyGetterType::ZeroNn) {
    //        parts_energy_getter =
    //    }
    return std::nullopt;
}

std::optional<std::unique_ptr<energy::getter::EnergyGetter<GLOBAL_N, GLOBAL_M, 7, 7>>>
get_nn_mm_7_energy_getters(const InterpretedProgramOptions& interpreted_program_options) {
    const ModelType& model_type = interpreted_program_options.model_type;
    const EnergyGetterType& energy_getter_type = interpreted_program_options.energy_getter_type;
    if (energy_getter_type == EnergyGetterType::FourNn) {
        std::unique_ptr<energy::getter::FourNnPartsEnergyGetter> parts_energy_getter;
        if (model_type == ModelType::IsingDoublet) {
            parts_energy_getter = std::make_unique<energy::ising::doublet::FourNnPartsEnergyGetter>();
        } if (model_type == ModelType::IsingMultiplet) {
            parts_energy_getter = std::make_unique<energy::ising::multiplet::FourNnPartsEnergyGetter>(2);
        } if (model_type == ModelType::Parametrized) {
            assert(false); //TODO implement
        }
        return  std::make_unique<energy::getter::FourNnEnergyGetter<GLOBAL_N, GLOBAL_M>>(std::move(parts_energy_getter));
    }
    if (energy_getter_type == EnergyGetterType::EightNn) {
        assert(false); //TODO implement
    }
    return std::nullopt;
}


template<unsigned N, unsigned M, unsigned NN, unsigned MM>
metropolis_engine::MetropolisEngineStatisticalAccumulator
do_calculations(
        const double beta,
        const energy::getter::EnergyGetter<N, M, 7, 7>& energy_getter,
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
    std::cout << "[TREE]temperatures loop" << std::endl;
    for (const auto& temperatue_index : temerature_steps | boost::adaptors::indexed(0)) {
        const double temperatue = temperatue_index.value();
        const double index = temperatue_index.index();
        const double beta = 1.0 / temperatue;
        std::cout << "[TREE]├temperature, beta, {#/#}: " << temperatue << ", " << beta << ", {" << index << "/" << temerature_steps.size() << "}" <<std::endl;
        const std::chrono::steady_clock::time_point calculation_time_begin = std::chrono::steady_clock::now();
        const auto results = do_calculations<N, M, NN, MM>(beta, energy_getter,
                                                           n_thermal_steps, n_average_steps);
        const std::chrono::steady_clock::time_point calculation_time_end = std::chrono::steady_clock::now();
        const auto simulation_time_sec = std::chrono::duration_cast<std::chrono::microseconds>(calculation_time_end - calculation_time_begin).count() / 1e6;
        print(temperatue, results, simulation_time_sec);
        all_results.push_back({temperatue, results});
    }
    return all_results;
}

void print_input_data(const InterpretedProgramOptions& interpreted_program_options) {
    const extension::std::StreamFromatStacker stream_format_stacker(std::cout);
    std::cout << "[INFO   ] [BOARD SIEZE] N x M                          = " << GLOBAL_N << " x " << GLOBAL_M << std::endl;
    std::cout << "[INFO   ] [PROGRAM_OPTIONS] temerature_steps           = "
              << extension::boost::RangeStringStreamer()
                 .set_stream_sustainer([](::std::ostream&, size_t){})
                 .set_stream_separer([](::std::ostream& s){ s << ", ";})
                 .stream(interpreted_program_options.temerature_steps).str()
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
        //SET MODEL AND ENERGY GETTERS: TODO
        //        const energy::getter::FourNnPartsEnergyGetter& parts_energy_getter =
        //                energy::ising::doublet::FourNnPartsEnergyGetter{};
        //        //const energy::ising::multiplet::FourNnPartsEnergyGetter parts_energy_getter{2};
        //        const energy::getter::BoardEnergyGetter<GLOBAL_N, GLOBAL_M>& board_energy_getter =
        //                energy::getter::FourNnBoardEnergyGetter<GLOBAL_N, GLOBAL_M>{parts_energy_getter};
        //        const energy::getter::NeighbourhoodEnergyGetter<7, 7>& neighbourhood_energy_getter =
        //                energy::getter::FourNnNeighbourhoodEnergyGetter{parts_energy_getter};
        // ******************************************************************
        //SET MODEL AND ENERGY GETTERS: TODO
        std::unique_ptr<energy::getter::FourNnPartsEnergyGetter> parts_energy_getter =
                std::make_unique<energy::ising::doublet::FourNnPartsEnergyGetter>();
        //const energy::ising::multiplet::FourNnPartsEnergyGetter parts_energy_getter{2};
        std::unique_ptr<energy::getter::EnergyGetter<GLOBAL_N, GLOBAL_M, 7, 7>> energy_getter =
                std::make_unique<energy::getter::FourNnEnergyGetter<GLOBAL_N, GLOBAL_M>>(std::move(parts_energy_getter));
        // ******************************************************************
        const auto all_results = do_main_temperature_loop<GLOBAL_N, GLOBAL_M, 7, 7>(
                    interpreted_program_options.temerature_steps,
                    *energy_getter,
                    interpreted_program_options.n_thermal_steps,
                    interpreted_program_options.n_average_steps);
        // ******************************************************************
        std::cout << "[INFO   ] Normal termination." << std::endl;
        return 0;
    } catch (std::exception& e) {
        std::cerr << "[ERROR  ] Abnormal termination!" << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
