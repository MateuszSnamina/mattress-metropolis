// GTEST:
#include <gtest/gtest.h>
// SELF:
#include <numboard_tests/prepare_board.hpp>
// TESTEE:
#include<energy/energy_getter.hpp>
//#include<numboard/numboard_streamer.hpp> // TEMP
#include<energy/energy_getter_ising_multiplet.hpp>
#include<energy/energy_getter_ising_doublet.hpp>
// STD:
#include<iostream>

TEST(Energy, IsingMultipletSimplesEnergyGetter) {
    const energy::ising::multiplet::ZeroNnPartsEnergyGetter parts_energy_getter{1001};
    {
        const energy::getter::ZeroNnEnergyGetter<4, 4> energy_getter{parts_energy_getter};
        const auto& neighborhood = numboard_tests::prepare_board<3, 3>();
        //std::cout << numboard::NumboardStreamer(neighborhood);
        const double got_neighborhood_energy = energy_getter.get_neighborhood_energy(neighborhood);
        const double expected_neighborhood_energy =
                - (500 - 210) * (500 - 211) - (500 - 211) * (500 - 212)
                - (500 - 201) * (500 - 211) - (500 - 211) * (500 - 221);
        ASSERT_DOUBLE_EQ(got_neighborhood_energy, expected_neighborhood_energy);
    }
    {
        const energy::getter::ZeroNnEnergyGetter<4, 4> energy_getter{parts_energy_getter};
        const auto& board = numboard_tests::prepare_board<4, 4>();
        //std::cout << numboard::NumboardStreamer(board);
        const double got_board_energy = energy_getter.get_board_energy(board);
        const double expected_board_energy =
                // horizontal bonds:
                - (500 - 200) * (500 - 201) - (500 - 201) * (500 - 202) - (500 - 202) * (500 - 203) - (500 - 203) * (500 - 200)
                - (500 - 210) * (500 - 211) - (500 - 211) * (500 - 212) - (500 - 212) * (500 - 213) - (500 - 213) * (500 - 210)
                - (500 - 220) * (500 - 221) - (500 - 221) * (500 - 222) - (500 - 222) * (500 - 223) - (500 - 223) * (500 - 220)
                - (500 - 230) * (500 - 231) - (500 - 231) * (500 - 232) - (500 - 232) * (500 - 233) - (500 - 233) * (500 - 230)
                // horizontal vertical bonds:
                - (500 - 200) * (500 - 210) - (500 - 210) * (500 - 220) - (500 - 220) * (500 - 230) - (500 - 230) * (500 - 200)
                - (500 - 201) * (500 - 211) - (500 - 211) * (500 - 221) - (500 - 221) * (500 - 231) - (500 - 231) * (500 - 201)
                - (500 - 202) * (500 - 212) - (500 - 212) * (500 - 222) - (500 - 222) * (500 - 232) - (500 - 232) * (500 - 202)
                - (500 - 203) * (500 - 213) - (500 - 213) * (500 - 223) - (500 - 223) * (500 - 233) - (500 - 233) * (500 - 203);
        ASSERT_DOUBLE_EQ(got_board_energy, expected_board_energy);
    }
}

