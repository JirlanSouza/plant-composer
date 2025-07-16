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

namespace domain::project {
    enum class NodeType {
        FOLDER, FILE
    };

    template<typename T>
    class NodeContainer;

    template<typename T>
    class ProjectNode {
    public:
        ProjectNode(NodeType type, std::string id, std::string name, NodeContainer<T> *parent);

        virtual ~ProjectNode() = default;

        [[nodiscard]] NodeType getType() const;

        [[nodiscard]] const std::string &getId() const;

        [[nodiscard]] const std::string &getName() const;

        void rename(std::string newName);

        [[nodiscard]] NodeContainer<T> *getParent() const;

        [[nodiscard]] bool isFolder() const;

        [[nodiscard]] bool isFile() const;

        virtual std::optional<NodeContainer<T> *> getAsFolder() = 0;

        virtual std::optional<T *> getAsFile() = 0;

    private:
        NodeType type_;
        std::string id_;
        std::string name_;
        NodeContainer<T> *parent_;
    };

    template<typename T>
    concept DerivedFromProjectNode = std::is_base_of_v<ProjectNode<T>, T>;

    template<typename T>
    class NodeContainer : public ProjectNode<T> {
    public:
        explicit NodeContainer(std::string id, NodeContainer<T> *parent, std::string name);

        void addChild(std::unique_ptr<ProjectNode<T> > item);

        void removeChild(const std::string &id);

        [[nodiscard]] std::vector<ProjectNode<T> *> getChildren() const;

        std::optional<NodeContainer<T> *> getFolder(const std::string &folderId);

        std::optional<T *> getFile(const std::string &fileId);

        std::optional<NodeContainer<T> *> getAsFolder() override;

        std::optional<T *> getAsFile() override;

    protected:

    private:
        std::unordered_map<std::string, std::unique_ptr<ProjectNode<T> > > children_{};
    };

    template<typename T>
    class ProjectCategory : public NodeContainer<T> {
    public:
        ProjectCategory<T>(std::string id, std::string name);
    };
}

#include "project_node.tpp"
