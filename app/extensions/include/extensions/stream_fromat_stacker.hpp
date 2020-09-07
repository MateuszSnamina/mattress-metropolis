#ifndef EXTENSION_STREAMER_FORMNAT_STACKER_HPP
#define EXTENSION_STREAMER_FORMNAT_STACKER_HPP

#include <iostream>

// #######################################################################
// ##  StreamFromatStacker                                              ##
// #######################################################################

namespace extension::std {

class StreamFromatStacker {
   public:
    StreamFromatStacker(::std::ostream& stream, bool is_working = true);
    ~StreamFromatStacker();
    StreamFromatStacker(const StreamFromatStacker& rhs) = delete;
    StreamFromatStacker& operator=(const StreamFromatStacker& rhs) = delete;

   private:
    ::std::ostream& _stream;
    const bool _is_working;
    const ::std::ios_base::fmtflags _fmtfl;
    const ::std::streamsize _wide;
    const ::std::streamsize _prec;
    const char _fillch;
};

inline StreamFromatStacker::StreamFromatStacker(::std::ostream& stream, bool is_working)
    : _stream(stream),
      _is_working(is_working),
      _fmtfl(_stream.flags()),
      _wide(stream.width()),
      _prec(stream.precision()),
      _fillch(stream.fill()) {
    if (_is_working) {
        _stream.flags(::std::ios_base::fmtflags());
        _stream.width(0);
        _stream.precision(6);
        _stream.fill(' ');
    }
}

inline StreamFromatStacker::~StreamFromatStacker() {
    if (_is_working) {
        _stream.flags(_fmtfl);
        _stream.width(_wide);
        _stream.precision(_prec);
        _stream.fill(_fillch);
    }
}

}  // namespace extension::std

#endif