#include <gtest/gtest.h>
#include "../../main/model/Group.h"


// Demonstrate some basic assertions.
TEST(GroupTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

//

TEST(GroupTest, GroupReassignBreeders) {
    //given
    Group group;
    int newBreederOutsider = 0, newBreederInsider = 0, inheritance = 0;
    int deaths = 0;
    group.calculateGroupSize();
    const int initialGroupSize = group.getGroupSize();

    //when
    for (int i = 0; i < 100; i++) {
        group.survivalGroup();
        group.reassignBreeders(newBreederOutsider, newBreederInsider, inheritance);
        group.mortalityGroup(deaths);

        //then
        if (group.getGroupSize() == 0) {
            EXPECT_EQ(deaths, initialGroupSize);
        }
    }
}

TEST(GroupTest, GroupReassignBreedersStaySameSize) {
    //given
    Group group;
    int newBreederOutsider = 0, newBreederInsider = 0, inheritance = 0;
    group.calculateGroupSize();
    const int initialGroupSize = group.getGroupSize();

    //when
    for (int i = 0; i < 100; i++) {
        group.reassignBreeders(newBreederOutsider, newBreederInsider, inheritance);
        group.calculateGroupSize();
        group.survivalGroup();
        //then
        EXPECT_EQ(group.getGroupSize(), initialGroupSize);

    }
}
