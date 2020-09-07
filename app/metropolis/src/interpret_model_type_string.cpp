// SELF:
#include <metropolis/interpret_model_type_string.hpp>
// STL:
#include<map>

utility::Result<ModelType, std::domain_error> interpret_model_type_string(const std::string& model_type_string) {
    using ResultT = utility::Result<ModelType, std::domain_error>;
    static const std::map<std::string, ModelType> m = {
        {"ising::doublet", ModelType::IsingDoublet},
        {"ising::multiplet", ModelType::IsingMultiplet},
        {"parametrized", ModelType::Parametrized}
    };
    if (!m.count(model_type_string)) {
        const std::string message1 = "Invalid model type string '" + model_type_string + "'.";
        //const std::string message2 = Valid strings  are // TODO enumerate m keys;
        const std::string message = message1;
        return ResultT::Err(std::domain_error(message));
    } else {
        return ResultT::Ok(m.at(model_type_string));
    }
}
