#pragma once

#include <extensions/stream_fromat_stacker.hpp>

#include <boost/range.hpp>
#include <boost/range/adaptor/indexed.hpp>

#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

// #######################################################################
// ##  RangeStreamerSettings                                            ##
// #######################################################################

namespace extension::boost {

struct RangeStreamerSettings {
    // Setters - general:
    RangeStreamerSettings& set_stream_preparer(::std::function<void(::std::ostream&)>);
    RangeStreamerSettings& set_stream_sustainer(::std::function<void(::std::ostream&, size_t)>);
    RangeStreamerSettings& set_stream_separer(::std::function<void(::std::ostream&)>);
    RangeStreamerSettings& set_stream_finisher(::std::function<void(::std::ostream&)>);
    RangeStreamerSettings& set_format_independence_flag(bool = true);
    // Setters - convenience:
    RangeStreamerSettings& set_null_preparer();
    RangeStreamerSettings& set_char_preparer(char c);
    RangeStreamerSettings& set_string_preparer(::std::string str);
    RangeStreamerSettings& set_null_sustainer();
    RangeStreamerSettings& set_null_separer();
    RangeStreamerSettings& set_char_separer(char c);
    RangeStreamerSettings& set_string_separer(::std::string str);
    RangeStreamerSettings& set_null_finisher();
    RangeStreamerSettings& set_char_finisher(char c);
    RangeStreamerSettings& set_string_finisher(::std::string str);
    RangeStreamerSettings& in_bracket_round();
    RangeStreamerSettings& in_bracket_curly();
    RangeStreamerSettings& in_bracket_square();
    RangeStreamerSettings& in_bracket_angle();
    RangeStreamerSettings& in_quotation_single();
    RangeStreamerSettings& in_quotation_double();
    RangeStreamerSettings& in_quotation_back();
    // Fields:
    ::std::function<void(::std::ostream&)> _stream_preparer =
            [](::std::ostream& s) { s << "{"; };
    ::std::function<void(::std::ostream&, size_t)> _stream_sustainer =
            [](::std::ostream& s, size_t i) { s << i << ":"; };
    ::std::function<void(::std::ostream&)> _stream_separer =
            [](::std::ostream& s) { s << ","; };
    ::std::function<void(::std::ostream&)> _stream_finisher =
            [](::std::ostream& s) { s << "}"; };
    bool _format_independence_flag = true;
};

// ***********************************************************************

inline RangeStreamerSettings& RangeStreamerSettings::set_stream_preparer(::std::function<void(::std::ostream&)> _) {
    _stream_preparer = _;
    return *this;
}

inline RangeStreamerSettings& RangeStreamerSettings::set_stream_sustainer(::std::function<void(::std::ostream&, size_t)> _) {
    _stream_sustainer = _;
    return *this;
}

inline RangeStreamerSettings& RangeStreamerSettings::set_stream_separer(::std::function<void(::std::ostream&)> _) {
    _stream_separer = _;
    return *this;
}

inline RangeStreamerSettings& RangeStreamerSettings::set_format_independence_flag(bool _) {
    _format_independence_flag = _;
    return *this;
}

inline RangeStreamerSettings& RangeStreamerSettings::set_stream_finisher(::std::function<void(::std::ostream&)> _) {
    _stream_finisher = _;
    return *this;
}

inline RangeStreamerSettings& RangeStreamerSettings::set_null_preparer() {
    _stream_preparer = [](::std::ostream&) {};
    return *this;
}

inline RangeStreamerSettings& RangeStreamerSettings::set_char_preparer(char c) {
    _stream_preparer = [c](::std::ostream& s) { s << c; };
    return *this;
}

inline RangeStreamerSettings& RangeStreamerSettings::set_string_preparer(::std::string str) {
    _stream_preparer = [str](::std::ostream& s) { s << str; };
    return *this;
}

inline RangeStreamerSettings& RangeStreamerSettings::set_null_sustainer() {
    _stream_sustainer = [](::std::ostream&, size_t) {};
    return *this;
}

inline RangeStreamerSettings& RangeStreamerSettings::set_null_separer() {
    _stream_separer = [](::std::ostream&) {};
    return *this;
}

inline RangeStreamerSettings& RangeStreamerSettings::set_char_separer(char c) {
    _stream_separer = [c](::std::ostream& s) { s << c; };
    return *this;
}

inline RangeStreamerSettings& RangeStreamerSettings::set_string_separer(::std::string str) {
    _stream_separer = [str](::std::ostream& s) { s << str; };
    return *this;
}

inline RangeStreamerSettings& RangeStreamerSettings::set_null_finisher() {
    _stream_finisher = [](::std::ostream&) {};
    return *this;
}

inline RangeStreamerSettings& RangeStreamerSettings::set_char_finisher(char c) {
    _stream_finisher = [c](::std::ostream& s) { s << c; };
    return *this;
}

inline RangeStreamerSettings& RangeStreamerSettings::set_string_finisher(::std::string str) {
    _stream_finisher = [str](::std::ostream& s) { s << str; };
    return *this;
}

inline RangeStreamerSettings& RangeStreamerSettings::in_bracket_round() {
    set_char_preparer('(');
    set_char_finisher(')');
    return *this;
}

inline RangeStreamerSettings& RangeStreamerSettings::in_bracket_curly() {
    set_char_preparer('{');
    set_char_finisher('}');
    return *this;
}

inline RangeStreamerSettings& RangeStreamerSettings::in_bracket_square() {
    set_char_preparer('[');
    set_char_finisher(']');
    return *this;
}

inline RangeStreamerSettings& RangeStreamerSettings::in_bracket_angle() {
    set_char_preparer('<');
    set_char_finisher('>');
    return *this;
}

inline RangeStreamerSettings& RangeStreamerSettings::in_quotation_single() {
    set_char_preparer('\'');
    set_char_finisher('\'');
    return *this;
}

inline RangeStreamerSettings& RangeStreamerSettings::in_quotation_double() {
    set_char_preparer('"');
    set_char_finisher('"');
    return *this;
}

inline RangeStreamerSettings& RangeStreamerSettings::in_quotation_back() {
    set_char_preparer('`');
    set_char_finisher('`');
    return *this;
}


}  // namespace extension::boost

