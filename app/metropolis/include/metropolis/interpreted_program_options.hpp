#pragma once


// METRPOPOLIS:
#include <metropolis/raw_program_options.hpp>
#include <metropolis/enum_energy_getter_type.hpp>
#include <metropolis/enum_model_type.hpp>
// STD:
#include <string>
#include <vector>

struct InterpretedProgramOptions {
    std::vector<double> temerature_steps;
    unsigned n_thermal_steps;
    unsigned n_average_steps;
    ModelType model_type;
    EnergyGetterType energy_getter_type;
    unsigned ising_multiplicity;
    std::string path_to_chached_data;
};

InterpretedProgramOptions interpret_program_options(const RawProgramOptions&);