TEST(Energy, IsingMultipletFourNnEnergyGetter) {
    const energy::ising::multiplet::FourNnPartsEnergyGetter parts_energy_getter{1001};
    {
        const energy::getter::FourNnEnergyGetter<8, 7> energy_getter{parts_energy_getter};
        const auto& neighborhood = numboard_tests::prepare_board<7, 7>();
        //std::cout << numboard::NumboardStreamer(neighborhood);
        const double got_neighborhood_energy = energy_getter.get_neighborhood_energy(neighborhood);
        const double expected_neighborhood_energy =
                // upper horizonal bonds:
                - (500 - 222) * (500 - 223) - (500 - 223) * (500 - 224)
                // mid horizonal bonds:
                - (500 - 231) * (500 - 232) - (500 - 232) * (500 - 233) - (500 - 233) * (500 - 234) - (500 - 234) * (500 - 235)
                // upper lower bonds:
                - (500 - 242) * (500 - 243) - (500 - 243) * (500 - 244)
                // left vertical bonds:
                - (500 - 222) * (500 - 232) - (500 - 232) * (500 - 242)
                // mid vertical bonds:
                - (500 - 213) * (500 - 223) - (500 - 223) * (500 - 233) - (500 - 233) * (500 - 243) - (500 - 243) * (500 - 253)
                // right vertical bonds:
                - (500 - 224) * (500 - 234) - (500 - 234) * (500 - 244);
        ASSERT_DOUBLE_EQ(got_neighborhood_energy, expected_neighborhood_energy);
    }
    {
        const energy::getter::FourNnEnergyGetter<8, 7> energy_getter{parts_energy_getter};
        const auto& board = numboard_tests::prepare_board<8, 7>();
        //std::cout << numboard::NumboardStreamer(board);
        const double got_board_energy = energy_getter.get_board_energy(board);
        const double expected_board_energy =
                // horizontal bonds:
                - (500 - 200) * (500 - 201) - (500 - 201) * (500 - 202) - (500 - 202) * (500 - 203) - (500 - 203) * (500 - 204) - (500 - 204) * (500 - 205) - (500 - 205) * (500 - 206) - (500 - 206) * (500 - 200)
                - (500 - 210) * (500 - 211) - (500 - 211) * (500 - 212) - (500 - 212) * (500 - 213) - (500 - 213) * (500 - 214) - (500 - 214) * (500 - 215) - (500 - 215) * (500 - 216) - (500 - 216) * (500 - 210)
                - (500 - 220) * (500 - 221) - (500 - 221) * (500 - 222) - (500 - 222) * (500 - 223) - (500 - 223) * (500 - 224) - (500 - 224) * (500 - 225) - (500 - 225) * (500 - 226) - (500 - 226) * (500 - 220)
                - (500 - 230) * (500 - 231) - (500 - 231) * (500 - 232) - (500 - 232) * (500 - 233) - (500 - 233) * (500 - 234) - (500 - 234) * (500 - 235) - (500 - 235) * (500 - 236) - (500 - 236) * (500 - 230)
                - (500 - 240) * (500 - 241) - (500 - 241) * (500 - 242) - (500 - 242) * (500 - 243) - (500 - 243) * (500 - 244) - (500 - 244) * (500 - 245) - (500 - 245) * (500 - 246) - (500 - 246) * (500 - 240)
                - (500 - 250) * (500 - 251) - (500 - 251) * (500 - 252) - (500 - 252) * (500 - 253) - (500 - 253) * (500 - 254) - (500 - 254) * (500 - 255) - (500 - 255) * (500 - 256) - (500 - 256) * (500 - 250)
                - (500 - 260) * (500 - 261) - (500 - 261) * (500 - 262) - (500 - 262) * (500 - 263) - (500 - 263) * (500 - 264) - (500 - 264) * (500 - 265) - (500 - 265) * (500 - 266) - (500 - 266) * (500 - 260)
                - (500 - 270) * (500 - 271) - (500 - 271) * (500 - 272) - (500 - 272) * (500 - 273) - (500 - 273) * (500 - 274) - (500 - 274) * (500 - 275) - (500 - 275) * (500 - 276) - (500 - 276) * (500 - 270)
                // horizontal vertical bonds:
                - (500 - 200) * (500 - 210) - (500 - 210) * (500 - 220) - (500 - 220) * (500 - 230) - (500 - 230) * (500 - 240) - (500 - 240) * (500 - 250) - (500 - 250) * (500 - 260) - (500 - 260) * (500 - 270) - (500 - 270) * (500 - 200)
                - (500 - 201) * (500 - 211) - (500 - 211) * (500 - 221) - (500 - 221) * (500 - 231) - (500 - 231) * (500 - 241) - (500 - 241) * (500 - 251) - (500 - 251) * (500 - 261) - (500 - 261) * (500 - 271) - (500 - 271) * (500 - 201)
                - (500 - 202) * (500 - 212) - (500 - 212) * (500 - 222) - (500 - 222) * (500 - 232) - (500 - 232) * (500 - 242) - (500 - 242) * (500 - 252) - (500 - 252) * (500 - 262) - (500 - 262) * (500 - 272) - (500 - 272) * (500 - 202)
                - (500 - 203) * (500 - 213) - (500 - 213) * (500 - 223) - (500 - 223) * (500 - 233) - (500 - 233) * (500 - 243) - (500 - 243) * (500 - 253) - (500 - 253) * (500 - 263) - (500 - 263) * (500 - 273) - (500 - 273) * (500 - 203)
                - (500 - 204) * (500 - 214) - (500 - 214) * (500 - 224) - (500 - 224) * (500 - 234) - (500 - 234) * (500 - 244) - (500 - 244) * (500 - 254) - (500 - 254) * (500 - 264) - (500 - 264) * (500 - 274) - (500 - 274) * (500 - 204)
                - (500 - 205) * (500 - 215) - (500 - 215) * (500 - 225) - (500 - 225) * (500 - 235) - (500 - 235) * (500 - 245) - (500 - 245) * (500 - 255) - (500 - 255) * (500 - 265) - (500 - 265) * (500 - 275) - (500 - 275) * (500 - 205)
                - (500 - 206) * (500 - 216) - (500 - 216) * (500 - 226) - (500 - 226) * (500 - 236) - (500 - 236) * (500 - 246) - (500 - 246) * (500 - 256) - (500 - 256) * (500 - 266) - (500 - 266) * (500 - 276) - (500 - 276) * (500 - 206);
        ASSERT_DOUBLE_EQ(got_board_energy, expected_board_energy);
    }
}
