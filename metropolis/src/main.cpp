// IMG_TO_RSET:
#include <metropolis/program_options.hpp>
// STL:
// STD:
#include<iostream>

int main(int argc, char** argv) {
  std::cout << "AAA" << std::endl;
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
