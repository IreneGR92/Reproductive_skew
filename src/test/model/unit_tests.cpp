#include <gtest/gtest.h>

int main(int count, char **argv) {
    testing::InitGoogleTest(&count, argv);
    return RUN_ALL_TESTS();
}