#include <gtest/gtest.h>
#include "../../main/Parameters.h"

int main(int count, char **argv) {
    testing::InitGoogleTest(&count, argv);
    Parameters::unitTestInit();
    return RUN_ALL_TESTS();
}