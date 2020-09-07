#pragma once

// METROPOLIS:
#include <metropolis/energy_getter_type.hpp>
// UTILITY:
#include <utility/result.hpp>
// STD:
#include <stdexcept>
#include <string>

utility::Result<EnergyGetterType, std::domain_error> interpret_energy_getter_type_string(const std::string&);
