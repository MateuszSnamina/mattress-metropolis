#pragma once

#include<numboard/numboard.hpp>

//TODO remove comments in the file, Rename NeighbourhoodEnergyGetter

//**********************************************************************
//***   EnergyGetter -- abstract base clase                          ***
//**********************************************************************

namespace energy::getter {

template<unsigned N, unsigned M, unsigned NN, unsigned MM>
class EnergyGetter { // Will be EnergyGetter after the refactoring!!!
    static_assert (NN % 2 == 1);
    static_assert (MM % 2 == 1);
public:
    virtual double get_neighborhood_energy(const numboard::NumboardView<NN, MM>& neighborhood) const = 0;
    virtual double get_board_energy(const numboard::NumboardView<N, M>& board) const = 0;
    virtual ~EnergyGetter() = default;
};

//template<unsigned N, unsigned M>
//class BoardEnergyGetter {
//public:
//    virtual double get_board_energy(const numboard::NumboardView<N, M>& board) const = 0;
//    virtual ~BoardEnergyGetter() = default;
//};

}

//**********************************************************************
//***   SimplesPartsEnergyGetter                                     ***
//***   SimplesNeighbourhoodEnergyGetter                             ***
//***   SimplesBoardEnergyGetter                                     ***
//**********************************************************************

namespace energy::getter {

class ZeroNnPartsEnergyGetter {
public:
    virtual double get_horizonal_bond_energy(const numboard::NumboardView<1, 2>& part) const = 0;
    virtual double get_vertical_bond_energy(const numboard::NumboardView<2, 1>& part) const = 0;
    virtual double get_on_site_energy(const numboard::NumboardView<3, 3>& part) const {
        return 0.0;
    }
};

template<unsigned N, unsigned M>
class ZeroNnEnergyGetter : public EnergyGetter<N, M, 3, 3> {
public:
    ZeroNnEnergyGetter(const ZeroNnPartsEnergyGetter& parts_energy_getter) :
        _parts_energy_getter(parts_energy_getter) {
    }
    double get_neighborhood_energy(const numboard::NumboardView<3, 3>& neighborhood) const override {
        using numboard::In;
        double energy = 0;
        energy += _parts_energy_getter.get_horizonal_bond_energy(numboard::NumboardSubViewFactory<1, 2>::from_corner(neighborhood, In<3>(1), In<3>(0)));
        energy += _parts_energy_getter.get_horizonal_bond_energy(numboard::NumboardSubViewFactory<1, 2>::from_corner(neighborhood, In<3>(1), In<3>(1)));
        energy += _parts_energy_getter.get_vertical_bond_energy(numboard::NumboardSubViewFactory<2, 1>::from_corner(neighborhood, In<3>(0), In<3>(1)));
        energy += _parts_energy_getter.get_vertical_bond_energy(numboard::NumboardSubViewFactory<2, 1>::from_corner(neighborhood, In<3>(1), In<3>(1)));
        energy += _parts_energy_getter.get_on_site_energy(neighborhood);
        return energy;
    }
    double get_board_energy(const numboard::NumboardView<N, M>& board) const override {
        using numboard::In;
        double energy = 0;
        // upper horizonal bonds:
        for (unsigned a = 0; a < N; a++) {
            for (unsigned b = 0; b < M; b++) {
                energy += _parts_energy_getter.get_horizonal_bond_energy(numboard::NumboardSubViewFactory<1, 2>::from_corner(board, In<N>(a), In<M>(b)));
            }
        }
        // mid vertical bonds:
        for (unsigned a = 0; a < N; a++) {
            for (unsigned b = 0; b < M; b++) {
                energy += _parts_energy_getter.get_vertical_bond_energy(numboard::NumboardSubViewFactory<2, 1>::from_corner(board, In<N>(a), In<M>(b)));
            }
        }
        // on-site energy:
        for (unsigned a = 0; a < N; a++) {
            for (unsigned b = 0; b < M; b++) {
                energy += _parts_energy_getter.get_on_site_energy(numboard::NumboardSubViewFactory<3, 3>::from_center(board, In<N>(a), In<M>(b)));
            }
        }
        return energy;
    }
private:
    const ZeroNnPartsEnergyGetter& _parts_energy_getter;
};

}

//**********************************************************************
//***   FourNnPartsEnergyGetter                                      ***
//***   FourNnNeighbourhoodEnergyGetter                              ***
//***   FourNnBoardEnergyGetter                                      ***
//**********************************************************************

