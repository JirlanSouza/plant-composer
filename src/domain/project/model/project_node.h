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

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <optional>

#include "domain/common/id_factory.h"
#include "domain/common/ilogger_factory.h"

namespace project {
    enum class NodeType {
        FOLDER, FILE
    };

    std::string toString(const NodeType &type);

    class NodeContainer;
    class FileNode;

    class ProjectNode {
    public:
        ProjectNode(NodeType type, std::string id, std::string name, NodeContainer *parent);

        virtual ~ProjectNode() = default;

        [[nodiscard]] NodeType getType() const;

        [[nodiscard]] const std::string &getId() const;

        [[nodiscard]] const std::string &getName() const;

        void rename(std::string newName);

        [[nodiscard]] NodeContainer *getParent() const;

        [[nodiscard]] bool isFolder() const;

        [[nodiscard]] bool isFile() const;

        [[nodiscard]] virtual bool canBeCopied() const;

        [[nodiscard]] virtual bool canBeMoved() const;

        [[nodiscard]] virtual bool canBeRemoved() const;

        [[nodiscard]] virtual bool canBeRenamed() const;

        [[nodiscard]] virtual ProjectNode *copy(common::IDFactory *idFactory) const = 0;

        virtual std::optional<NodeContainer *> getAsFolder() = 0;

        virtual std::optional<FileNode *> getAsFile() = 0;


        void setParent(NodeContainer *parent) { parent_ = parent; }

    private:
        NodeType type_;
        std::string id_;
        std::string name_;
        NodeContainer *parent_;
    };

    class FileNode : public ProjectNode {
    public:
        FileNode(std::string id, NodeContainer *parent, std::string name, std::string filePath);

        [[nodiscard]] const std::string &getFilePath() const;

        std::optional<NodeContainer *> getAsFolder() override;

        std::optional<FileNode *> getAsFile() override;

        [[nodiscard]] ProjectNode *copy(common::IDFactory *idFactory) const override;

    private:
        std::string filePath_;
    };

    class NodeContainer : public ProjectNode {
    public:
        explicit NodeContainer(std::string id, NodeContainer *parent, std::string name);

        void addChild(std::unique_ptr<ProjectNode> item);

        void removeChild(const std::string &id);

        std::unique_ptr<ProjectNode> releaseChild(const std::string &id);

        [[nodiscard]] std::vector<ProjectNode *> getChildren() const;

        std::optional<NodeContainer *> getFolder(const std::string &folderId);

        std::optional<FileNode *> getFile(const std::string &fileId);

        std::optional<NodeContainer *> getAsFolder() override;

        std::optional<FileNode *> getAsFile() override;

        [[nodiscard]] ProjectNode *copy(common::IDFactory *idFactory) const override;

        [[nodiscard]] std::optional<ProjectNode *> findNode(const std::string &id) const;

        [[nodiscard]] bool hasChildWithName(const std::string &name) const;

    private:
        std::unordered_map<std::string, std::unique_ptr<ProjectNode> > children_{};
    };

    class ProjectCategory : public NodeContainer {
    public:
        ProjectCategory(std::string id, std::string name, std::string folderName);

        [[nodiscard]] const std::string &getFolderName() const;

        [[nodiscard]] bool canBeCopied() const override;

        [[nodiscard]] bool canBeMoved() const override;

        [[nodiscard]] bool canBeRemoved() const override;

        [[nodiscard]] bool canBeRenamed() const override;

    private:
        std::string folderName_;;
    };
}
