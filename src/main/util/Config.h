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
     * Path to the output directory where the results will be stored
     */
    static std::string OUTPUT_DIR;

    /**
     * Path to the folder containing the parameter files
     */
    static std::string PARAMETERS_FOLDER;

    /**
     * Parameter-collectioin filename
     */
    static std::string COLLECTION_FILE;

    static std::string COLLECTION_FOLDER;

    static std::string LOG_PATTERN;

    static std::string LOG_FILE;

    static std::string LOG_LEVEL;

    static bool LOG_TO_CONSOLE;

    static bool LOG_TO_FILE;

    /**
     * \brief Calculates the maximum number of threads to use for running simulations.
     *
     * This function determines the optimal number of threads based on the provided configuration.
     *
     * \param configThreads The number of threads specified in the configuration.
     * \return The calculated maximum number of threads.
     */
    static int calulateMaxThreads(int configThreads);

public:
    /**
   * \brief Loads the configuration from a YAML file.
   *
   * This function attempts to load the configuration from a file located in the user's home directory.
   * If the file is not found, it falls back to a default location.
   * The configuration values are then parsed and assigned to the corresponding static members.
   *
   * \throws YAML::BadFile if the configuration file cannot be loaded.
   */
    static void loadConfig();

    static const std::string &GET_COLLECTION_FILE();

    static const std::string &GET_COLLECTION_FOLDER();

    static const int &GET_MAX_THREADS();

    static const std::string &GET_PARAMETERS_FOLDER();

    static const std::string &GET_OUTPUT_DIR();

    static const std::string &GET_LOG_PATTERN();

    static const std::string &GET_LOG_FILE();

    static const bool &IS_LOG_TO_CONSOLE();

    static const bool &IS_LOG_TO_FILE();

    static const std::string &GET_LOG_LEVEL();
};


#endif //CONFIG_H
