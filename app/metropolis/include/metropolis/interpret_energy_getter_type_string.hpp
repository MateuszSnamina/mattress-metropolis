#pragma once

// METROPOLIS:
#include <metropolis/enum_energy_getter_type.hpp>
// UTILITY:
#include <utility/result.hpp>
// STL:
#include <map>
// STD:
#include <stdexcept>
#include <string>

extern const std::map<std::string, EnergyGetterType> interpret_energy_getter_type_string_map;

utility::Result<EnergyGetterType, std::domain_error> interpret_energy_getter_type_string(const std::string&);
