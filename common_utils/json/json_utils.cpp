#include "json_utils.h"
#include <fstream>

json jsonFromFile(const fs::path &path) {
    std::ifstream file(path);
    return json::parse(file);
}
