#include <string>
#include <__filesystem/operations.h>
#include "yaml-cpp/yaml.h"
#include "spdlog/spdlog.h"
#include "Config.h"

void Config::loadConfig() {
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
