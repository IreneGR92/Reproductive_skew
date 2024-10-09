#include <string>
#include <filesystem>
#include <thread>
#include <iostream>
#include "yaml-cpp/yaml.h"
#include "spdlog/spdlog.h"
#include "Config.h"


// Define static members
int Config::MAX_THREADS;
bool Config::RUN_MULTITHREADED;
std::string Config::OUTPUT_DIR;
std::string Config::PARAMETERS_FOLDER;


void Config::loadConfig() {
    std::string url;
    YAML::Node config;

    // construct the path to the config file
    const char *homeDir = std::getenv("HOME");
    std::string configFilePath = std::string(homeDir) + "/.reproductive_skew.yml";

    if (std::filesystem::exists(configFilePath)) {
        spdlog::trace("loading config using {}", configFilePath);
        url = configFilePath;
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

    MAX_THREADS = calulateMaxThreads(config["MAX_THREADS"].as<int>());
    RUN_MULTITHREADED = config["RUN_MULTITHREADED"].as<bool>();
    OUTPUT_DIR = config["OUTPUT_DIR"].as<std::string>();
    PARAMETERS_FOLDER = config["PARAMETERS_FOLDER"].as<std::string>();
}

const int &Config::GET_MAX_THREADS() {
    return Config::MAX_THREADS;
}

const std::string &Config::GET_PARAMETERS_FOLDER() {
    return Config::PARAMETERS_FOLDER;
}

const bool &Config::IS_MULTITHREADED() {
    return Config::RUN_MULTITHREADED;
}

const std::string &Config::GET_OUTPUT_DIR() {
    return Config::OUTPUT_DIR;
}


int Config::calulateMaxThreads(int configThreads) {
    int numCores = std::thread::hardware_concurrency();
    int netThreads = 0;
    if (numCores == 0) {
        spdlog::warn("Unable to determine number of CPU cores. Using raw config value: {}", configThreads);
        if (configThreads == 0) {
            spdlog::error("Config value for MAX_THREADS is 0. And unable to determine number of CPU cores. Exiting...");
            exit(1);
        }
        netThreads = configThreads;
    } else {
        netThreads = std::min(configThreads, numCores);
        spdlog::info("Using {} threads out of {} available cores", netThreads, numCores);
    }
    return netThreads;
}
