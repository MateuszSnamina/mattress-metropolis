// TESTEE:
#include <numboard/numboard.hpp>

namespace numboard_tests {

template<unsigned N, unsigned M>
numboard::Numboard<N, M> prepare_board(unsigned const_value = 200) {
    using numboard::In;
    numboard::Numboard<N, M> board;
    for (unsigned i = 0; i < N; i++) {
        for (unsigned j = 0; j < M; j++) {
            board.set(In<N>(i), In<M>(j), const_value + 10 * i + j);
        }
    }
    return board;
}

}
