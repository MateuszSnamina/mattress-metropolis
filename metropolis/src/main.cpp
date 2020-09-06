// METRPOPOLIS-ENGINE:
#include <metropolis_engine/metropolis_engine.hpp>
// ENERGY:
#include <energy/energy_getter_ising_doublet.hpp>
// NUMBOARD:
#include <numboard/numboard.hpp>
// STL:
// STD:
#include<iostream>

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
    const energy::ising::doublet::FourNnPartsEnergyGetter parts_energy_getter;
    const energy::getter::BoardEnergyGetter<N, M>& board_energy_getter =
            energy::getter::FourNnBoardEnergyGetter<N, M>{parts_energy_getter};
    const energy::getter::NeighbourhoodEnergyGetter<7, 7>& neighbourhood_energy_getter =
            energy::getter::FourNnNeighbourhoodEnergyGetter{parts_energy_getter};
    const double beta = 1.0 / 1.5;
//    const unsigned n_thermal_steps = 1e6;
//    const unsigned n_average_steps = 10 * 1e6;
    const unsigned n_thermal_steps = 0;
    const unsigned n_average_steps = 1e5;

    const auto results = do_calculations<N, M, 7, 7>(beta,
                                  board_energy_getter,
                                  neighbourhood_energy_getter,
                                  n_thermal_steps,
                                  n_average_steps);

    const double average_energy = results.get_average_energy();
    const double average_sq_energy = results.get_average_sq_energy();
    const double average_sigma_energy = average_sq_energy - average_energy * average_energy;
    const double specific_heat = average_sigma_energy * beta * beta;
    const double average_magnetization = results.get_average_magnetization();
    const double average_abs_magnetization = results.get_average_abs_magnetization();

    std::cout << "steps:        " << results.get_steps() << std::endl;
    std::cout << "E:            " << average_energy << std::endl;
    std::cout << "E*E:          " << average_sq_energy << std::endl;
    std::cout << "σ E:          " << average_sigma_energy << std::endl;
    std::cout << "C:            " << specific_heat << std::endl;
    std::cout << "M:            " << average_magnetization << std::endl;
    std::cout << "|M|:          " << average_abs_magnetization << std::endl;
    std::cout << "E / #sites:   " << average_energy / (N * M) << std::endl;
    std::cout << "E*E / #sites: " << average_sq_energy / (N * M) << std::endl;
    std::cout << "σ E / #sites: " << average_sigma_energy / (N * M) << std::endl;
    std::cout << "C / #sites:   " << specific_heat / (N * M) << std::endl;
    std::cout << "M / #sites:   " << average_magnetization / (N * M) << std::endl;
    std::cout << "|M| / #sites: " << average_abs_magnetization / (N * M) << std::endl;

    return 0;
}



/*
int main(int argc, char** argv) {
    // ******************************************************************
    ProgramOptions program_options = grep_program_options(argc, argv);
    std::cout << "[INFO   ] [PROGRAM_OPTIONS] path_to_conductivity_image = " << program_options.path_to_conductivity_image << std::endl;
    std::cout << "[INFO   ] [PROGRAM_OPTIONS] path_to_dimension_image    = " << program_options.path_to_dimension_image << std::endl;
    // ******************************************************************
    // calculation parameters:
    const double const_conductivity = 1.0;
    const double variable_conductivity = 13.0;
    // ******************************************************************
    // The image data:
    std::string path = "/home/mateusz/img_to_rset_v2/img/frame4.ppm";
    arma::cube image_data;
    image_data.load(path, arma::ppm_binary);
    // ******************************************************************
    // The image legend:
    const auto decode_is_vaccum = [](arma::vec3 rgb) -> bool {
        return rgb(0) > 200;
    };

    const auto decode_conductivity = [const_conductivity, variable_conductivity](arma::vec3 rgb) -> double {
        return const_conductivity + variable_conductivity * (rgb(1) / 255.0);
    };
    const conductor_solver::ImageLegend image_legend = {decode_is_vaccum, decode_conductivity};
    // ******************************************************************
    // do the experiments:
    const conductor_solver::ImageDataAndImageLegend image_data_end_image_legend{image_data, image_legend};
    std::cout << "[STAGE  ] Do experiment with row_pads." << std::endl;
    do_experiment_row_pads(image_data_end_image_legend);
    std::cout << "[STAGE  ] Do experiment with col pads." << std::endl;
    do_experiment_col_pads(image_data_end_image_legend);
    // ******************************************************************
    return 0;
}
*/
