#ifndef REPRODUCTIVE_SKEW_SIMULATIONRUNNER_H
#define REPRODUCTIVE_SKEW_SIMULATIONRUNNER_H


#include <memory>
#include "util/Parameters.h"
#include "util/ResultCache.h"
#include "loadbalancing/ThreadPool.h"

class SimulationRunner {
    std::shared_ptr<Parameters> parameters; // Pointer to the Parameters object

    // Static method to run a single simulation and store the result
    static void runSimulation(std::shared_ptr<Simulation> simulation, std::unique_ptr<ResultCache> &result);

    // Method to run simulations in a multi-threaded mode
    void runMultithreaded(std::vector<std::unique_ptr<ResultCache> > &results, const std::shared_ptr<ThreadPool>& threadPool);

    // Method to run simulations in a single-threaded mode
    void runSinglethreaded(std::vector<std::unique_ptr<ResultCache> > &results);

public:
    // Method to initialize and run the simulation with the given parameter file path
    void run(const std::string &parameterFilePath, const std::shared_ptr<ThreadPool>& threadPool);
};

#endif //REPRODUCTIVE_SKEW_SIMULATIONRUNNER_H
