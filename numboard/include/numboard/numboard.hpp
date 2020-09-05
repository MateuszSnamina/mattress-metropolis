#ifndef NDIM_NDIM_HPP
#define NDIM_NDIM_HPP

// STL:
#include<array>
// STD:
#include<cassert>
#include<iostream>
#include<iomanip>

//**********************************************************************
//***   Idx                                                          ***
//**********************************************************************

namespace numboard {

template<unsigned S>
class Idx {
public:
    virtual unsigned get() const = 0;
    virtual ~Idx() = default;
    operator unsigned() const {
        return get();
    }
};

}

//**********************************************************************
//***   In                                                           ***
//**********************************************************************

namespace numboard {

template<unsigned S>
class In : public Idx<S> {
public:
    explicit In(unsigned idx) : _idx(idx) {
        assert(idx < S);
    };
    unsigned get() const override {
        return _idx;
    }
private:
    const unsigned _idx;
};

}

//**********************************************************************
//***   Adj                                                          ***
//**********************************************************************

namespace numboard {

template<unsigned S>
class Adj : public Idx<S> {
public:
    explicit Adj(int idx) : _idx(Adj<S>::reduce(idx)) {
        assert(-(int)S <= idx);
        assert(idx < (int)(2 * S));
    }
    unsigned get() const override {
        return _idx;
    }
    static unsigned reduce(int idx) {
        if (idx >= (int)S) {
            idx -= S;
        }
        if (idx < 0) {
            idx += S;
        }
        assert(idx >= 0);
        return (unsigned)idx;
    }
private:
    const unsigned _idx;
};

}

//**********************************************************************
//***   Out                                                          ***
//**********************************************************************

namespace numboard {

template<unsigned S>
class Out : public Idx<S> {
public:
    explicit Out(int idx) : _idx(Out<S>::reduce(idx)) {
    }
    unsigned get() const override {
        return _idx;
    }
    static unsigned reduce(int idx) {
        while (idx >= (int)S) {
            idx -= S;
        }
        while (idx < 0) {
            idx += S;
        }
        assert(idx >= 0);
        return (unsigned)idx;
    }
private:
    const unsigned _idx;
};

}

//**********************************************************************
//***   NumboardView                                                 ***
//**********************************************************************

namespace numboard {

template<unsigned N, unsigned M>
class NumboardView {
public:
    virtual unsigned operator()(const Idx<N>&, const Idx<M>&) const = 0;
    virtual ~NumboardView() = default;
    unsigned get_n() const {
        return N;
    };
    unsigned get_m() const {
        return M;
    };
};

}

//**********************************************************************
//***   NumboardSubView and NumboardSubViewFactory                   ***
//**********************************************************************

namespace numboard {

template<unsigned NN, unsigned MM>
class NumboardSubViewFactory;

template<unsigned N, unsigned M, unsigned NN, unsigned MM>
class NumboardSubView : public NumboardView<NN, MM> {
    static_assert (NN <= N);
    static_assert (MM <= M);
public:
    virtual unsigned operator()(const Idx<NN>& x, const Idx<MM>& y) const override {
        return _numboard(Adj<N>(_x + (unsigned)x), Adj<M>(_y + (unsigned)y));
    }
    friend class NumboardSubViewFactory<NN, MM>;
private:
    const NumboardView<N, M>& _numboard;
    const unsigned _x, _y;
    NumboardSubView(const NumboardView<N, M>& numboard, const Idx<N>& x, const Idx<M>& y) :
        _numboard(numboard), _x(x), _y(y) {
    }
};

template<unsigned NN, unsigned MM>
class NumboardSubViewFactory {
public:
    template<unsigned N, unsigned M>
    static NumboardSubView<N, M, NN, MM> from_corner(const NumboardView<N, M>& numboard, const Idx<N>& x, const Idx<M>& y) {
        static_assert (NN <= N);
        static_assert (MM <= M);
        return NumboardSubView<N, M, NN, MM>(numboard, x, y);
    }

    template<unsigned N, unsigned M>
    static NumboardSubView<N, M, NN, MM> from_center(const NumboardView<N, M>& numboard, const Idx<N>& x, const Idx<M>& y) {
        static_assert (NN <= N);
        static_assert (MM <= M);
        static_assert (NN % 2 == 1);
        static_assert (MM % 2 == 1);
        return NumboardSubView<N, M, NN, MM>(numboard, Adj<N>((int)(unsigned)x - NN / 2), Adj<M>((int)(unsigned)y - MM / 2));
    }
};

}

//**********************************************************************
//***   Numboard                                                     ***
//**********************************************************************

namespace numboard {

template<unsigned N, unsigned M>
class Numboard : public NumboardView<N, M>{
public:
    unsigned operator()(const Idx<N>& x, const Idx<M>& y) const override {
        const unsigned idx = x * M + y;
        return _buffer[idx];
    };

    void set(const Idx<N>& x, const Idx<M>& y, unsigned val) {
        const unsigned idx = x * M + y;
        _buffer[idx] = val;
    };

private:
    std::array<int, N * M> _buffer;
};

}

//**********************************************************************
//***   NumboardPrinter                                              ***
//**********************************************************************

namespace numboard {

template<unsigned N, unsigned M>
class NumboardPrinter;

template<unsigned N, unsigned M>
std::ostream& operator<<(std::ostream&, const NumboardPrinter<N, M>&);

template<unsigned N, unsigned M>
class NumboardPrinter {
public:
    NumboardPrinter(const NumboardView<N, M>& numboard) : _numboard(numboard) {
    }
    friend std::ostream& operator<< <>(std::ostream&, const NumboardPrinter<N, M>&);
private:
    const NumboardView<N, M>& _numboard;
};

template<unsigned N, unsigned M>
std::ostream& operator<<(std::ostream& s, const NumboardPrinter<N, M>& p){
    for (unsigned a = 0; a < N; a++) {
        for (unsigned b = 0; b < M; b++) {
            s << std::setw(2) << p._numboard(In<N>(a), In<M>(b)) << " ";
        }
        s << std::endl;
    }
    return s;
}

}

#endif
