#pragma once

#include <string>

struct ProgramOptions {
  std::string temerature_steps_string;
  std::string n_thermal_steps_string;
  std::string n_average_steps_string;
  std::string model;
  unsigned ising_multiplicity;
  std::string path_to_chached_data;
};

ProgramOptions grep_program_options(int argc, char** argv);
