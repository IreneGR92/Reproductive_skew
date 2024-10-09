#ifndef CONFIG_H
#define CONFIG_H
#include <string>
#include <__filesystem/operations.h>
#include "yaml-cpp/yaml.h"
#include "spdlog/spdlog.h"

namespace YAML {
    class BadFile;
}

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
    static void loadConfig() {
        std::string url;
        YAML::Node config;

        if (std::filesystem::exists("~/.reproductive_skew_config.yml")) {
            spdlog::trace("loading config using ~/.reproductive_skew_config.yml");
            url = "~/.reproductive_skew_config.yml";
        } else {
            spdlog::trace("loading config using ../config.yml");
            url = "../config.yml";
        }
        try {
            config = YAML::LoadFile(url);
        } catch (YAML::BadFile &e) {
            spdlog::error("Error loading config from file: {}", url);
            throw e;
        }

        MAX_THREADS = config["MAX_THREADS"].as<int>();
        RUN_MULTITHREADED = config["RUN_MULTITHREADED"].as<bool>();
        OUTPUT_DIR = config["OUTPUT_DIR"].as<std::string>();
        PARAMETERS_FOLDER = config["PARAMETERS_FOLDER"].as<std::string>();
    }

    static int GET_MAX_THREADS() {
        return MAX_THREADS;
    }

    static bool IS_MULTITHREADED() {
        return RUN_MULTITHREADED;
    }

    static std::string GET_OUTPUT_DIR() {
        return OUTPUT_DIR;
    }

    static std::string GET_PARAMETERS_FOLDER() {
        return PARAMETERS_FOLDER;
    }
};
#endif //CONFIG_H