namespace energy::getter {

class FourNnPartsEnergyGetter {
public:
    virtual double get_horizonal_bond_energy(const numboard::NumboardView<3, 4>& part) const = 0;
    virtual double get_vertical_bond_energy(const numboard::NumboardView<4, 3>& part) const = 0;
    virtual double get_on_site_energy(const numboard::NumboardView<3, 3>& part) const {
        return 0.0;
    }
    virtual ~FourNnPartsEnergyGetter() = default;
};

/*
 * EnergyGetterFourNN implements an assumption that
 * a site orbital depends on
 * - the site spin, and
 * - the four site NN spins.
 */

template<unsigned N, unsigned M>
class FourNnEnergyGetter : public EnergyGetter<N, M, 7, 7> {
public:
    FourNnEnergyGetter(const FourNnPartsEnergyGetter& parts_energy_getter) :
        _parts_energy_getter(parts_energy_getter) {
    };
    double get_neighborhood_energy(const numboard::NumboardView<7, 7>& neighborhood) const override {
        using numboard::In;
        double energy = 0;
        // upper horizonal bonds:
        energy += _parts_energy_getter.get_horizonal_bond_energy(numboard::NumboardSubViewFactory<3, 4>::from_corner(neighborhood, In<7>(1), In<7>(1)));
        energy += _parts_energy_getter.get_horizonal_bond_energy(numboard::NumboardSubViewFactory<3, 4>::from_corner(neighborhood, In<7>(1), In<7>(2)));
        // mid horizonal bonds:
        energy += _parts_energy_getter.get_horizonal_bond_energy(numboard::NumboardSubViewFactory<3, 4>::from_corner(neighborhood, In<7>(2), In<7>(0)));
        energy += _parts_energy_getter.get_horizonal_bond_energy(numboard::NumboardSubViewFactory<3, 4>::from_corner(neighborhood, In<7>(2), In<7>(1)));
        energy += _parts_energy_getter.get_horizonal_bond_energy(numboard::NumboardSubViewFactory<3, 4>::from_corner(neighborhood, In<7>(2), In<7>(2)));
        energy += _parts_energy_getter.get_horizonal_bond_energy(numboard::NumboardSubViewFactory<3, 4>::from_corner(neighborhood, In<7>(2), In<7>(3)));
        // upper lower bonds:
        energy += _parts_energy_getter.get_horizonal_bond_energy(numboard::NumboardSubViewFactory<3, 4>::from_corner(neighborhood, In<7>(3), In<7>(1)));
        energy += _parts_energy_getter.get_horizonal_bond_energy(numboard::NumboardSubViewFactory<3, 4>::from_corner(neighborhood, In<7>(3), In<7>(2)));
        // left vertical bonds:
        energy += _parts_energy_getter.get_vertical_bond_energy(numboard::NumboardSubViewFactory<4, 3>::from_corner(neighborhood, In<7>(1), In<7>(1)));
        energy += _parts_energy_getter.get_vertical_bond_energy(numboard::NumboardSubViewFactory<4, 3>::from_corner(neighborhood, In<7>(2), In<7>(1)));
        // mid vertical bonds:
        energy += _parts_energy_getter.get_vertical_bond_energy(numboard::NumboardSubViewFactory<4, 3>::from_corner(neighborhood, In<7>(0), In<7>(2)));
        energy += _parts_energy_getter.get_vertical_bond_energy(numboard::NumboardSubViewFactory<4, 3>::from_corner(neighborhood, In<7>(1), In<7>(2)));
        energy += _parts_energy_getter.get_vertical_bond_energy(numboard::NumboardSubViewFactory<4, 3>::from_corner(neighborhood, In<7>(2), In<7>(2)));
        energy += _parts_energy_getter.get_vertical_bond_energy(numboard::NumboardSubViewFactory<4, 3>::from_corner(neighborhood, In<7>(3), In<7>(2)));
        // right vertical bonds:
        energy += _parts_energy_getter.get_vertical_bond_energy(numboard::NumboardSubViewFactory<4, 3>::from_corner(neighborhood, In<7>(1), In<7>(3)));
        energy += _parts_energy_getter.get_vertical_bond_energy(numboard::NumboardSubViewFactory<4, 3>::from_corner(neighborhood, In<7>(2), In<7>(3)));
        // on-site energy:
        energy += _parts_energy_getter.get_on_site_energy(numboard::NumboardSubViewFactory<3, 3>::from_center(neighborhood, In<7>(3), In<7>(3)));
        return energy;
    }
    double get_board_energy(const numboard::NumboardView<N, M>& board) const override {
        using numboard::In;
        double energy = 0;
        // upper horizonal bonds:
        for (unsigned a = 0; a < N; a++) {
            for (unsigned b = 0; b < M; b++) {
                energy += _parts_energy_getter.get_horizonal_bond_energy(numboard::NumboardSubViewFactory<3, 4>::from_corner(board, In<N>(a), In<M>(b)));
            }
        }
        // mid vertical bonds:
        for (unsigned a = 0; a < N; a++) {
            for (unsigned b = 0; b < M; b++) {
                energy += _parts_energy_getter.get_vertical_bond_energy(numboard::NumboardSubViewFactory<4, 3>::from_corner(board, In<N>(a), In<M>(b)));
            }
        }
        // on-site energy:
        for (unsigned a = 0; a < N; a++) {
            for (unsigned b = 0; b < M; b++) {
                energy += _parts_energy_getter.get_on_site_energy(numboard::NumboardSubViewFactory<3, 3>::from_center(board, In<N>(a), In<M>(b)));
            }
        }
        return energy;
    }
private:
    const FourNnPartsEnergyGetter& _parts_energy_getter;
};

}
