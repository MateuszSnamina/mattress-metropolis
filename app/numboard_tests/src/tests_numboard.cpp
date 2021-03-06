// GTEST:
#include <gtest/gtest.h>
// SELF:
#include <numboard_tests/prepare_board.hpp>
// TESTEE:
#include <numboard/numboard.hpp>
#include <numboard/numboard_streamer.hpp>
// STD:
#include <iostream>
#include <sstream>
#include <string>

namespace  {

template<unsigned N, unsigned M>
std::string repr(const numboard::NumboardView<N, M>& board) {
    std::ostringstream oss;
    oss << numboard::NumboardStreamer(board);
    return oss.str();
}

}

TEST(Numboard, Print) {
    const auto& board = numboard_tests::prepare_board<5, 8>();
    const std::string got_str = repr(board);
    const std::string exprected_str =
            "200 201 202 203 204 205 206 207\n"
            "210 211 212 213 214 215 216 217\n"
            "220 221 222 223 224 225 226 227\n"
            "230 231 232 233 234 235 236 237\n"
            "240 241 242 243 244 245 246 247\n";
    ASSERT_EQ(got_str, exprected_str);
}

TEST(Numboard, Getters) {
    using numboard::In;
    using numboard::Adj;
    using numboard::Out;
    const auto& board = numboard_tests::prepare_board<5, 8>();
    // inside
    ASSERT_EQ(246, board(In<5>(4), In<8>(6)));
    ASSERT_EQ(246, board(In<5>(4), Adj<8>(6)));
    ASSERT_EQ(246, board(In<5>(4), Adj<8>(6 + 8)));
    ASSERT_EQ(246, board(In<5>(4), Adj<8>(6 - 8)));
    ASSERT_EQ(246, board(In<5>(4), Out<8>(6)) );
    ASSERT_EQ(246, board(In<5>(4), Out<8>(6 + 8)));
    ASSERT_EQ(246, board(In<5>(4), Out<8>(6 - 8)));
    ASSERT_EQ(246, board(In<5>(4), Out<8>(6 + 2 * 8)));
    ASSERT_EQ(246, board(In<5>(4), Out<8>(6 - 2 * 8)));
    // corners:
    ASSERT_EQ(200, board(In<5>(0), In<8>(0)));
    ASSERT_EQ(207, board(In<5>(0), In<8>(7)));
    ASSERT_EQ(240, board(In<5>(4), In<8>(0)));
    ASSERT_EQ(247, board(In<5>(4), In<8>(7)));
    ASSERT_EQ(200, board(Adj<5>(0), Adj<8>(0)));
    ASSERT_EQ(207, board(Adj<5>(0), Adj<8>(7)));
    ASSERT_EQ(240, board(Adj<5>(4), Adj<8>(0)));
    ASSERT_EQ(247, board(Adj<5>(4), Adj<8>(7)));
    ASSERT_EQ(200, board(Adj<5>(0 - 5), Adj<8>(0 - 8)));
    ASSERT_EQ(207, board(Adj<5>(0 - 5), Adj<8>(7 + 8)));
    ASSERT_EQ(240, board(Adj<5>(4 + 5), Adj<8>(0 - 8)));
    ASSERT_EQ(247, board(Adj<5>(4 + 5), Adj<8>(7 + 8)));
    ASSERT_EQ(200, board(Out<5>(0 - 2 * 5), Out<8>(0 - 2 * 8)));
    ASSERT_EQ(207, board(Out<5>(0 - 2 * 5), Out<8>(7 + 2 * 8)));
    ASSERT_EQ(240, board(Out<5>(4 + 2 * 5), Out<8>(0 - 2 * 8)));
    ASSERT_EQ(247, board(Out<5>(4 + 2 * 5), Out<8>(7 + 2 * 8)));
}

TEST(Numboard, SubviewCorner) {
    using numboard::In;
    const auto& board = numboard_tests::prepare_board<5, 8>();
    {
        const auto& subboard1 = numboard::NumboardSubViewFactory<3,4>::from_corner(board, In<5>(1), In<8>(3));
        const std::string got_str1 = repr(subboard1);
        const std::string exprected_str1 =
                "213 214 215 216\n"
                "223 224 225 226\n"
                "233 234 235 236\n";
        ASSERT_EQ(got_str1, exprected_str1);
    }
    {
        const auto& subboard2 = numboard::NumboardSubViewFactory<3,4>::from_corner(board, In<5>(1), In<8>(6));
        const std::string got_str2 = repr(subboard2);
        const std::string exprected_str2 =
                "216 217 210 211\n"
                "226 227 220 221\n"
                "236 237 230 231\n";
        ASSERT_EQ(got_str2, exprected_str2);
    }
    {
        const auto& subboard3 = numboard::NumboardSubViewFactory<3,4>::from_corner(board, In<5>(3), In<8>(6));
        const std::string got_str3 = repr(subboard3);
        const std::string exprected_str3 =
                "236 237 230 231\n"
                "246 247 240 241\n"
                "206 207 200 201\n";
        ASSERT_EQ(got_str3, exprected_str3);

    }
}

