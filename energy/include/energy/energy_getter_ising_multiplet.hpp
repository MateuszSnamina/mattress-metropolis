#pragma once

// ENERGY:
#include<energy/energy_getter.hpp>
// NUMBOARD:
#include<numboard/numboard.hpp>

//**********************************************************************
//***   Helpers                                                      ***
//**********************************************************************

namespace  {

double get_multiplet_ising_result(
        const unsigned multiplicity,
        const unsigned n_excitons_1,
        const unsigned n_excitons_2) {
    assert(multiplicity > 0);
    const unsigned doubled_spin_max = multiplicity - 1;
    const int doubled_spin_1 = (int)doubled_spin_max - 2 * (int)n_excitons_1;
    const int doubled_spin_2 = (int)doubled_spin_max - 2 * (int)n_excitons_2;
    return - (double)(doubled_spin_1  * doubled_spin_2) / 4.0;
}

}

//**********************************************************************
//***   SimplesIsingEnergyGetter                                     ***
//**********************************************************************

namespace energy::ising::multiplet {

class SimplesPartsEnergyGetter : public energy::getter::SimplesPartsEnergyGetter {
public:
    SimplesPartsEnergyGetter(unsigned multiplicity) : _multiplicity(multiplicity) {
        assert(multiplicity > 0);
    }
    double get_horizonal_bond_energy(const numboard::NumboardView<1, 2>& part) const {
        using numboard::In;
        const unsigned n_excitons_1 = part(In<1>(0), In<2>(0));
        const unsigned n_excitons_2 = part(In<1>(0), In<2>(1));
        return get_multiplet_ising_result(_multiplicity, n_excitons_1, n_excitons_2);
    }
    double get_vertical_bond_energy(const numboard::NumboardView<2, 1>& part) const {
        using numboard::In;
        const unsigned n_excitons_1 = part(In<2>(0), In<1>(0));
        const unsigned n_excitons_2 = part(In<2>(1), In<1>(0));
        return get_multiplet_ising_result(_multiplicity, n_excitons_1, n_excitons_2);
    }
private:
    const unsigned _multiplicity;
};

}

//**********************************************************************
//***   IsingPartsEnergyGetterFourNN                                 ***
//**********************************************************************

namespace energy::ising::multiplet {

class FourNnPartsEnergyGetter : public energy::getter::FourNnPartsEnergyGetter {
public:
    FourNnPartsEnergyGetter(unsigned multiplicity) : _multiplicity(multiplicity) {
        assert(multiplicity > 0);
    }
    double get_horizonal_bond_energy(const numboard::NumboardView<3, 4>& part) const override {
        using numboard::In;
        const unsigned n_excitons_1 = part(In<3>(1), In<4>(1));
        const unsigned n_excitons_2 = part(In<3>(1), In<4>(2));
        return get_multiplet_ising_result(_multiplicity, n_excitons_1, n_excitons_2);
    }
    double get_vertical_bond_energy(const numboard::NumboardView<4, 3>& part) const override {
        using numboard::In;
        const unsigned n_excitons_1 = part(In<4>(1), In<3>(1));
        const unsigned n_excitons_2 = part(In<4>(2), In<3>(1));
        return get_multiplet_ising_result(_multiplicity, n_excitons_1, n_excitons_2);
    }
private:
    const unsigned _multiplicity;
};

}


