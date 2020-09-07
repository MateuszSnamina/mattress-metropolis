// SELF:
#include <metropolis/interpreted_program_options.hpp>
// METROPOLIS:
#include <metropolis/raw_program_options.hpp>
#include <metropolis/interpret_number_string.hpp>
#include <metropolis/interpret_steps_string.hpp>
// STD:
#include <stdexcept>
#include <iostream> //TODO temp
namespace {

ModelType interpret_model(const std::string& model_string) {
    if (model_string == "ising::doublet") {
        return ModelType::IsingDoublet;
    } else if (model_string == "ising::multiplet") {
        return ModelType::IsingMultiplet;
    } else if (model_string == "parametrized") {
        return ModelType::Parametrized;
    }
    const std::string message = "Unknown model string '" + model_string + "'.";
    throw std::domain_error(message);
}

}  // namespace

InterpretedProgramOptions interpret_program_options(const ProgramOptions& raw_program_options) {
    InterpretedProgramOptions interpreted_program_options;
    if (const auto _ = interpret_steps_string(raw_program_options.temerature_steps_string)){
        interpreted_program_options.temerature_steps = _.unwrap();
    } else {
        const std::string message1 = "Problem with interpreting 'temerature_steps_string' program option.";
        const std::string message = message1 + "\n" + _.unwrap_err().what();
        throw std::runtime_error(message);
    }

    if (const auto _ = interpret_unsigned_string(raw_program_options.n_average_steps_string)) {
        interpreted_program_options.n_average_steps = _.unwrap();
    } else {
        const std::string message1 = "Problem with interpreting 'n_average_steps_string' program option.";
        const std::string message = message1 + "\n" + _.unwrap_err().what();
        throw std::runtime_error(message);
    }

    if (const auto _ = interpret_unsigned_string(raw_program_options.n_thermal_steps_string)) {
        interpreted_program_options.n_thermal_steps = _.unwrap();
    } else {
        const std::string message1 = "Problem with interpreting 'n_thermal_steps_string' program option.";
        const std::string message = message1 + "\n" + _.unwrap_err().what();
        throw std::runtime_error(message);
    }

    interpreted_program_options.model = interpret_model(raw_program_options.model); //TODO
    interpreted_program_options.ising_multiplicity = raw_program_options.ising_multiplicity;
    interpreted_program_options.path_to_chached_data = raw_program_options.path_to_chached_data;
    return interpreted_program_options;
}
