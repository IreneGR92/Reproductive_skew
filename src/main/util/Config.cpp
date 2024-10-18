#include <string>
#include <filesystem>
#include <thread>
#include "yaml-cpp/yaml.h"
#include "spdlog/spdlog.h"
#include "Config.h"


// Define static members
int Config::MAX_THREADS;
std::string Config::OUTPUT_DIR;
std::string Config::PARAMETERS_FOLDER;
std::string Config::COLLECTION_FILE;
std::string Config::COLLECTION_FOLDER;
std::string Config::LOG_PATTERN;
std::string Config::LOG_FILE;
std::string Config::LOG_LEVEL;
bool Config::LOG_TO_CONSOLE;
bool Config::LOG_TO_FILE;

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
    OUTPUT_DIR = config["OUTPUT_DIR"].as<std::string>();
    PARAMETERS_FOLDER = config["PARAMETERS_FOLDER"].as<std::string>();
    COLLECTION_FILE = config["COLLECTION_FILE"].as<std::string>();
    COLLECTION_FOLDER = config["COLLECTION_FOLDER"].as<std::string>();
    LOG_PATTERN = config["LOG_PATTERN"].as<std::string>();
    LOG_FILE = config["LOG_FILE"].as<std::string>();
    LOG_TO_CONSOLE = config["LOG_TO_CONSOLE"].as<bool>();
    LOG_TO_FILE = config["LOG_TO_FILE"].as<bool>();
    LOG_LEVEL = config["LOG_LEVEL"].as<std::string>();
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

const int &Config::GET_MAX_THREADS() {
    return MAX_THREADS;
}

const std::string &Config::GET_PARAMETERS_FOLDER() {
    return PARAMETERS_FOLDER;
}

const std::string &Config::GET_OUTPUT_DIR() {
    return OUTPUT_DIR;
}

const std::string &Config::GET_LOG_PATTERN() {
    return LOG_PATTERN;
}

const std::string &Config::GET_LOG_FILE() {
    return LOG_FILE;
}

const bool &Config::IS_LOG_TO_CONSOLE() {
    return LOG_TO_CONSOLE;
}

const bool &Config::IS_LOG_TO_FILE() {
    return LOG_TO_FILE;
}

const std::string &Config::GET_LOG_LEVEL() {
    return LOG_LEVEL;
}


const std::string &Config::GET_COLLECTION_FILE() {
    return COLLECTION_FILE;
}

const std::string &Config::GET_COLLECTION_FOLDER() {
    return COLLECTION_FOLDER;
}
