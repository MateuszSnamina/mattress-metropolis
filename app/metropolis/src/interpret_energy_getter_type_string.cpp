// SELF:
#include <metropolis/interpret_energy_getter_type_string.hpp>
// STL:
#include<map>

utility::Result<EnergyGetterType, std::domain_error> interpret_energy_getter_type_string(const std::string& energy_getter_string) {
    using ResultT = utility::Result<EnergyGetterType, std::domain_error>;
    static const std::map<std::string, EnergyGetterType> m = {
        {"4nn", EnergyGetterType::FourNn},
        {"8nn", EnergyGetterType::EightNn},
    };
    if (!m.count(energy_getter_string)) {
        const std::string message1 = "Invalid energy getter type string '" + energy_getter_string + "'.";
        //const std::string message2 = Valid strings are // TODO enumerate m keys;
        const std::string message = message1;
        return ResultT::Err(std::domain_error(message));
    } else {
        return ResultT::Ok(m.at(energy_getter_string));
    }
}
