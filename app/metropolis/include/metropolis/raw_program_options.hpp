#pragma once

#include <string>

struct ProgramOptions {
  std::string temerature_steps_string;
  unsigned n_thermal_steps;
  unsigned n_average_steps;
  std::string model;
  unsigned ising_multiplicity;
  std::string path_to_chached_data;
};

ProgramOptions grep_program_options(int argc, char** argv);
