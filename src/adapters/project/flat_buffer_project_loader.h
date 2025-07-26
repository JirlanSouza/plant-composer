/*
 * plant-composer
 * Copyright (c) 2025 jirlansouza
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "domain/project/project_loader.h"
#include "flat_buffer_project_parser.h"
#include "domain/shared/id_factory.h"
#include "domain/shared/ilogger.h"

namespace dp = domain::project;
namespace dd = domain::diagram;

namespace adapters::project {
    class FlatBufferProjectLoader : public dp::IProjectLoader {
    public:
        FlatBufferProjectLoader(domain::Ilogger *logger, domain::IDFactory *idFactory);

        std::optional<std::unique_ptr<dp::Project> > createNewProject(
            const std::string &name,
            const std::string &description,
            const std::string &author,
            const std::string &parentDirectory
        ) override;

        std::optional<std::unique_ptr<dp::Project> > loadProject(const std::string &path) override;

        void saveProject(const dp::Project &project) override;

        std::optional<std::unique_ptr<dd::Diagram> > loadDiagram(const dp::DiagramMetadata &metadata) override;

        void saveDiagram(const dp::DiagramMetadata &metadata, const dd::Diagram &diagram) override;

    private:
        domain::Ilogger *logger_;
        domain::IDFactory *idFactory_;
        std::unique_ptr<FlatBufferProjectParser> parser_;
    };
}
