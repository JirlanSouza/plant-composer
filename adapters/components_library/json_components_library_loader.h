#pragma once
#include "../../domain/components_library/components_library_loader.h"


class JsonComponentsLibraryLoader final : public ComponentsLibraryLoader {
public:
    explicit JsonComponentsLibraryLoader(const std::string &librariesDir);

    [[nodiscard]] std::vector<Library> loadLibraries() override;

private:
    const std::string LIB_EXTENSION = ".json";
    fs::path librariesDir_;

    [[nodiscard]] std::vector<fs::path> listLibrariesPaths() const;

    static Library loadLibraryFromPath(fs::path &libraryPath);
};
