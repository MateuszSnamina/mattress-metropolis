#pragma once

// UTILITY
#include <utility/result.hpp>
// STD:
#include <string>
#include <optional>
#include <stdexcept>

//**********************************************************************
//***   interpret_{int,unsigned,double}_string                       ***
//**********************************************************************

utility::Result<double, std::domain_error> interpret_double_string(const std::string& s);
utility::Result<int, std::domain_error> interpret_int_string(const std::string& s);
utility::Result<unsigned, std::domain_error> interpret_unsigned_string(const std::string& s);
