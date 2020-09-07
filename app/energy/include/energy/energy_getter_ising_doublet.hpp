#pragma once

// ENERGY:
#include<energy/energy_getter.hpp>
// NUMBOARD:
#include<numboard/numboard.hpp>

//**********************************************************************
//***   Helpers                                                      ***
//**********************************************************************

namespace  {

double get_dublet_ising_result(
        const unsigned n_excitons_1,
        const unsigned n_excitons_2) {
    assert(n_excitons_1 < 2);
    assert(n_excitons_2 < 2);
    return (n_excitons_1 == n_excitons_2 ? -0.25 : 0.25);
}

}

//**********************************************************************
//***   SimplesIsingEnergyGetter                                     ***
//**********************************************************************

namespace energy::ising::doublet {

class SimplesPartsEnergyGetter : public energy::getter::SimplesPartsEnergyGetter {
public:
    double get_horizonal_bond_energy(const numboard::NumboardView<1, 2>& part) const {
        using numboard::In;
        const unsigned n_excitons_1 = part(In<1>(0), In<2>(0));
        const unsigned n_excitons_2 = part(In<1>(0), In<2>(1));
        return get_dublet_ising_result(n_excitons_1, n_excitons_2);
    }
    double get_vertical_bond_energy(const numboard::NumboardView<2, 1>& part) const {
        using numboard::In;
        const unsigned n_excitons_1 = part(In<2>(0), In<1>(0));
        const unsigned n_excitons_2 = part(In<2>(1), In<1>(0));
        return get_dublet_ising_result(n_excitons_1, n_excitons_2);
    }
};

}

//**********************************************************************
//***   IsingPartsEnergyGetterFourNN                                 ***
//**********************************************************************

namespace energy::ising::doublet {

class FourNnPartsEnergyGetter : public energy::getter::FourNnPartsEnergyGetter {
public:
    double get_horizonal_bond_energy(const numboard::NumboardView<3, 4>& part) const override {
        using numboard::In;
        const unsigned n_excitons_1 = part(In<3>(1), In<4>(1));
        const unsigned n_excitons_2 = part(In<3>(1), In<4>(2));
        return get_dublet_ising_result(n_excitons_1, n_excitons_2);
    }
    double get_vertical_bond_energy(const numboard::NumboardView<4, 3>& part) const override {
        using numboard::In;
        const unsigned n_excitons_1 = part(In<4>(1), In<3>(1));
        const unsigned n_excitons_2 = part(In<4>(2), In<3>(1));
        return get_dublet_ising_result(n_excitons_1, n_excitons_2);
    }

};

}
