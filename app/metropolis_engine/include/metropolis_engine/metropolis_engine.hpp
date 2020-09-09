#pragma once

// ENERGY:
#include<energy/energy_getter.hpp>
// NUMBOARD:
#include<numboard/numboard.hpp>
// STD:
#include<cmath>
#include<random>

namespace {

template<unsigned N, unsigned M>
double calculate_board_magnetization(const numboard::NumboardView<N, M>& numboard){
    unsigned board_stars = 0;
    using numboard::In;
    for (unsigned i = 0; i < N; i++) {
        for (unsigned j = 0; j < M; j++) {
            const unsigned site_stars = numboard(In<N>(i), In<M>(j));
            assert(site_stars <= 1);
            board_stars += site_stars;
        }
    }
    const double magnetization_max = 0.5 * (N * M);
    const double magnetization = magnetization_max - board_stars;
    return magnetization;
}

}

//**********************************************************************
//***   Forward declatarions                                         ***
//**********************************************************************

namespace metropolis_engine {

template<unsigned N, unsigned M, unsigned NN, unsigned MM>
class MetropolisEngine;

}

//**********************************************************************
//***   MetropolisStepReceipt                                         ***
//**********************************************************************

namespace metropolis_engine {

struct MetropolisStepReceipt {
    bool is_accepted;
    std::pair<unsigned, unsigned> position;
    unsigned original_value;
    unsigned new_value;
};

}

//**********************************************************************
//***   MetropolisEngineStatisticalAccumulator                       ***
//**********************************************************************

namespace metropolis_engine {

class MetropolisEngineStatisticalAccumulator {
public:
    double get_steps() const {
        return _steps;
    }
    double get_average_energy() const {
        return _energy / _steps;
    }
    double get_average_sq_energy() const {
        return _sq_energy / _steps;
    }
    double get_average_sigma_energy() const {
        return get_average_sq_energy() - get_average_energy() * get_average_energy();
    }
    double get_specific_heat(double beta) const {
        return get_average_sigma_energy() * beta * beta;
    }
    double get_average_magnetization() const {
        return _magnetization / _steps;
    }
    double get_average_abs_magnetization() const {
        return _abs_magnetization / _steps;
    }
    template<unsigned N, unsigned M, unsigned NN, unsigned MM>
    friend class MetropolisEngine;
private:
    unsigned _steps = 0;
    double _energy = 0.0;
    double _sq_energy = 0.0;
    double _magnetization = 0.0;
    double _abs_magnetization = 0.0;
private:
    void put_energy_and_magnetization(double energy, double magnetization){
        _steps += 1;
        _energy += energy;
        _sq_energy += energy * energy;
        _magnetization += magnetization;
        _abs_magnetization += std::abs(magnetization);
    }
};

}

//**********************************************************************
//***   MetropolisEngineCache                                        ***
//**********************************************************************

namespace metropolis_engine {

class MetropolisEngineCache {
public:
    double get_energy() const {
        return _energy;
    }
    double get_magnetization() const {
        return _magnetization;
    }
    template<unsigned N, unsigned M, unsigned NN, unsigned MM>
    friend class MetropolisEngine;
private:
    void put_energy_and_magnetization(double energy, double magnetization){
        _energy = energy;
        _magnetization = magnetization;
    }
private:
    double _energy = 0.0;
    double _magnetization = 0.0;
};

}

//**********************************************************************
//***   MetropolisEngine                                             ***
//**********************************************************************

