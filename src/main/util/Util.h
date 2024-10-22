#ifndef UTIL_H
#define UTIL_H
#include <string>
#include <vector>


struct Util {
    static void setupLogging();

    static std::vector<std::string> loadParameterFiles();
};


#endif //UTIL_H
