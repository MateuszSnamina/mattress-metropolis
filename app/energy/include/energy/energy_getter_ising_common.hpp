#pragma once

// ENERGY:
#include<energy/energy_getter.hpp>
// NUMBOARD:
#include<numboard/numboard.hpp>

//**********************************************************************
//***   ZeroNnPartsEnergyGetter                                      ***
//**********************************************************************

namespace energy::ising::common {

template<class BinaryFunctor, class UnaryFunctor>
class ZeroNnPartsEnergyGetterHelper : public energy::getter::ZeroNnPartsEnergyGetter {
protected:
    ZeroNnPartsEnergyGetterHelper(BinaryFunctor f, UnaryFunctor u) :
        _f(f), _u(u) {
    }
public:
    static std::unique_ptr<ZeroNnPartsEnergyGetterHelper> make(BinaryFunctor f, UnaryFunctor u) {
        return std::unique_ptr<ZeroNnPartsEnergyGetterHelper>(new ZeroNnPartsEnergyGetterHelper(f, u));
    }
    double get_horizonal_bond_energy(const numboard::NumboardView<1, 2>& part) const {
        using numboard::In;
        const unsigned n_excitons_1 = part(In<1>(0), In<2>(0));
        const unsigned n_excitons_2 = part(In<1>(0), In<2>(1));
        return _f(n_excitons_1, n_excitons_2);
    }
    double get_vertical_bond_energy(const numboard::NumboardView<2, 1>& part) const {
        using numboard::In;
        const unsigned n_excitons_1 = part(In<2>(0), In<1>(0));
        const unsigned n_excitons_2 = part(In<2>(1), In<1>(0));
        return _f(n_excitons_1, n_excitons_2);
    }
private:
    const BinaryFunctor _f;
    const UnaryFunctor _u;
};

}

//**********************************************************************
//***   FourNnPartsEnergyGetter                                      ***
//**********************************************************************

namespace energy::ising::common {

template<class BinaryFunctor, class UnaryFunctor>
class FourNnPartsEnergyGetterHelper : public energy::getter::FourNnPartsEnergyGetter {
protected:
    FourNnPartsEnergyGetterHelper(BinaryFunctor f, UnaryFunctor u) :
        _f(f), _u(u) {
    }
public:
    static std::unique_ptr<FourNnPartsEnergyGetterHelper> make(BinaryFunctor f, UnaryFunctor u) {
        return std::unique_ptr<FourNnPartsEnergyGetterHelper>(new FourNnPartsEnergyGetterHelper(f, u));
    }
    double get_horizonal_bond_energy(const numboard::NumboardView<3, 4>& part) const override {
        using numboard::In;
        const unsigned n_excitons_1 = part(In<3>(1), In<4>(1));
        const unsigned n_excitons_2 = part(In<3>(1), In<4>(2));
        return _f(n_excitons_1, n_excitons_2);
    }
    double get_vertical_bond_energy(const numboard::NumboardView<4, 3>& part) const override {
        using numboard::In;
        const unsigned n_excitons_1 = part(In<4>(1), In<3>(1));
        const unsigned n_excitons_2 = part(In<4>(2), In<3>(1));
        return _f(n_excitons_1, n_excitons_2);
    }
private:
    const BinaryFunctor _f;
    const UnaryFunctor _u;
};

}
