#pragma once

// METRPOPOLIS:
#include <metropolis/raw_program_options.hpp>
// STD:
#include <string>
#include <vector>

enum class ModelType {
    IsingDoublet,
    IsingMultiplet,
    Parametrized
};

struct InterpretedProgramOptions {
  std::vector<double> temerature_steps;
  unsigned n_thermal_steps;
  unsigned n_average_steps;
  ModelType model;
  unsigned ising_multiplicity;
  std::string path_to_chached_data;
};

InterpretedProgramOptions interpret_program_options(const ProgramOptions&);
