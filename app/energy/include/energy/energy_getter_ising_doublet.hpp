#pragma once

// ENERGY:
#include<energy/energy_getter_ising_common.hpp>
#include<energy/energy_getter.hpp>
// NUMBOARD:
#include<numboard/numboard.hpp>

//**********************************************************************
//***   Helpers                                                      ***
//**********************************************************************

namespace energy::ising::doublet {

double get_dublet_ising_result(
        const unsigned n_excitons_1,
        const unsigned n_excitons_2) {
    assert(n_excitons_1 < 2);
    assert(n_excitons_2 < 2);
    return (n_excitons_1 == n_excitons_2 ? -0.25 : 0.25);
}

class IsingDoubletBinaryFunctor {
public:
    double operator()(const unsigned n_excitons_1, const unsigned n_excitons_2) const {
        assert(n_excitons_1 < 2);
        assert(n_excitons_2 < 2);
        return (n_excitons_1 == n_excitons_2 ? -0.25 : 0.25);
    }
};

class IsingDoubletUnaryFunctor {
public:
    double operator()(const unsigned n_excitons) const {
        return 0;
    }
};

}

//**********************************************************************
//***   ZeroNnPartsEnergyGetter                                      ***
//**********************************************************************

namespace energy::ising::doublet {

class ZeroNnPartsEnergyGetter : public energy::ising::common::ZeroNnPartsEnergyGetterHelper<IsingDoubletBinaryFunctor, IsingDoubletUnaryFunctor> {
private:
    ZeroNnPartsEnergyGetter() :
        ZeroNnPartsEnergyGetterHelper(IsingDoubletBinaryFunctor(), IsingDoubletUnaryFunctor()) {
    }
public:
    static std::unique_ptr<ZeroNnPartsEnergyGetter> make() {
        return std::unique_ptr<ZeroNnPartsEnergyGetter>(new ZeroNnPartsEnergyGetter());
    }
};

}

//**********************************************************************
//***   FourNnPartsEnergyGetter                                      ***
//**********************************************************************

namespace energy::ising::doublet {

class FourNnPartsEnergyGetter : public energy::ising::common::FourNnPartsEnergyGetterHelper<IsingDoubletBinaryFunctor, IsingDoubletUnaryFunctor> {
private:
    FourNnPartsEnergyGetter() :
        FourNnPartsEnergyGetterHelper(IsingDoubletBinaryFunctor(), IsingDoubletUnaryFunctor()) {
    }
public:
    static std::unique_ptr<FourNnPartsEnergyGetter> make() {
        return std::unique_ptr<FourNnPartsEnergyGetter>(new FourNnPartsEnergyGetter());
    }
};

}
