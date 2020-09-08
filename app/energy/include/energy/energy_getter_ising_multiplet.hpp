#pragma once

// ENERGY:
#include<energy/energy_getter_ising_common.hpp>
#include<energy/energy_getter.hpp>
// NUMBOARD:
#include<numboard/numboard.hpp>

//**********************************************************************
//***   Helpers                                                      ***
//**********************************************************************

namespace energy::ising::multiplet {

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

class IsingMultipletBinaryFunctor {
public:
    IsingMultipletBinaryFunctor(const double multiplicity) :
        _multiplicity(multiplicity) {
        assert(multiplicity > 0);
    }
    double operator()(const unsigned n_excitons_1, const unsigned n_excitons_2) const {
        const unsigned doubled_spin_max = _multiplicity - 1;
        const int doubled_spin_1 = (int)doubled_spin_max - 2 * (int)n_excitons_1;
        const int doubled_spin_2 = (int)doubled_spin_max - 2 * (int)n_excitons_2;
        return - (double)(doubled_spin_1  * doubled_spin_2) / 4.0;
    }
private:
    const double _multiplicity;
};

class IsingMultipletUnaryFunctor {
public:
    IsingMultipletUnaryFunctor(const double multiplicity) :
        _multiplicity(multiplicity) {
        assert(multiplicity > 0);
    }
    double operator()(const unsigned n_excitons) const {
        return 0;
    }
private:
    const double _multiplicity;
};

}

//**********************************************************************
//***   ZeroNnPartsEnergyGetter                                      ***
//**********************************************************************

namespace energy::ising::multiplet {

class ZeroNnPartsEnergyGetter : public energy::ising::common::ZeroNnPartsEnergyGetterHelper<IsingMultipletBinaryFunctor, IsingMultipletUnaryFunctor> {
private:
    explicit ZeroNnPartsEnergyGetter(unsigned multiplicity) :
        ZeroNnPartsEnergyGetterHelper(IsingMultipletBinaryFunctor(multiplicity), IsingMultipletUnaryFunctor(multiplicity)) {
        assert(multiplicity > 0);
    }
public:
    static std::unique_ptr<ZeroNnPartsEnergyGetter> make(unsigned multiplicity) {
        return std::unique_ptr<ZeroNnPartsEnergyGetter>(new ZeroNnPartsEnergyGetter(multiplicity));
    }
};

}

//**********************************************************************
//***   FourNnPartsEnergyGetter                                      ***
//**********************************************************************

namespace energy::ising::multiplet {

class FourNnPartsEnergyGetter : public energy::ising::common::FourNnPartsEnergyGetterHelper<IsingMultipletBinaryFunctor, IsingMultipletUnaryFunctor> {
private:
    explicit FourNnPartsEnergyGetter(unsigned multiplicity) :
        FourNnPartsEnergyGetterHelper(IsingMultipletBinaryFunctor(multiplicity), IsingMultipletUnaryFunctor(multiplicity)) {
        assert(multiplicity > 0);
    }
public:
    static std::unique_ptr<FourNnPartsEnergyGetter> make(unsigned multiplicity) {
        return std::unique_ptr<FourNnPartsEnergyGetter>(new FourNnPartsEnergyGetter(multiplicity));
    }
};

}
