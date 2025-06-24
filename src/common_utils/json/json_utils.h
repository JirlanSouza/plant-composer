#pragma once
#include <filesystem>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
using json = nlohmann::json;

json jsonFromFile(const fs::path &path);
