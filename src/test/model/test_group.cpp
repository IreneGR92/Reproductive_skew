#include <gtest/gtest.h>
#include "../../main/model/Group.h"


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
        group.transferBreedersToHelpers();
        group.reassignBreeders(newBreederOutsider, newBreederInsider, inheritance);
        group.mortalityGroup(deaths);
        int currentGroupSize = group.getGroupSize();

        //then
        if (currentGroupSize == 0) {
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
        group.transferBreedersToHelpers();
        group.reassignBreeders(newBreederOutsider, newBreederInsider, inheritance);
        group.calculateGroupSize();
        group.survivalGroup();
        //then
        EXPECT_EQ(group.getGroupSize(), initialGroupSize);

    }
}

TEST(GroupTest, OffspringProduction) { //TODO: This test is not working after reproduce method
    //given
    Group group;
    int initialGroupSize, groupSizeAfterReproduction;
    int fecundity;
    int newBreederOutsider = 0, newBreederInsider = 0, inheritance = 0;

    //when
    for (int i = 0; i < 10; i++) {
        initialGroupSize = group.getGroupSize();
        group.transferBreedersToHelpers();
        group.reassignBreeders(newBreederOutsider, newBreederInsider, inheritance);
        group.reproduce(i);
        fecundity = group.getFecundityGroup();
        group.calculateGroupSize();
        groupSizeAfterReproduction = group.getGroupSize();
                //then
        EXPECT_EQ(groupSizeAfterReproduction, initialGroupSize + fecundity);
        EXPECT_EQ(group.getFecundityGroup(), group.getOffspringMainBreeder() + group.getOffspringSubordinateBreeders());
    }
}