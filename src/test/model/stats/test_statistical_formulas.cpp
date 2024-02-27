#include <gtest/gtest.h>
#include "../../../main/stats/StatisticalFormulas.h"

namespace {
    void addValues(StatisticalFormulas &stats) {
        stats.addValue(5);
        stats.addValue(5);
        stats.addValue(2);
        stats.addValue(6);
    }
}

TEST(StatisticalForumlasTest, getMaxValueTest) {
    //given
    StatisticalFormulas stats;

    //when
    addValues(stats);
    //then
    EXPECT_EQ(stats.getMaxValue(), 6);

}

TEST(StatisticalForumlasTest, mergeTest) {
    //given
    StatisticalFormulas stats;
    StatisticalFormulas statsToMerge;
    addValues(stats);
    addValues(statsToMerge);
    statsToMerge.addValue(1);

    //when
    stats.merge(statsToMerge);
    //then
    EXPECT_EQ(stats.size(), 9);

}

TEST(StatisticalForumlasTest, correlationTest) {
    //given
    StatisticalFormulas stats;
    StatisticalFormulas correlated;
    StatisticalFormulas notCorrelated;

    addValues(stats);
    addValues(correlated);
    notCorrelated.addValue(1000);
    notCorrelated.addValue(500);

    //when

    //then
    EXPECT_EQ(stats.correlation(correlated), 1);
    EXPECT_EQ(stats.correlation(notCorrelated), 0);

}

