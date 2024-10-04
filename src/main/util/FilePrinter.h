
#ifndef REPRODUCTIVE_SKEW_FILEPRINTER_H
#define REPRODUCTIVE_SKEW_FILEPRINTER_H


#include <string>
#include <memory>
#include "Parameters.h"
#include "ResultCache.h"

class FilePrinter {
private:
    std::shared_ptr<Parameters> parameters;

    void printHeader(std::ofstream &writer);

    static void writeResults(std::ofstream &writer, const std::vector<std::string> &cache);

    std::ofstream *mainWriter; ///< A pointer to the main output stream writer.
    std::ofstream *lastGenerationWriter; ///< A pointer to the last generation output stream writer.

public:
    explicit FilePrinter(std::shared_ptr<Parameters> parameters);

    void writeMainFile(std::vector<ResultCache *> &results);

    void writeLastGenerationFile(std::vector<ResultCache *> &results);
};


#endif //REPRODUCTIVE_SKEW_FILEPRINTER_H
