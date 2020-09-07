// SELF:
#include <metropolis/interpret_steps_string.hpp>
// METROPOLIS:
#include <metropolis/interpret_number_string.hpp>
// UTILITY:
#include <utility/result.hpp>
// BOOST:
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>//TODO remove
// STL:
#include <vector>
// STD:
#include <utility>
#include <regex>
#include <variant>
#include <optional>
#include <iostream>

//**********************************************************************
//***   SpanTermination, parse_span_termination                      ***
//**********************************************************************

namespace {

enum class SpanTermination {
    Inclusive,
    Exclusive
};

std::optional<SpanTermination> parse_span_termination(const std::string& s) {
    if (s == "[" || s == "]") {
        return SpanTermination::Inclusive;
    }
    if (s == "(" || s == ")") {
        return SpanTermination::Exclusive;
    }
    return std::nullopt;
}

}

//**********************************************************************
//***   SpanTokenData, interpret_steps_string_stage_1                ***
//**********************************************************************

namespace {

struct SpanTokenData {
    std::pair<SpanTermination, SpanTermination> opening_closing;
    std::pair<double, double> from_to;
    double step;
};

struct ValueTokenData {
    double value;
};

using TokensVectorT = std::vector<std::variant<SpanTokenData, ValueTokenData>>;

//struct TokenStageResults {
//    bool ok_flag;
//    std::string message;
//    std::vector<std::variant<SpanTokenData, ValueTokenData>> tokens_data;
//};

utility::Result<TokensVectorT, std::domain_error>
interpret_steps_string_stage_1(const std::string& steps_string) {
    using ResultT = utility::Result<TokensVectorT, std::domain_error>;
    std::vector<std::string> tokens_string;
    std::vector<std::variant<SpanTokenData, ValueTokenData>> tokens_data;
    boost::split(tokens_string, steps_string, boost::is_any_of(";"));
    std::string already_parsed;
    for (const auto& token : tokens_string) {
        //std::cout << "working on token: " << token << std::endl;
        const std::regex span_token_regex ("(\\[|\\()(.*),(.*)(\\]|\\))@(.*)");
        const std::regex value_token_regex ("([0-9eE+-.]*)");
        std::smatch m;
        if (std::regex_match(token, m, span_token_regex) ) {
            //std::cout << "  Identified span token." << std::endl;
            const std::string opening_string = m.str(1);
            const std::string from_string = m.str(2);
            const std::string to_string = m.str(3);
            const std::string closing_string = m.str(4);
            const std::string step_string = m.str(5);
            //std::cout << "opening_string: " << opening_string << std::endl;
            //std::cout << "closing_string: " << closing_string << std::endl;
            //std::cout << "from_string: " << from_string << std::endl;
            //std::cout << "to_string: " << to_string << std::endl;
            //std::cout << "step_string: " << step_string << std::endl;
            const auto from_optional = interpret_double_string(from_string);
            const auto to_optional = interpret_double_string(to_string);
            const auto step_optional = interpret_double_string(step_string);
            const auto opening_optional = parse_span_termination(opening_string);
            const auto closing_optional = parse_span_termination(closing_string);
            if (!opening_optional) {
                throw std::domain_error("Invalid spantermination '" + opening_string + "'.");
            }
            if (!closing_optional) {
                throw std::domain_error("Invalid spantermination '" + closing_string + "'.");
            }
            if (!from_optional) {
                const std::string message1 = "Problem with parsing token '" + token + "'.";
                const std::string message2 = "Cannot cast from string '" + from_string + "' into a number.";
                const std::string message = message1 + " " + message2;
                return ResultT::Err(std::domain_error(message));
            }
            if (!to_optional) {
                const std::string message1 = "Problem with parsing token '" + token + "'.";
                const std::string message2 = "Cannot cast to string '" + to_string + "' into a number.";
                const std::string message = message1 + " " + message2;
                return ResultT::Err(std::domain_error(message));

            }
            if (!step_optional) {
                const std::string message1 = "Problem with parsing token '" + token + "'.";
                const std::string message2 = "Cannot cast step string '" + step_string + "' into a number.";
                const std::string message = message1 + " " + message2;
                return ResultT::Err(std::domain_error(message));
            }
            SpanTokenData token_data{
                {*opening_optional, *closing_optional},
                {*from_optional, *to_optional},
                *step_optional};
            tokens_data.push_back(token_data);
            continue;
        }
        if (std::regex_match(token, m, value_token_regex) ) {
            //std::cout << "  Identified value token." << std::endl;
            const auto value_optional = interpret_double_string(token);
            if (!value_optional) {
                const std::string message1 = "Problem with parsing token '" + token + "'.";
                const std::string message2 = "Cannot cast value string '" + token + "' into a number.";
                const std::string message = message1 + " " + message2;
                return ResultT::Err(std::domain_error(message));
            }
            ValueTokenData token_data{*value_optional};
            tokens_data.push_back(token_data);
            continue;
        }
        {
            const std::string message = "Problem with parsing token '" + token + "'.";
            return ResultT::Err(std::domain_error(message));
        }
    }
    return ResultT::Ok(tokens_data);
}

struct StepsAppender {
    StepsAppender(std::vector<double>& steps) :
        _steps(steps){
    };
    void operator()(const ValueTokenData& token) {
        _steps.push_back(token.value);
    }
    void operator()(const SpanTokenData& token) {
        const auto& opening = token.opening_closing.first ;
        const auto& closing = token.opening_closing.second;
        const auto& from = token.from_to.first;
        const auto& to = token.from_to.second;
        const auto& step = token.step;
        if (opening == SpanTermination::Inclusive && closing == SpanTermination::Inclusive) {
            for (double value = from ; value <= to + 1e-10 * step; value +=step) {
                _steps.push_back(value);
            }
        } else if (opening == SpanTermination::Inclusive && closing == SpanTermination::Exclusive) {
            for (double value = from ; value < to - 1e-10 * step; value +=step) {
                _steps.push_back(value);
            }
        } else if (opening == SpanTermination::Exclusive && closing == SpanTermination::Inclusive) {
            for (double value = from + step; value <= to + 1e-10 * step; value +=step) {
                _steps.push_back(value);
            }
        } else if (opening == SpanTermination::Exclusive && closing == SpanTermination::Exclusive) {
            for (double value = from + step ; value < to - 1e-10 * step; value +=step) {
                _steps.push_back(value);
            }
        }
    }
    std::vector<double>& _steps;
};

std::vector<double> interpret_steps_string_stage_2(
        const std::vector<std::variant<SpanTokenData, ValueTokenData>>& tokens_data) {
    std::vector<double> result;
    for (const auto& token : tokens_data) {
        std::visit(StepsAppender(result), token);
    }
    return result;
}

} // namespae

utility::Result<std::vector<double>, std::domain_error> interpret_steps_string(const std::string& steps_string){
    using ResultT = utility::Result<std::vector<double>, std::domain_error>;
    const auto steps_parse_results = interpret_steps_string_stage_1(steps_string);
    if (steps_parse_results) {
        const std::vector<double> steps = interpret_steps_string_stage_2(steps_parse_results.unwrap());
        return ResultT::Ok(steps);
    } else {
        return ResultT::Err(steps_parse_results.unwrap_err());
    }
}
