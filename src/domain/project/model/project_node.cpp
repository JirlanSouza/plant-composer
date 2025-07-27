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

#include "project_node.h"

#include <ranges>
#include <utility>

namespace project {
    ProjectNode::ProjectNode(const NodeType type, std::string id, std::string name, NodeContainer *parent)
        : type_(type), id_(std::move(id)), name_(std::move(name)), parent_(parent) {
    }

    NodeType ProjectNode::getType() const { return type_; }

    const std::string &ProjectNode::getId() const { return id_; }

    const std::string &ProjectNode::getName() const { return name_; }

    void ProjectNode::rename(std::string newName) {
        if (newName.empty()) {
            return;
        }
        name_ = std::move(newName);
    }

    NodeContainer *ProjectNode::getParent() const { return parent_; }

    bool ProjectNode::isFolder() const { return type_ == NodeType::FOLDER; }

    bool ProjectNode::isFile() const { return type_ == NodeType::FILE; }

    bool ProjectNode::canBeCopied() const { return true; }

    bool ProjectNode::canBeMoved() const { return true; }

    bool ProjectNode::canBeRemoved() const { return true; }

    bool ProjectNode::canBeRenamed() const { return true; }


    FileNode::FileNode(std::string id, NodeContainer *parent, std::string name, std::string filePath)
        : ProjectNode(NodeType::FILE, std::move(id), std::move(name), parent),
        filePath_(std::move(filePath)) {
    }

    const std::string &FileNode::getFilePath() const {
        return filePath_;
    }

    std::optional<NodeContainer *> FileNode::getAsFolder() {
        return std::nullopt;
    }

    std::optional<FileNode *> FileNode::getAsFile() {
        return this;
    }


    NodeContainer::NodeContainer(std::string id, NodeContainer *parent, std::string name)
        : ProjectNode(NodeType::FOLDER, std::move(id), std::move(name), parent) {
    }

    void NodeContainer::addChild(std::unique_ptr<ProjectNode> item) {
        if (item == nullptr) {
            return;
        }

        item->setParent(this);
        children_[item->getId()] = std::move(item);
    }

    void NodeContainer::removeChild(const std::string &id) {
        children_.erase(id);
    }

    std::unique_ptr<ProjectNode> NodeContainer::releaseChild(const std::string &id) {
        if (!children_.contains(id)) {
            return nullptr;
        }

        auto node = std::move(children_[id]);
        children_.erase(id);
        return node;
    }

    std::vector<ProjectNode *> NodeContainer::getChildren() const {
        std::vector<ProjectNode *> items;
        for (const auto &child: children_ | std::views::values) {
            items.push_back(child.get());
        }
        return items;
    }

    std::optional<NodeContainer *> NodeContainer::getAsFolder() {
        return this;
    }

    std::optional<FileNode *> NodeContainer::getAsFile() {
        return std::nullopt;
    }

    std::optional<NodeContainer *> NodeContainer::getFolder(const std::string &folderId) {
        if (children_.contains(folderId) && children_.at(folderId)->isFolder()) {
            return static_cast<NodeContainer *>(children_.at(folderId).get());
        }

        for (const std::unique_ptr<ProjectNode> &child: children_ | std::views::values) {
            if (child->isFile()) {
                continue;
            }

            auto folderOpt = child->getAsFolder();

            if (!folderOpt.has_value()) {
                continue;
            }

            const auto folder = folderOpt.value()->getFolder(folderId);

            if (folder.has_value()) {
                return folder;
            }
        }

        return std::nullopt;
    }

    std::optional<FileNode *> NodeContainer::getFile(const std::string &fileId) {
        if (children_.contains(fileId) && children_.at(fileId)->isFile()) {
            return static_cast<FileNode *>(children_.at(fileId).get());
        }

        for (const std::unique_ptr<ProjectNode> &child: children_ | std::views::values) {
            if (child->isFile()) {
                continue;
            }

            auto folderOpt = child->getAsFolder();

            if (!folderOpt.has_value()) {
                continue;
            }

            const auto file = folderOpt.value()->getFile(fileId);

            if (file.has_value()) {
                return file;
            }
        }

        return std::nullopt;
    }


    ProjectCategory::ProjectCategory(std::string id, std::string name, std::string folderName)
        : NodeContainer(std::move(id), nullptr, std::move(name)), folderName_(std::move(folderName)) {
    }

    const std::string &ProjectCategory::getFolderName() const {
        return folderName_;
    }

    bool ProjectCategory::canBeCopied() const { return false; }

    bool ProjectCategory::canBeMoved() const { return false; }

    bool ProjectCategory::canBeRemoved() const { return false; }

    bool ProjectCategory::canBeRenamed() const { return false; }
}