namespace metropolis_engine {

template<unsigned N, unsigned M, unsigned NN, unsigned MM>
class MetropolisEngine {
public:
    MetropolisEngine(
            const double beta,
            const energy::getter::EnergyGetter<N, M, NN, MM>& energy_getterer);
    void init_random();
    void init_uniform(unsigned value = 0);
    MetropolisStepReceipt do_step();
    void accumulate();
    MetropolisEngineStatisticalAccumulator get_accumulator() const;
private:
    const double _beta;
    const energy::getter::EnergyGetter<N, M, NN, MM>& _energy_getter;
    numboard::Numboard<N, M> _numboard{};
    MetropolisEngineStatisticalAccumulator _accumulator{};
    MetropolisEngineCache _cache{};
private:
    void init_cache();
    unsigned draw_value();
    std::pair<unsigned, unsigned> draw_positon();
    bool draw_should_accept(const double delta_energy);
    std::uniform_int_distribution<unsigned> _di_value{0, 1};
    std::uniform_int_distribution<unsigned> _di_position{0, N * M - 1};
    std::uniform_real_distribution<double> _dr{0.0, 1.0};
    std::default_random_engine _dre;
};

template<unsigned N, unsigned M, unsigned NN, unsigned MM>
MetropolisEngine<N, M, NN, MM>::MetropolisEngine(
        const double beta,
        const energy::getter::EnergyGetter<N, M, NN, MM>& energy_getterer) :
    _beta(beta),
    _energy_getter(energy_getterer) {
}

template<unsigned N, unsigned M, unsigned NN, unsigned MM>
MetropolisStepReceipt MetropolisEngine<N, M, NN, MM>::do_step() {
    using numboard::In;
    //std::cout << "----------------------------" << std::endl;
    // Proposed position to change:
    const std::pair<unsigned, unsigned> position = draw_positon();
    const unsigned& a = position.first;
    const unsigned& b = position.second;
    //std::cout << "a, b: " << a << ", " << b << std::endl;
    // Original and proposed value:
    const unsigned original_value = _numboard(In<N>(a), In<M>(b));
    assert(original_value == 0 || original_value == 1);
    // //const unsigned proposed_value = draw_proposed_value();
    const unsigned proposed_value = ( original_value ? 0 : 1 );
    //std::cout << "original_value, proposed_value: " << original_value << ", " << proposed_value << std::endl;
    // Calculate delta_energy:
    const auto& original_neighbourhood = numboard::NumboardSubViewFactory<NN, MM>::from_center(_numboard, In<N>(a), In<M>(b));
    const auto& proposed_neighbourhood = numboard::NumboardAlterViewFactory<NN, MM>::center(original_neighbourhood, proposed_value);
    double original_neighbourhood_energy = _energy_getter.get_neighborhood_energy(original_neighbourhood);
    double proposed_neighbourhood_energy =  _energy_getter.get_neighborhood_energy(proposed_neighbourhood);
    double delta_energy = proposed_neighbourhood_energy - original_neighbourhood_energy;
    //std::cout << "delta_energy:" << delta_energy << std::endl;
    // Calculate delta_magnetisation:
    double delta_magnetisation = (original_value ? +1.0 : -1.0);
    // Accept the proposed change or not:
    const bool is_accepted = draw_should_accept(delta_energy);
    //std::cout << "is_accepted:" << is_accepted << std::endl;
    const unsigned new_value = (is_accepted? original_value : proposed_value);
    // Board energy and magnetisation:
    const double original_board_energy = _cache.get_energy();
    const double original_board_magnetization = _cache.get_magnetization();
    const double new_board_energy = (is_accepted ? original_board_energy + delta_energy : original_board_energy);
    const double new_board_magnetization = (is_accepted ? original_board_magnetization + delta_magnetisation : original_board_magnetization);
    // Save results:
    if (is_accepted) {
        _cache.put_energy_and_magnetization(new_board_energy, new_board_magnetization);
        _numboard.set(In<N>(a), In<M>(b), proposed_value);
    }
    // Debug part:
#ifndef NDEBUG
    {
        const double board_energy_recalculated = _energy_getter.get_board_energy(_numboard); //TODO remove debug
        //std::cout << "new_board_energy:          " << new_board_energy << std::endl; //TODO remove debug
        //std::cout << "board_energy_recalculated: " << board_energy_recalculated << std::endl; //TODO remove debug
        assert(board_energy_recalculated - new_board_energy < 1e-10);
        const double board_magnetization_recalculated = calculate_board_magnetization(_numboard); //TODO remove debug
        //std::cout << "board_magnetization_recalculated: " << board_magnetization_recalculated << std::endl; //TODO remove debug
        //std::cout << "new_board_magnetization:          " << new_board_magnetization << std::endl; //TODO remove debug
        assert(board_magnetization_recalculated - new_board_magnetization < 1e-10);
    }
#endif
    // return:
    return {is_accepted, position, original_value, new_value};
}

template<unsigned N, unsigned M, unsigned NN, unsigned MM>
void MetropolisEngine<N, M, NN, MM>::accumulate() {
    const double current_energy = _cache.get_energy();
    const double current_magnetization = _cache.get_magnetization();
    _accumulator.put_energy_and_magnetization(current_energy, current_magnetization);
}

template<unsigned N, unsigned M, unsigned NN, unsigned MM>
void MetropolisEngine<N, M, NN, MM>::init_uniform(unsigned value){
    using numboard::In;
    for (unsigned i = 0; i < N; i++) {
        for (unsigned j = 0; j < M; j++) {
            _numboard.set(In<N>(i), In<M>(j), value);
        }
    }
    init_cache();
}

template<unsigned N, unsigned M, unsigned NN, unsigned MM>
void MetropolisEngine<N, M, NN, MM>::init_random() {
    using numboard::In;
    for (unsigned i = 0; i < N; i++) {
        for (unsigned j = 0; j < M; j++) {
            _numboard.set(In<N>(i), In<M>(j), draw_value());
        }
    }
    init_cache();
}

template<unsigned N, unsigned M, unsigned NN, unsigned MM>
void MetropolisEngine<N, M, NN, MM>::init_cache() {
    const double board_energy = _energy_getter.get_board_energy(_numboard);
    const double board_magnetization = calculate_board_magnetization(_numboard);
    //std::cout << "init board_energy:" << board_energy << std::endl;
    //std::cout << "init board_magnetization:" << board_magnetization << std::endl;
    _cache.put_energy_and_magnetization(board_energy, board_magnetization);
}


template<unsigned N, unsigned M, unsigned NN, unsigned MM>
std::pair<unsigned, unsigned> MetropolisEngine<N, M, NN, MM>::draw_positon() {
    const unsigned site_to_change = _di_position(_dre);
    const unsigned a = site_to_change / M;
    const unsigned b = site_to_change % M;
    // TOFO std::divmod(...)
    return {a, b};
}

template<unsigned N, unsigned M, unsigned NN, unsigned MM>
unsigned MetropolisEngine<N, M, NN, MM>::draw_value() {
    const unsigned value = _di_value(_dre);
    return value;
}

template<unsigned N, unsigned M, unsigned NN, unsigned MM>
bool MetropolisEngine<N, M, NN, MM>::draw_should_accept(const double delta_energy) {
    if (delta_energy <= 0) {
        return true;
    } else {
        const double accept_probability = std::exp(- _beta * delta_energy);
        // std::cout << "accept_probability:" << accept_probability << std::endl;
        if (_dr(_dre) < accept_probability) {
            return true;
        } else {
            return false;
        }
    }
}

template<unsigned N, unsigned M, unsigned NN, unsigned MM>
MetropolisEngineStatisticalAccumulator MetropolisEngine<N, M, NN, MM>::get_accumulator() const {
    return _accumulator;
}

}
