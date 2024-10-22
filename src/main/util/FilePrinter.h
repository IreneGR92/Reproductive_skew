#ifndef REPRODUCTIVE_SKEW_FILEPRINTER_H
#define REPRODUCTIVE_SKEW_FILEPRINTER_H


#include <string>
#include "Parameters.h"
#include "ResultCache.h"

class FilePrinter {
    const int PRECISION = 4;

    std::shared_ptr<Parameters> &parameters;

    void printHeader(std::ofstream &writer);

    static void writeResults(std::ofstream &writer, const std::vector<std::string> &cache);

    std::unique_ptr<std::ofstream> mainWriter; ///< A pointer to the main output stream writer.
    std::unique_ptr<std::ofstream> lastGenerationWriter; ///< A pointer to the last generation output stream writer.

public:
    explicit FilePrinter(std::shared_ptr<Parameters> &parameters);

    virtual ~FilePrinter();

    void writeMainFile(std::vector<std::unique_ptr<ResultCache> > &results);

    void writeLastGenerationFile(std::vector<std::unique_ptr<ResultCache> > &results);
};


#endif //REPRODUCTIVE_SKEW_FILEPRINTER_H
