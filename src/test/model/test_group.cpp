#include <gtest/gtest.h>
#include "../../main/model/Group.h"

// Demonstrate some basic assertions.
TEST(GroupTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

TEST(GroupTest, GroupReassignBreeders) {
    //given

    Group group;
//when

    group.reassignBreeders();

//then


    std::cout << "GroupTest, GroupReassignBreeders" << std::endl;


}
