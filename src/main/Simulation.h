#ifndef GROUP_AUGMENTATION_SIMULATION_H
#define GROUP_AUGMENTATION_SIMULATION_H

#include "Parameters.h"
#include "model/Population.h"

// The Simulation class represents a single simulation run.
class Simulation {
    // A pointer to the Parameters singleton, which holds the parameters for the simulation.
    Parameters *parameters;

    // The population of individuals in the simulation.
    Population population;

    // The replica number for this simulation.
    const int replica;

    // The current generation number in the simulation.
    int generation = 0;

public:
    // Constructor for the Simulation class.
    // @param replica: The replica number for this simulation.
    Simulation(const int replica);

    // Runs the simulation.
    void run();

    // Returns the replica number for this simulation.
    // @return The replica number.
    int getReplica() const;

    // Returns the current generation number in the simulation.
    // @return The generation number.
    int getGeneration() const;
};

#endif //GROUP_AUGMENTATION_SIMULATION_H