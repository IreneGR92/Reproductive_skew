
#ifndef REPRODUCTIVE_SKEW_FILEPRINTER_H
#define REPRODUCTIVE_SKEW_FILEPRINTER_H


#include <string>
#include "Parameters.h"
#include "ResultCache.h"

class FilePrinter {
private:
    Parameters *parameters;

    void printHeader(std::ofstream &writer);

    static void writeResults(std::ofstream &writer, const std::vector<std::string>& cache);

public:
    explicit FilePrinter(Parameters *parameters) : parameters(parameters) {}

    void writeMainFile(std::vector<ResultCache *> &results);

    void writeLastGenerationFile(std::vector<ResultCache *> &results);
};


#endif //REPRODUCTIVE_SKEW_FILEPRINTER_H
