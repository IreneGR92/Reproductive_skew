#ifndef REPRODUCTIVE_SKEW_SIMULATIONRUNNER_H
#define REPRODUCTIVE_SKEW_SIMULATIONRUNNER_H


#include "util/Parameters.h"
#include "util/ResultCache.h"

class SimulationRunner {
private:
    Parameters *parameters; // Pointer to the Parameters object

    // Static method to run a single simulation and store the result
    static void runSimulation(Simulation *simulation, ResultCache **result);

    // Method to run simulations in a multi-threaded mode
    void runMultithreaded(Parameters &parameters, std::vector<ResultCache *> &results);

    // Method to run simulations in a single-threaded mode
    void runSinglethreaded(Parameters &parameters, std::vector<ResultCache *> &results);

public:
    // Method to initialize and run the simulation with the given parameter file path
    void run(const std::string &parameterFilePath);
};

#endif //REPRODUCTIVE_SKEW_SIMULATIONRUNNER_H
