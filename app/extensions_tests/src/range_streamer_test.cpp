#include <extensions/range_streamer.hpp>

#include <iomanip>
#include <vector>

#include <gtest/gtest.h>

TEST(RangeStreamer, RawRangeStreamerSixElementRange) {
    using namespace extension::boost::stream_pragma;
    int v1[6] = {13, 14, 15, 16, 11, 12};
    ASSERT_EQ((v1 | RSS()).str(), "{0:13,1:14,2:15,3:16,4:11,5:12}");
}

TEST(RangeStreamer, RawRangeStreamerOneElementRange) {
    using namespace extension::boost::stream_pragma;
    int v2[1] = {13};
    ASSERT_EQ((v2 | RSS()).str(), "{0:13}");
}

TEST(RangeStreamer, RawRangeStreamerEmptyRange) {
    using namespace extension::boost::stream_pragma;
    std::vector<int> v3;
    ASSERT_EQ((v3 |RSS()).str(), "{}");
}

TEST(RangeStreamer, Fancy) {
    using namespace extension::boost::stream_pragma;
    double v1[3] = {1.1, 1.2, 1.3};
    const auto range_stream_settings = RSS()
            .set_stream_preparer([](std::ostream& s) { s << "BEGIN***|" << std::fixed << std::setprecision(2) << std::showpos; })
            .set_stream_sustainer([](std::ostream& s, size_t i) { s << std::setw(2) << char('A' + i) << ":" << std::setw(6); })
    .set_stream_separer([](std::ostream& s) { s << "|"; })
    .set_stream_finisher([](std::ostream& s) { s << "|***END"; });
    const auto str = (v1 | range_stream_settings).str();
    ASSERT_EQ(str, "BEGIN***| A: +1.10| B: +1.20| C: +1.30|***END");
}

TEST(RangeStreamer, FormatIndependencyFlagOff) {
    using namespace extension::boost::stream_pragma;
    double v1[3] = {1.1, 1.2, 1.3};
    std::ostringstream oss;
    oss.fill('*');
    oss << std::setw(5);
    const auto range_stream_settings = RSS()
            .set_stream_preparer([](std::ostream& s) { s << "{" << std::fixed << std::setprecision(2) << std::showpos; })
            .set_stream_sustainer([](std::ostream& s, size_t i) { s << std::setw(2) << char('A' + i) << ":" << std::setw(6); })
    .set_stream_separer([](std::ostream& s) { s << "|"; })
    .set_format_independence_flag(false);
    oss << (v1 | range_stream_settings);
    oss << 1.4;
    ASSERT_EQ(oss.str(), "****{*A:*+1.10|*B:*+1.20|*C:*+1.30}+1.40");
}

TEST(RangeStreamer, FormatIndependencyFlagExplicitOn) {
    using namespace extension::boost::stream_pragma;
    double v1[3] = {1.1, 1.2, 1.3};
    std::ostringstream oss;
    oss.fill('*');
    oss << std::setw(5);
    const auto range_stream_settings = RSS()
            .set_stream_preparer([](std::ostream& s) { s << "{" << std::fixed << std::setprecision(2) << std::showpos; })
            .set_stream_sustainer([](std::ostream& s, size_t i) { s << std::setw(2) << char('A' + i) << ":" << std::setw(6); })
    .set_stream_separer([](std::ostream& s) { s << "|"; })
    .set_format_independence_flag(true);
    oss << (v1 | range_stream_settings);
    oss << 1.4;
    ASSERT_EQ(oss.str(), "{ A: +1.10| B: +1.20| C: +1.30}**1.4");
}

TEST(RangeStreamer, FormatIndependencyFlagImplicitOn) {
    using namespace extension::boost::stream_pragma;
    double v1[3] = {1.1, 1.2, 1.3};
    std::ostringstream oss;
    oss.fill('*');
    oss << std::setw(5);
    const auto range_stream_settings = RSS()
            .set_stream_preparer([](std::ostream& s) { s << "{" << std::fixed << std::setprecision(2) << std::showpos; })
           .set_stream_sustainer([](std::ostream& s, size_t i) { s << std::setw(2) << char('A' + i) << ":" << std::setw(6); })
            .set_stream_separer([](std::ostream& s) { s << "|"; });
    oss << (v1 | range_stream_settings);
    oss << 1.4;
    ASSERT_EQ(oss.str(), "{ A: +1.10| B: +1.20| C: +1.30}**1.4");
}
