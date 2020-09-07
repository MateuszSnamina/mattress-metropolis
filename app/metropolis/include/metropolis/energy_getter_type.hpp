#pragma once

// STD:
#include <string>
#include <ostream>
#include <stdexcept>

enum class EnergyGetterType {
    FourNn,
    EightNn
};

inline std::ostream& operator<<(std::ostream& s, const EnergyGetterType& eg) {
    if (eg == EnergyGetterType::FourNn) {
        s << "EnergyGetterType::FourNn";
    } else if (eg == EnergyGetterType::EightNn) {
        s << "EnergyGetterType::EightNn";
    } else {
        throw std::logic_error("Invalid energy_getter enum value!");
    }
    return s;
}
