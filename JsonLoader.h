#ifndef JSONLOADER_H
#define JSONLOADER_H

#include <string>
#include <rapidjson/document.h>

class JsonLoader {
public:
    explicit JsonLoader(const std::string& filename);
    void load();
    const rapidjson::Document& getDocument() const;

private:
    std::string filename;
    rapidjson::Document document;
};

#endif // JSONLOADER_H
