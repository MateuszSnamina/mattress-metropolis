#pragma once

// METROPOLIS:
#include <metropolis/model_type.hpp>
// UTILITY:
#include <utility/result.hpp>
// STL:
#include <map>
// STD:
#include <stdexcept>
#include <string>

extern const std::map<std::string, ModelType> interpret_model_type_string_map;

utility::Result<ModelType, std::domain_error> interpret_model_type_string(const std::string&);
