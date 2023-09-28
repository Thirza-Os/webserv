#include "configParser.hpp"
#include "serverConfig.hpp"
#include "utilities/utilities.hpp"

#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>

configParser::configParser(std::string path) {
    if (path.empty()) {
        throw ConfigParserException("Config file path is empty");
    }

    if (!std::filesystem::exists(path)) {
        throw ConfigParserException("Configuration file not found");
    }
    this->_path = path;
}

configParser::~configParser() {
}

configParser::configParser(const configParser &src) {
    *this = src;
}

configParser &configParser::operator=(const configParser &src) {
    if (this != &src) {
        this->_path = src._path;
    }
    return *this;
}

const serverConfig& configParser::getServerConfig() const {
    return _server;
}

void    configParser::config_error(const std::string &message) {
    std::cerr << "Error: " << message << std::endl;
}

void    configParser::validate_braces(std::string line) {
        if (line.find('{') != std::string::npos) {
            this->_braceStack.push('{');
    }
    else if (line.find('}') != std::string::npos) {
        if (_braceStack.empty()) {
            throw ConfigParserException("Mismatched closing brace '}' ");
        } else {
            _braceStack.pop();
        }
    }
}

void    configParser::read_and_parse_config() {

    std::ifstream configFile(this->_path);

    if (configFile.is_open()) {
        std::string line;
        while (std::getline(configFile, line)) {
            utility::stringTrim(line, " \t\n\r\f\v");
            validate_braces(line);
            process_line(line);
        }
        if (!this->_braceStack.empty()) {
            throw std::runtime_error("Mismatched opening brace '{'");
        }
    }
    else {
        throw ConfigParserException("Failed to open configuration file");
    }
}

void    configParser::process_line(std::string line) {

}
