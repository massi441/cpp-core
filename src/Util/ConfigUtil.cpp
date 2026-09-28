#ifndef NN_SWITCH

#include "Core/Util/ConfigUtil.h"

#include <filesystem>
#include <fstream>

namespace ml {

bool parseConfig(const std::filesystem::path& path, const std::function<void(const std::string& key, const std::string& value)>& kvParser, char kvDelimiter) {
    std::ifstream file(path);

    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t delimiterIt = line.find(kvDelimiter);

        if (delimiterIt == std::string::npos) {
            continue;
        }

        std::string key = line.substr(0, delimiterIt);
        std::string value = line.substr(delimiterIt + 1);

        kvParser(key, value);
    }

    return true;
}

}

#endif
