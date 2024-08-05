#include "JsonLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

JsonLoader::JsonLoader(const std::string& filename) : filename(filename) {}

void JsonLoader::load() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string jsonStr = buffer.str();

    document.Parse(jsonStr.c_str());
    if (document.HasParseError()) {
        throw std::runtime_error("JSON parse error");
    }
}

const rapidjson::Document& JsonLoader::getDocument() const {
    return document;
}
