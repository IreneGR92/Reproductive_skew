#include <gtest/gtest.h>
#include "../../../main/model/container/Container.h"

TEST(ContainerTest, TestRandomness) {
    auto *container1 = new Container<int>();
    auto *container2 = new Container<int>();

//    auto *container1 = new Container<Parameters>();

    int four = 4;
    int two = 2;
    int one = 1;

    EXPECT_TRUE(container1->isEmpty());
    EXPECT_TRUE(container2->isEmpty());

    container1->add(four);
    container1->add(two);
    container1->add(one);
    container1->remove(1); //by pointer instead of index if possible
    container1->add(two);

    EXPECT_EQ(container1->size(), 3);
    EXPECT_FALSE(container1->isEmpty());


    std::cout << container1->accessElement(0) << std::endl;

    container2->add(four);
    container2->add(four);
    container2->add(four);
    container2->add(four);
    container2->add(four);

    EXPECT_EQ(container2->size(), 5);
    EXPECT_FALSE(container2->isEmpty());

    container1->merge(*container2);
    EXPECT_EQ(container1->size(), 8);

    delete container1;
    delete container2;

}