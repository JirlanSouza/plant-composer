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

#include "domain/common/id_factory.h"
#include "domain/common/ilogger_factory.h"
#include "flat_buffer_project_parser.h"

namespace project {
    class FlatBufferProjectLoader : public IProjectLoader {
    public:
        FlatBufferProjectLoader(common::ILoggerFactory *loggerFactory, common::IDFactory *idFactory);

        std::optional<std::unique_ptr<Project> > createNewProject(
            const std::string &name,
            const std::string &description,
            const std::string &author,
            const std::string &parentDirectory
        ) override;

        std::optional<std::unique_ptr<Project> > loadProject(const std::string &path) override;

        void saveProject(const Project &project) override;

        std::optional<std::unique_ptr<diagram::Diagram> > loadDiagram(const DiagramMetadata &metadata) override;

        void saveDiagram(const DiagramMetadata &metadata, const diagram::Diagram &diagram) override;

    private:
        std::unique_ptr<common::Ilogger> logger_;
        common::IDFactory *idFactory_;
        std::unique_ptr<FlatBufferProjectParser> parser_;
    };
}
