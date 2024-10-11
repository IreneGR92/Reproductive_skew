//
// Created by odin on 11.10.2024.
//

#include "Util.h"

#include <fstream>

#include "Config.h"
#include "spdlog/logger.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

void Util::setupLogging() {
    // Create a console sink
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    // Create a file sink
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(Config::GET_LOG_FILE(), true);

    // Combine the sinks into a single logger
    std::vector<spdlog::sink_ptr> sinks;

    if (Config::IS_LOG_TO_CONSOLE())
        sinks.push_back(console_sink);

    if (Config::IS_LOG_TO_FILE())
        sinks.push_back(file_sink);

    auto logger = std::make_shared<spdlog::logger>("logger", sinks.begin(), sinks.end());
    logger->set_level(spdlog::level::from_str(Config::GET_LOG_LEVEL()));

    logger->set_pattern(Config::GET_LOG_PATTERN());
    // Set the combined logger as the default logger
    set_default_logger(logger);
}

std::vector<std::string> Util::loadParameterFiles() {
    // Get the path to the collections file
    std::string filePath = Config::GET_COLLECTION_FOLDER() + "/" + Config::GET_COLLECTION_FILE();

    std::vector<std::string> parameterFiles;
    std::ifstream file(filePath);
    std::string line;

    // Read each line from the file and add to the parameter files vector
    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (line.empty()) continue; //ignore empty lines
            if (line[0] == '#') continue; //ignore comments
            parameterFiles.push_back(Config::GET_PARAMETERS_FOLDER() + "/" + line);
        }
        file.close();
    } else {
        spdlog::error("Unable to open file: {} ", filePath);
    }
    return parameterFiles;
}
