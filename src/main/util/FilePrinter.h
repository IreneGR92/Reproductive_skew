#ifndef REPRODUCTIVE_SKEW_FILEPRINTER_H
#define REPRODUCTIVE_SKEW_FILEPRINTER_H


#include <string>
#include "Parameters.h"
#include "ResultCache.h"


/**
 * \class FilePrinter
 * \brief Handles printing results to files.
 */
class FilePrinter {
    const int PRECISION = 4;

    std::shared_ptr<Parameters> &parameters;

    /**
     * \brief Prints the header to the given writer.
     * \param writer The output stream writer.
     */
    void printHeader(std::ofstream &writer);

    /**
     * \brief Writes results to the given writer.
     * \param writer The output stream writer.
     * \param cache The results to write.
     */
    static void writeResults(std::ofstream &writer, const std::vector<std::string> &cache);

    std::unique_ptr<std::ofstream> mainWriter; ///< A pointer to the main output stream writer.
    std::unique_ptr<std::ofstream> lastGenerationWriter; ///< A pointer to the last generation output stream writer.

public:
    /**
     * \brief Constructs a FilePrinter with the given parameters.
     * \param parameters The parameters to use.
     */
    explicit FilePrinter(std::shared_ptr<Parameters> &parameters);

    /**
     * \brief Destructor for FilePrinter.
     */
    virtual ~FilePrinter();

    /**
     * \brief Writes the main file with the given results.
     * \param results The results to write.
     */
    void writeMainFile(std::vector<std::unique_ptr<ResultCache> > &results);

    /**
     * \brief Writes the last generation file with the given results.
     * \param results The results to write.
     */
    void writeLastGenerationFile(std::vector<std::unique_ptr<ResultCache> > &results);
};


#endif //REPRODUCTIVE_SKEW_FILEPRINTER_H
