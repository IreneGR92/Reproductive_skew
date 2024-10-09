#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config {
    /**
     * Maximum number of working threads to use for running simulations
     * it will use more threads but only with light work on them and mainly idling
     */
    static int MAX_THREADS;

    /**
    * Flag to enable/disable multithreaded execution
    */
    static bool RUN_MULTITHREADED;

    /**
     * Path to the output directory where the results will be stored
     */
    static std::string OUTPUT_DIR;

    /**
     * Path to the folder containing the parameter files
     */
    static std::string PARAMETERS_FOLDER;

public:
    static void loadConfig();

    static int &GET_MAX_THREADS();

    static bool &IS_MULTITHREADED();

    static std::string &GET_OUTPUT_DIR();

    static std::string &GET_PARAMETERS_FOLDER();
};


#endif //CONFIG_H
