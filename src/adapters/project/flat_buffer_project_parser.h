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

#include <memory>
#include <vector>

#include "domain/project/model/project.h"
#include "domain/common/ilogger_factory.h"
#include "domain/common/ilogger.h"
#include "flatbuffers/flatbuffers.h"
#include "adapters/serialization/flatbuffers/project_generated.h"

namespace project {
    struct FbProjectNode {
        flatbuffers::Offset<void> nodeOffset;
        plant_composer::fbs::ProjectNode type;
    };

    class FlatBufferProjectParser {
    public:
        explicit FlatBufferProjectParser(common::ILoggerFactory *loggerFactory);

        FbProjectNode serializeProjectNode(flatbuffers::FlatBufferBuilder &builder, ProjectNode *domainNode);

        void parserFlatBufferProjectNodes(
            const flatbuffers::Vector<flatbuffers::Offset<void> > *nodes,
            const flatbuffers::Vector<u_int8_t> *types,
            NodeContainer *parent
        );

        flatbuffers::Offset<plant_composer::fbs::ProjectCategory> serializeProjectCategory(
            flatbuffers::FlatBufferBuilder &builder,
            const ProjectCategory *category
        );

        std::unique_ptr<ProjectCategory> parserFlatBufferProjectCategory(
            const plant_composer::fbs::ProjectCategory *fbProjectCategory
        );

        std::optional<std::unique_ptr<Project> > parse(const std::vector<char> &buffer, const std::string &path);

        flatbuffers::DetachedBuffer serialize(const Project &project);

    private:
        std::unique_ptr<common::Ilogger> logger_;
    };
}
