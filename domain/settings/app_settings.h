#pragma once
#include <string>

class AppSettings {
public:
    virtual ~AppSettings() = default;

    virtual std::string getAssetsDir() = 0;

    virtual void setAssetsDir(std::string assetsDir) = 0;

    virtual std::string getComponentsLibraryDir() = 0;

    virtual std::string getComponentIconPath() = 0;
};
