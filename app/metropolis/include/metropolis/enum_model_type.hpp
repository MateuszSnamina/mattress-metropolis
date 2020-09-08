#pragma once

// STD:
#include <string>
#include <ostream>
#include <stdexcept>

enum class ModelType {
    IsingDoublet,
    IsingMultiplet,
    Parametrized
};

inline std::ostream& operator<<(std::ostream& s, const ModelType& m) {
    if (m == ModelType::IsingDoublet) {
        s << "ModelType::IsingDoublet";
    } else if (m == ModelType::IsingMultiplet) {
        s << "ModelType::IsingMultiplet";
    } else if (m == ModelType::Parametrized) {
        s << "ModelType::Parametrized";
    } else {
        throw std::logic_error("Invalid model enum value!");
    }
    return s;
}