// #######################################################################
// ##  RangeStreamer                                                    ##
// #######################################################################

namespace extension::boost {

template<class R>
class RangeStreamer {
public:
    RangeStreamer(const R& range, const RangeStreamerSettings& range_streamer_settings) :
        _range(range),
        _range_streamer_settings(range_streamer_settings) {
    }

    ::std::ostream& stream(::std::ostream& os) const {
        const extension::std::StreamFromatStacker stream_format_stacker(
                    os, _range_streamer_settings._format_independence_flag);
        _range_streamer_settings._stream_preparer(os);
        for (const auto& _ : _range | ::boost::adaptors::indexed(0)) {
            if (_.index() != 0) {
                _range_streamer_settings._stream_separer(os);
            }
            _range_streamer_settings._stream_sustainer(os, _.index());
            os << _.value();
        }
        _range_streamer_settings._stream_finisher(os);
        return os;
    }

    ::std::string str() const {
        ::std::ostringstream oss;
        stream(oss);
        return oss.str();
    }

    const R& _range;
    const RangeStreamerSettings& _range_streamer_settings;
};

namespace stream_pragma {

template<class R>
RangeStreamer<R> operator|(const R& range, const RangeStreamerSettings& range_streamer_settings){
    return RangeStreamer<R>(range, range_streamer_settings);
}

template<class R>
::std::ostream& operator<<(::std::ostream& os, const RangeStreamer<R>& range_streamer) {
    return range_streamer.stream(os);
}

struct Stringifier {
};

template<class R>
::std::string operator|(const RangeStreamer<R>& range_streamer, Stringifier) {
    return range_streamer.str();
}

using RSS = RangeStreamerSettings;

}  // namespace op
}  // namespace extension::boost
