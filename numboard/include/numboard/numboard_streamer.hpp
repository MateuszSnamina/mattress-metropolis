# pragma once

// SELF:
#include<numboard/numboard.hpp>
// STD:
#include<iostream>
#include<iomanip>
#include<functional>

//**********************************************************************
//***   NumboardStreamerSettings                                     ***
//**********************************************************************

namespace numboard {

struct NumboardStreamerSettings {
    // Setters:
    NumboardStreamerSettings& set_stream_preparer(::std::function<void(::std::ostream&)>);
    NumboardStreamerSettings& set_stream_sustainer(::std::function<void(::std::ostream&, size_t)>);
    NumboardStreamerSettings& set_stream_separer(::std::function<void(::std::ostream&)>);
    NumboardStreamerSettings& set_stream_finisher(::std::function<void(::std::ostream&)>);
    NumboardStreamerSettings& set_format_independence_flag(bool = true);
    // Fields:
    ::std::function<void(::std::ostream&)> _stream_preparer =
            [](::std::ostream& s) { };
    ::std::function<void(::std::ostream&, size_t)> _stream_sustainer =
            [](::std::ostream& s, size_t i) { s << std::setw(2); };
    ::std::function<void(::std::ostream&)> _stream_separer =
            [](::std::ostream& s) { s << " "; };
    ::std::function<void(::std::ostream&)> _stream_finisher =
            [](::std::ostream& s) { };
    bool _format_independence_flag = true;
};

// ***********************************************************************

inline NumboardStreamerSettings& NumboardStreamerSettings::set_stream_preparer(::std::function<void(::std::ostream&)> _) {
    _stream_preparer = _;
    return *this;
}

inline NumboardStreamerSettings& NumboardStreamerSettings::set_stream_sustainer(::std::function<void(::std::ostream&, size_t)> _) {
    _stream_sustainer = _;
    return *this;
}

inline NumboardStreamerSettings& NumboardStreamerSettings::set_stream_separer(::std::function<void(::std::ostream&)> _) {
    _stream_separer = _;
    return *this;
}

inline NumboardStreamerSettings& NumboardStreamerSettings::set_format_independence_flag(bool _) {
    _format_independence_flag = _;
    return *this;
}

inline NumboardStreamerSettings& NumboardStreamerSettings::set_stream_finisher(::std::function<void(::std::ostream&)> _) {
    _stream_finisher = _;
    return *this;
}

//**********************************************************************
//***   NumboardStreamer                                             ***
//**********************************************************************

template<unsigned N, unsigned M>
class NumboardStreamer;

template<unsigned N, unsigned M>
std::ostream& operator<<(std::ostream&, const NumboardStreamer<N, M>&);

// ***********************************************************************

template<unsigned N, unsigned M>
class NumboardStreamer {
public:
    NumboardStreamer(const NumboardView<N, M>& numboard) : _numboard(numboard) {
    }
    friend std::ostream& operator<< <>(std::ostream&, const NumboardStreamer<N, M>&);

public:  // Setters for fine streaming settings:
    NumboardStreamer<N, M>& set_streamer_settings(NumboardStreamerSettings);
    NumboardStreamer<N, M>& set_stream_preparer(::std::function<void(::std::ostream&)>);
    NumboardStreamer<N, M>& set_stream_sustainer(::std::function<void(::std::ostream&, size_t)>);
    NumboardStreamer<N, M>& set_stream_separer(::std::function<void(::std::ostream&)>);
    NumboardStreamer<N, M>& set_stream_finisher(::std::function<void(::std::ostream&)>);
    NumboardStreamer<N, M>& set_format_independence_flag(bool = true);
private:
    NumboardStreamerSettings _streamer_settings{};
    const NumboardView<N, M>& _numboard;
};

// ***********************************************************************

template<unsigned N, unsigned M>
NumboardStreamer<N, M>& NumboardStreamer<N, M>::set_streamer_settings(NumboardStreamerSettings _) {
    _streamer_settings = _;
    return *this;
}

template<unsigned N, unsigned M>
NumboardStreamer<N, M>& NumboardStreamer<N, M>::set_stream_preparer(::std::function<void(::std::ostream&)> _) {
    _streamer_settings.set_stream_preparer(_);
    return *this;
}

template<unsigned N, unsigned M>
NumboardStreamer<N, M>& NumboardStreamer<N, M>::set_stream_sustainer(::std::function<void(::std::ostream&, size_t)> _) {
    _streamer_settings.set_stream_sustainer(_);
    return *this;
}

template<unsigned N, unsigned M>
NumboardStreamer<N, M>& NumboardStreamer<N, M>::set_stream_separer(::std::function<void(::std::ostream&)> _) {
    _streamer_settings.set_stream_separer(_);
    return *this;
}

template<unsigned N, unsigned M>
NumboardStreamer<N, M>& NumboardStreamer<N, M>::set_stream_finisher(::std::function<void(::std::ostream&)> _) {
    _streamer_settings.set_stream_finisher(_);
    return *this;
}

template<unsigned N, unsigned M>
NumboardStreamer<N, M>& NumboardStreamer<N, M>::set_format_independence_flag(bool _) {
    _streamer_settings.set_format_independence_flag(_);
    return *this;
}

// ***********************************************************************

template<unsigned N, unsigned M>
std::ostream& operator<<(std::ostream& os, const NumboardStreamer<N, M>& p){
    const extension::std::StreamFromatStacker stream_format_stacker(os, p._streamer_settings._format_independence_flag);
    for (unsigned a = 0; a < N; a++) {
        for (unsigned b = 0; b < M; b++) {
            if (b != 0) {
                p._streamer_settings._stream_separer(os);
            }
            p._streamer_settings._stream_sustainer(os, b);
            os << p._numboard(In<N>(a), In<M>(b));
        }
        os << std::endl;
    }
    return os;
}

}
