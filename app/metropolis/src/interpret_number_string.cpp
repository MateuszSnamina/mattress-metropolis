//SELF:
#include <metropolis/interpret_number_string.hpp>
// BOOST:
#include <boost/lexical_cast.hpp>
// STL:
#include <map>
// STD:
#include <optional>
#include <iostream> //TODO remove

namespace {

//**********************************************************************
//***   {integer,real}_suffix_char_to_suffix_value                   ***
//**********************************************************************

std::optional<double> real_suffix_char_to_suffix_value(const char c) {
    static const std::map<char, double> m {
        {'T', 1e+12},
        {'G', 1e+9},
        {'M', 1e+6},
        {'k', 1e+3},
        {'m', 1e-3},
        {'u', 1e-6},
        {'n', 1e-9},
        {'p', 1e-12}
    };
    if (m.count(c)) {
        return m.at(c);
    }
    return std::nullopt;
}

std::optional<unsigned> integer_suffix_char_to_suffix_value(const char c) {
    static const std::map<char, unsigned> m {
        {'T', 1e+12},
        {'G', 1e+9},
        {'M', 1e+6},
        {'k', 1e+3},
    };
    if (m.count(c)) {
        return m.at(c);
    }
    return std::nullopt;
}

}

//**********************************************************************
//***   find_{integer,real}_suffix                                   ***
//**********************************************************************

namespace  {

struct RealSuffixSplitterResult {
    std::string string_without_suffix;
    double factor;
};

struct IntegerSuffixSplitterResult {
    std::string string_without_suffix;
    unsigned factor;
};

RealSuffixSplitterResult split_real_suffix(std::string s) {
    if (!s.empty()) {
        const auto& suffix_char = s.back();
        if (const auto suffix_value = real_suffix_char_to_suffix_value(suffix_char)) {
            s.pop_back();
            return {s, *suffix_value};
        }
    }
    return {s, 1};
}

IntegerSuffixSplitterResult split_integer_suffix(std::string s) {
    if (!s.empty()) {
        const auto& suffix_char = s.back();
        if (const auto suffix_value = integer_suffix_char_to_suffix_value(suffix_char)) {
            s.pop_back();
            return {s, *suffix_value};
        }
    }
    return {s, 1};
}

}

//**********************************************************************
//***   interpret_{int,unsigned,double}_string                       ***
//**********************************************************************

utility::Result<double, std::domain_error> interpret_double_string(const std::string& s);
utility::Result<int, std::domain_error> interpret_int_string(const std::string& s);
utility::Result<unsigned, std::domain_error> interpret_unsigned_string(const std::string& s);


utility::Result<double, std::domain_error> interpret_double_string(const std::string& s) {
    using ResultT = utility::Result<double, std::domain_error>;
    const auto splitter_result = split_real_suffix(s);
    try {
        return ResultT::Ok(boost::lexical_cast<double>(splitter_result.string_without_suffix) * splitter_result.factor);
    } catch (boost::bad_lexical_cast& e) {
        return ResultT::Err(std::domain_error(e.what()));
    }
}

utility::Result<int, std::domain_error> interpret_int_string(const std::string& s) {
    using ResultT = utility::Result<int, std::domain_error>;
    const auto splitter_result = split_integer_suffix(s);
    try {
        return ResultT::Ok(boost::lexical_cast<int>(splitter_result.string_without_suffix) * splitter_result.factor);
    } catch (boost::bad_lexical_cast& e) {
        return ResultT::Err(std::domain_error(e.what()));
    }
}

utility::Result<unsigned, std::domain_error> interpret_unsigned_string(const std::string& s) {
    using ResultT = utility::Result<unsigned, std::domain_error>;
    const auto splitter_result = split_integer_suffix(s);
    try {
        return ResultT::Ok(boost::lexical_cast<unsigned>(splitter_result.string_without_suffix) * splitter_result.factor);
    } catch (boost::bad_lexical_cast& e) {
        return ResultT::Err(std::domain_error(e.what()));
    }
}
