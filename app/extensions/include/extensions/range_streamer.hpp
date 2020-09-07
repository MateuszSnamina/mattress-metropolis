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
    // Setters:
    RangeStreamerSettings& set_stream_preparer(::std::function<void(::std::ostream&)>);
    RangeStreamerSettings& set_stream_sustainer(::std::function<void(::std::ostream&, size_t)>);
    RangeStreamerSettings& set_stream_separer(::std::function<void(::std::ostream&)>);
    RangeStreamerSettings& set_stream_finisher(::std::function<void(::std::ostream&)>);
    RangeStreamerSettings& set_format_independence_flag(bool = true);
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

}  // namespace extension::boost

// #######################################################################
// ##  RangeStreamer                                                    ##
// #######################################################################

namespace extension::boost {

class RangeStreamer {
   public:  // Ctor:
    RangeStreamer(::std::ostream& os, RangeStreamerSettings = RangeStreamerSettings());

   public:  // Internal ostream assessor:
    ::std::ostream& ostream();
    const ::std::ostream& ostream() const;

   public:  // Setters for fine streaming settings:
    RangeStreamer& set_range_streamer_settings(RangeStreamerSettings);
    RangeStreamer& set_stream_preparer(::std::function<void(::std::ostream&)>);
    RangeStreamer& set_stream_sustainer(::std::function<void(::std::ostream&, size_t)>);
    RangeStreamer& set_stream_separer(::std::function<void(::std::ostream&)>);
    RangeStreamer& set_stream_finisher(::std::function<void(::std::ostream&)>);
    RangeStreamer& set_format_independence_flag(bool = true);

   public:  // The streaming function:
    template <typename SinglePassRange>
    RangeStreamer& stream(const SinglePassRange& rng);

   private:
    ::std::ostream& _os;
    RangeStreamerSettings _range_streamer_settings;
};

// ***********************************************************************

inline RangeStreamer::RangeStreamer(::std::ostream& os, RangeStreamerSettings range_streamer_settings)
    : _os(os),
      _range_streamer_settings(range_streamer_settings) {}

// ***********************************************************************

inline RangeStreamer& RangeStreamer::set_range_streamer_settings(RangeStreamerSettings _) {
    _range_streamer_settings = _;
    return *this;
}
inline RangeStreamer& RangeStreamer::set_stream_preparer(::std::function<void(::std::ostream&)> _) {
    _range_streamer_settings.set_stream_preparer(_);
    return *this;
}

inline RangeStreamer& RangeStreamer::set_stream_sustainer(::std::function<void(::std::ostream&, size_t)> _) {
    _range_streamer_settings.set_stream_sustainer(_);
    return *this;
}

inline RangeStreamer& RangeStreamer::set_stream_separer(::std::function<void(::std::ostream&)> _) {
    _range_streamer_settings.set_stream_separer(_);
    return *this;
}

inline RangeStreamer& RangeStreamer::set_stream_finisher(::std::function<void(::std::ostream&)> _) {
    _range_streamer_settings.set_stream_finisher(_);
    return *this;
}

inline RangeStreamer& RangeStreamer::set_format_independence_flag(bool _) {
    _range_streamer_settings.set_format_independence_flag(_);
    return *this;
}

template <typename SinglePassRange>
RangeStreamer& RangeStreamer::stream(const SinglePassRange& rng) {
    const extension::std::StreamFromatStacker stream_format_stacker(_os, _range_streamer_settings._format_independence_flag);
    _range_streamer_settings._stream_preparer(_os);
    for (const auto& _ : rng | ::boost::adaptors::indexed(0)) {
        if (_.index() != 0) {
            _range_streamer_settings._stream_separer(_os);
        }
        _range_streamer_settings._stream_sustainer(_os, _.index());
        _os << _.value();
    }
    _range_streamer_settings._stream_finisher(_os);
    return *this;
}

inline ::std::ostream& RangeStreamer::ostream() {
    return _os;
}

inline const ::std::ostream& RangeStreamer::ostream() const {
    return _os;
}

// #######################################################################
// ##  RangeStringStreamer                                              ##
// #######################################################################

class RangeStringStreamer {
   public:  // Ctor:
    RangeStringStreamer(RangeStreamerSettings = RangeStreamerSettings());

   public:  // Setters for fine streaming settings:
    RangeStringStreamer& set_range_streamer_settings(RangeStreamerSettings);
    RangeStringStreamer& set_stream_preparer(::std::function<void(::std::ostream&)>);
    RangeStringStreamer& set_stream_sustainer(::std::function<void(::std::ostream&, size_t)>);
    RangeStringStreamer& set_stream_separer(::std::function<void(::std::ostream&)>);
    RangeStringStreamer& set_stream_finisher(::std::function<void(::std::ostream&)>);
    RangeStringStreamer& set_format_independence_flag(bool = true);

   public:  // The streaming function:
    template <typename SinglePassRange>
    RangeStringStreamer& stream(const SinglePassRange& rng);

   public:  // Function to retreive the streaming result
    ::std::string str() const;

   private:
    ::std::ostringstream _oss;
    RangeStreamer _rs;
};

// ***********************************************************************

inline RangeStringStreamer::RangeStringStreamer(RangeStreamerSettings range_streamer_settings)
    : _rs(_oss, range_streamer_settings) {
}

// ***********************************************************************

inline RangeStringStreamer& RangeStringStreamer::set_range_streamer_settings(RangeStreamerSettings _) {
    _rs.set_range_streamer_settings(_);
    return *this;
}
inline RangeStringStreamer& RangeStringStreamer::set_stream_preparer(::std::function<void(::std::ostream&)> _) {
    _rs.set_stream_preparer(_);
    return *this;
}

inline RangeStringStreamer& RangeStringStreamer::set_stream_sustainer(::std::function<void(::std::ostream&, size_t)> _) {
    _rs.set_stream_sustainer(_);
    return *this;
}

inline RangeStringStreamer& RangeStringStreamer::set_stream_separer(::std::function<void(::std::ostream&)> _) {
    _rs.set_stream_separer(_);
    return *this;
}

inline RangeStringStreamer& RangeStringStreamer::set_stream_finisher(::std::function<void(::std::ostream&)> _) {
    _rs.set_stream_finisher(_);
    return *this;
}

inline RangeStringStreamer& RangeStringStreamer::set_format_independence_flag(bool _) {
    _rs.set_format_independence_flag(_);
    return *this;
}

template <typename SinglePassRange>
RangeStringStreamer& RangeStringStreamer::stream(const SinglePassRange& rng) {
    _rs.stream(rng);
    return *this;
}

inline ::std::string RangeStringStreamer::str() const {
    return _oss.str();
}

}  // namespace extension::boost
