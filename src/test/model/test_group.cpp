#include <gtest/gtest.h>
#include "../../main/model/Group.h"
#include "../../main/util/Config.h"


TEST(GroupTest, GroupReassignBreeders) {
    //given
    Config::loadConfig();
    auto parameters = std::make_shared<Parameters>("baseline.yml", 0);
    Group group(parameters);
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
    Config::loadConfig();
    auto parameters = std::make_shared<Parameters>("baseline.yml", 0);
    Group group(parameters);
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

TEST(GroupTest, OffspringProduction) {
    //given
    Config::loadConfig();
    auto parameters = std::make_shared<Parameters>("baseline.yml", 0);
    Group group(parameters);
    int initialGroupSize, groupSizeAfterReproduction;
    double fecundity;
    int newBreederOutsider = 0, newBreederInsider = 0, inheritance = 0;

    //when
    for (int i = 0; i < 10; i++) {
        initialGroupSize = group.getGroupSize();
        group.transferBreedersToHelpers();
        group.reassignBreeders(newBreederOutsider, newBreederInsider, inheritance);
        group.reproduce(i, 1);
        fecundity = group.getFecundityGroup();
        group.calculateGroupSize();
        groupSizeAfterReproduction = group.getGroupSize();
        //then
        EXPECT_EQ(groupSizeAfterReproduction, initialGroupSize + fecundity);
        EXPECT_EQ(group.getFecundityGroup(), group.getOffspringMainBreeder() + group.getOffspringSubordinateBreeders());
    }
}


TEST(GroupTest, CalulateFecundity) {
    //given
    Config::loadConfig();
    auto parameters = std::make_shared<Parameters>("default.yml", 0);
    Group group(parameters);


    const int fecundityBefore = group.getFecundityGroup();
    printf("fecundityBefore: %d\n", fecundityBefore);
    //when
    const int fecundityAfter = group.calcFecundity(1);
    printf("fecundityAfter: %f\n", group.getFecundityGroup());

    //then
//    EXPECT_EQ(fecundityBefore < fecundityAfter);
}