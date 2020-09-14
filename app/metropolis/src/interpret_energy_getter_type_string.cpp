// SELF:
#include <metropolis/interpret_energy_getter_type_string.hpp>
// EXTENSIONS:
#include <extensions/range_streamer.hpp>
// BOOST:
#include <boost/range/adaptor/map.hpp>

const extern std::map<std::string, EnergyGetterType> interpret_energy_getter_type_string_map = {
    {"0nn", EnergyGetterType::ZeroNn},
    {"4nn", EnergyGetterType::FourNn},
    {"8nn", EnergyGetterType::EightNn},
};

utility::Result<EnergyGetterType, std::domain_error> interpret_energy_getter_type_string(const std::string& energy_getter_string) {
    using namespace extension::boost::stream_pragma;
    using ResultT = utility::Result<EnergyGetterType, std::domain_error>;
    if (interpret_energy_getter_type_string_map.count(energy_getter_string)) {
        return ResultT::Ok(interpret_energy_getter_type_string_map.at(energy_getter_string));
    } else {
        const std::string message1 = "Invalid energy getter type string '" + energy_getter_string + "'.";
        const auto range_stream_settings = RSS().set_null_sustainer().set_string_separer(", ");
        const std::string possible_values = (interpret_energy_getter_type_string_map | boost::adaptors::map_keys| range_stream_settings).str();
        const std::string message2 = "Valid strings are: " + possible_values + ".";
        const std::string message = message1 + " " + message2;
        return ResultT::Err(std::domain_error(message));
    }
}
