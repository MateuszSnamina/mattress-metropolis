// METRPOPOLIS:
#include <metropolis/raw_program_options.hpp>
#include <metropolis/interpreted_program_options.hpp>
// METRPOPOLIS-ENGINE:
#include <metropolis_engine/metropolis_engine.hpp>
// ENERGY:
#include <energy/energy_getter_ising_doublet.hpp>
#include <energy/energy_getter_ising_multiplet.hpp> //TODO remove
// NUMBOARD:
#include <numboard/numboard.hpp>
// STD:
#include<iostream>
#include<chrono>

template<unsigned N, unsigned M, unsigned NN, unsigned MM>
metropolis_engine::MetropolisEngineStatisticalAccumulator
do_calculations(
        const double beta,
        const energy::getter::BoardEnergyGetter<N, M>& board_energy_getter,
        const energy::getter::NeighbourhoodEnergyGetter<7, 7>& neighbourhood_energy_getter,
        const unsigned n_thermal_steps,
        const unsigned n_average_steps) {
    metropolis_engine::MetropolisEngine<N, M, NN, MM> engine(beta, board_energy_getter, neighbourhood_energy_getter);
    engine.init_uniform();
    for (unsigned i =0 ; i < n_thermal_steps; i++) {
        engine.do_step();
    }
    for (unsigned i =0 ; i < n_average_steps; i++) {
        engine.do_step();
        engine.accumulate();
    }
    return engine.get_accumulator();
}

constexpr unsigned N = 10;
constexpr unsigned M = 10;

int main(int argc, char** argv) {
    // ******************************************************************
    const ProgramOptions program_options = grep_program_options(argc, argv);
    // ******************************************************************
    std::cout << "[INFO   ] [PROGRAM_OPTIONS] temerature_steps_string = " << program_options.temerature_steps_string << std::endl;
    std::cout << "[INFO   ] [PROGRAM_OPTIONS] n_thermal_steps         = " << program_options.n_thermal_steps << std::endl;
    std::cout << "[INFO   ] [PROGRAM_OPTIONS] n_average_steps         = " << program_options.n_average_steps << std::endl;
    std::cout << "[INFO   ] [PROGRAM_OPTIONS] model                   = " << program_options.model << std::endl;
    std::cout << "[INFO   ] [PROGRAM_OPTIONS] ising_multiplicity      = " << program_options.ising_multiplicity << std::endl;
    std::cout << "[INFO   ] [PROGRAM_OPTIONS] path_to_chached_data    = " << program_options.path_to_chached_data << std::endl;
    // ******************************************************************
    const InterpretedProgramOptions interpreted_program_options = interpret_program_options(program_options);
    // ******************************************************************
    std::cout << "[INFO   ] [PROGRAM_OPTIONS] temerature_steps = ";
    for (const auto temperatue : interpreted_program_options.temerature_steps) {
        std::cout << temperatue << ", ";
    }
    std::cout << std::endl;
    // ******************************************************************
    //SET MODEL AND ENERGY GETTERS: TODO
    const energy::ising::doublet::FourNnPartsEnergyGetter parts_energy_getter;
    //const energy::ising::multiplet::FourNnPartsEnergyGetter parts_energy_getter{2};
    const energy::getter::BoardEnergyGetter<N, M>& board_energy_getter =
            energy::getter::FourNnBoardEnergyGetter<N, M>{parts_energy_getter};
    const energy::getter::NeighbourhoodEnergyGetter<7, 7>& neighbourhood_energy_getter =
            energy::getter::FourNnNeighbourhoodEnergyGetter{parts_energy_getter};
    // ******************************************************************
    //const double ising_tc = 2 / std::log(1 + std::sqrt(2)) / 4;
    const unsigned n_thermal_steps = interpreted_program_options.n_thermal_steps;
    const unsigned n_average_steps = interpreted_program_options.n_average_steps;
    std::cout << "n_thermal_steps: " << n_thermal_steps << std::endl;
    std::cout << "n_average_steps: " << n_average_steps << std::endl;
    for (const auto temperatue : interpreted_program_options.temerature_steps) {
        const double beta = 1.0 / temperatue;
        std::cout << "temperature, beta:        " << temperatue << ", " << beta << std::endl;
        const std::chrono::steady_clock::time_point calculation_time_begin = std::chrono::steady_clock::now();
        const auto results = do_calculations<N, M, 7, 7>(beta,
                                                         board_energy_getter,
                                                         neighbourhood_energy_getter,
                                                         n_thermal_steps,
                                                         n_average_steps);
        const std::chrono::steady_clock::time_point calculation_time_end = std::chrono::steady_clock::now();
        std::cout << "simulation time: "
                  <<  std::chrono::duration_cast<std::chrono::microseconds>(calculation_time_end - calculation_time_begin).count() / 1e6
                   << "[s]" << std::endl;
        const double average_energy = results.get_average_energy();
        const double average_sq_energy = results.get_average_sq_energy();
        const double average_sigma_energy = average_sq_energy - average_energy * average_energy;
        const double specific_heat = average_sigma_energy * beta * beta;
        const double average_magnetization = results.get_average_magnetization();
        const double average_abs_magnetization = results.get_average_abs_magnetization();
        //        std::cout << "steps:        " << results.get_steps() << std::endl;
        //        std::cout << "E:            " << average_energy << std::endl;
        //        std::cout << "E*E:          " << average_sq_energy << std::endl;
        //        std::cout << "σ E:          " << average_sigma_energy << std::endl;
        //        std::cout << "C:            " << specific_heat << std::endl;
        //        std::cout << "M:            " << average_magnetization << std::endl;
        //        std::cout << "|M|:          " << average_abs_magnetization << std::endl;
        std::cout << "E(" << temperatue << "):   " << average_energy / (N * M) << std::endl;
        //std::cout << "E^2(" << temperatue << "): " << average_sq_energy / (N * M) << std::endl;
        std::cout << "σE(" << temperatue << ") : " << average_sigma_energy / (N * M) << std::endl;
        std::cout << "C(" << temperatue << "):   " << specific_heat / (N * M) << std::endl;
        //std::cout << "M(" << temperatue << "):   " << average_magnetization / (N * M) << std::endl;
        std::cout << "|M|(" << temperatue << "): " << average_abs_magnetization / (N * M) << std::endl;
    }
    return 0;
}
