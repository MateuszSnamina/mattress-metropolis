#pragma once

// METROPOLIS:
#include <metropolis/model_type.hpp>
// UTILITY:
#include <utility/result.hpp>
// STD:
#include <stdexcept>
#include <string>

utility::Result<ModelType, std::domain_error> interpret_model_type_string(const std::string&);