TEST(Numboard, SubviewCenter) {
    using numboard::In;
    const auto& board = numboard_tests::prepare_board<5, 8>();
    {
        const auto& subboard1 = numboard::NumboardSubViewFactory<3,5>::from_center(board, In<5>(1), In<8>(3));
        const std::string got_str1 = repr(subboard1);
        const std::string exprected_str1 =
                "201 202 203 204 205\n"
                "211 212 213 214 215\n"
                "221 222 223 224 225\n";
        ASSERT_EQ(got_str1, exprected_str1);
    }
    {
        const auto& subboard2 = numboard::NumboardSubViewFactory<3,5>::from_center(board, In<5>(4), In<8>(6));
        const std::string got_str2 = repr(subboard2);
        const std::string exprected_str2 =
                "234 235 236 237 230\n"
                "244 245 246 247 240\n"
                "204 205 206 207 200\n";
        ASSERT_EQ(got_str2, exprected_str2);
    }
    {
        const auto& subboard3 = numboard::NumboardSubViewFactory<3,5>::from_center(board, In<5>(0), In<8>(1));
        const std::string got_str3 = repr(subboard3);
        const std::string exprected_str3 =
                "247 240 241 242 243\n"
                "207 200 201 202 203\n"
                "217 210 211 212 213\n";
        ASSERT_EQ(got_str3, exprected_str3);
    }
}

TEST(Numboard, AlterViewCorner) {
    using numboard::In;
    const auto& board = numboard_tests::prepare_board<5, 8>();
    const auto& altered1 = numboard::NumboardAlterViewFactory<5, 8>::offset_corner<3, 2>(board, 888);
    const std::string got_str = repr(altered1);
    const std::string exprected_str =
            "200 201 202 203 204 205 206 207\n"
            "210 211 212 213 214 215 216 217\n"
            "220 221 222 223 224 225 226 227\n"
            "230 231 888 233 234 235 236 237\n"
            "240 241 242 243 244 245 246 247\n";
    ASSERT_EQ(got_str, exprected_str);
}

TEST(Numboard, AlterViewCenter) {
    using numboard::In;
    const auto& board = numboard_tests::prepare_board<5, 7>();
    const auto& altered1 = numboard::NumboardAlterViewFactory<5, 7>::center(board, 888);
    const std::string got_str = repr(altered1);
    const std::string exprected_str =
            "200 201 202 203 204 205 206\n"
            "210 211 212 213 214 215 216\n"
            "220 221 222 888 224 225 226\n"
            "230 231 232 233 234 235 236\n"
            "240 241 242 243 244 245 246\n";
    ASSERT_EQ(got_str, exprected_str);
}


TEST(Numboard, SubviewCenterSubviewCorner) {
    using numboard::In;
    const auto& board = numboard_tests::prepare_board<5, 8>();
    const auto& subboard1 = numboard::NumboardSubViewFactory<3,5>::from_center(board, In<5>(0), In<8>(1));
    const auto& subboard12 = numboard::NumboardSubViewFactory<2,4>::from_corner(subboard1, In<3>(2), In<5>(3));
    const std::string got_str12 = repr(subboard12);
    const std::string exprected_str12 =
            "212 213 217 210\n"
            "242 243 247 240\n";
    ASSERT_EQ(got_str12, exprected_str12);
}

TEST(Numboard, SubviewCenterSubviewCornerAlterCenter) {
    using numboard::In;
    const auto& board = numboard_tests::prepare_board<5, 8>();
    //std::cout << numboard::NumboardStreamer(board) << std::endl;
    const auto& subboard1 = numboard::NumboardSubViewFactory<3, 5>::from_center(board, In<5>(0), In<8>(1));
    //std::cout << numboard::NumboardStreamer(subboard1) << std::endl;
    const auto& subboard12 = numboard::NumboardSubViewFactory<3, 3>::from_corner(subboard1, In<3>(2), In<5>(3));
    //std::cout << numboard::NumboardStreamer(subboard12) << std::endl;
    const auto& altered_subboard12 = numboard::NumboardAlterViewFactory<3, 3>::center(subboard12, 888);
    const std::string got = repr(altered_subboard12);
    const std::string exprected =
            "212 213 217\n"
            "242 888 247\n"
            "202 203 207\n";
    ASSERT_EQ(got, exprected);
}
