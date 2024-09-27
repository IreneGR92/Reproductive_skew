
#ifndef REPRODUCTIVE_SKEW_FILEPRINTER_H
#define REPRODUCTIVE_SKEW_FILEPRINTER_H


#include <string>
#include "Parameters.h"

class FilePrinter {
private:
    Parameters *parameters;

    void printHeader(std::ofstream &outputStream);

public:
    explicit FilePrinter(Parameters *parameters) : parameters(parameters) {}

    void writeMainFile();

    void writeLastGenerationFile();
};


#endif //REPRODUCTIVE_SKEW_FILEPRINTER_H
