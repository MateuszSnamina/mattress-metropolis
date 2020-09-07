#pragma once

// UTILITY:
#include <utility/result.hpp>
// STD:
#include <string>
#include <vector>
#include <stdexcept>

utility::Result<std::vector<double>, std::domain_error> interpret_steps_string(const std::string& steps_string);
