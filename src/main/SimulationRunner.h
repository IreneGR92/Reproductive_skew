#ifndef REPRODUCTIVE_SKEW_SIMULATIONRUNNER_H
#define REPRODUCTIVE_SKEW_SIMULATIONRUNNER_H


#include "util/Parameters.h"
#include "util/ResultCache.h"
#include "loadbalancing/ThreadPool.h"

class SimulationRunner {
    /**
     * Pointer to the Parameters object.
        */
    std::shared_ptr<Parameters> parameters;


    /**
     * Static method to run a single simulation and store the result.
     *
     * @param simulation A shared pointer to the Simulation object.
     * @param result A unique pointer to the ResultCache object where the result will be stored.
     */
    static void runSimulation(std::shared_ptr<Simulation> simulation, std::unique_ptr<ResultCache> &result,
                              std::condition_variable &theadPoolEmptyCondition);

    /**
     * Method to run simulations in a multi-threaded mode.
     *
     * @param results A vector of unique pointers to ResultCache objects where the results will be stored.
     * @param threadPool A shared pointer to the ThreadPool used for managing threads.
     */
    void spawnWorkerThreads(std::vector<std::unique_ptr<ResultCache> > &results,
                            const std::shared_ptr<ThreadPool> &threadPool,
                            std::condition_variable &theadPoolEmptyCondition) const;

    /**
 * Method to initialize and run the simulation with the given parameter file path.
 *
 * @param parameterFilePath The file path to the parameter file.
 * @param threadPool A shared pointer to the ThreadPool used for managing threads.
 */
    void run(const std::string &parameterFilePath, const std::shared_ptr<ThreadPool> &threadPool,
             std::condition_variable &theadPoolEmptyCondition);

public:
    /**
 * Runs the simulations based on the provided parameters and thread pool.
 *
 * @param parameters A vector of strings containing the parameter file names.
 * @param threadPool A shared pointer to the ThreadPool used for managing threads.
 * @param stopFlag An atomic boolean flag to stop the application gracefully.
 */
    static void runSimulations(const std::vector<std::string> &parameters, std::shared_ptr<ThreadPool> &threadPool,
                               std::atomic<bool> &stopFlag);
};

#endif //REPRODUCTIVE_SKEW_SIMULATIONRUNNER_H
