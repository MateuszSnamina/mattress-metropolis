#pragma once

#include <string>

struct RawProgramOptions {
  std::string temerature_steps_string;
  std::string n_thermal_steps_string;
  std::string n_average_steps_string;
  std::string model_type_string;
  std::string energy_getter_type_string;
  unsigned ising_multiplicity;
  std::string path_to_chached_data;
};

RawProgramOptions grep_program_options(int argc, char** argv);
