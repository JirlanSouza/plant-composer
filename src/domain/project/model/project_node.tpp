#include "project_node.h"

#include <ranges>
#include <utility>

namespace domain::project {
    template<typename T>
    ProjectNode<T>::ProjectNode(const NodeType type, std::string id, std::string name, NodeContainer<T> *parent)
        : type_(type), id_(std::move(id)), name_(std::move(name)), parent_(parent) {
    }

    template<typename T>
    NodeType ProjectNode<T>::getType() const { return type_; }

    template<typename T>
    const std::string &ProjectNode<T>::getId() const { return id_; }

    template<typename T>
    const std::string &ProjectNode<T>::getName() const { return name_; }

    template<typename T>
    void ProjectNode<T>::rename(std::string newName) {
        if (newName.empty()) {
            return;
        }
        name_ = std::move(newName);
    }

    template<typename T>
    NodeContainer<T> *ProjectNode<T>::getParent() const { return parent_; }

    template<typename T>
    bool ProjectNode<T>::isFolder() const { return type_ == NodeType::FOLDER; }

    template<typename T>
    bool ProjectNode<T>::isFile() const { return type_ == NodeType::FILE; }

    template<typename T>
    bool ProjectNode<T>::canBeCopied() const { return true; }

    template<typename T>
    bool ProjectNode<T>::canBeMoved() const { return true; }

    template<typename T>
    bool ProjectNode<T>::canBeRemoved() const { return true; }

    template<typename T>
    bool ProjectNode<T>::canBeRenamed() const { return true; }


    template<typename T>
    FileNode<T>::FileNode(std::string id, NodeContainer<T> *parent, std::string name, std::string filePath)
        : ProjectNode<T>(NodeType::FILE, std::move(id), std::move(name), parent),
        filePath_(std::move(filePath)) {
    }

    template<typename T>
    const std::string &FileNode<T>::getFilePath() const {
        return filePath_;
    }

    template<typename T>
    std::optional<NodeContainer<T> *> FileNode<T>::getAsFolder() {
        return std::nullopt;
    }

    template<typename T>
    std::optional<T *> FileNode<T>::getAsFile() {
        return static_cast<T *>(this);
    }


    template<typename T>
    NodeContainer<T>::NodeContainer(std::string id, NodeContainer<T> *parent, std::string name)
        : ProjectNode<T>(NodeType::FOLDER, std::move(id), std::move(name), parent) {
    }

    template<typename T>
    void NodeContainer<T>::addChild(std::unique_ptr<ProjectNode<T> > item) {
        if (item == nullptr || item->getParent() != this) {
            return;
        }

        children_[item->getId()] = std::move(item);
    }

    template<typename T>
    void NodeContainer<T>::removeChild(const std::string &id) {
        children_.erase(id);
    }

    template<typename T>
    std::vector<ProjectNode<T> *> NodeContainer<T>::getChildren() const {
        std::vector<ProjectNode<T> *> items;
        for (const auto &child: children_ | std::views::values) {
            items.push_back(child.get());
        }
        return items;
    }

    template<typename T>
    std::optional<NodeContainer<T> *> NodeContainer<T>::getAsFolder() {
        return this;
    }

    template<typename T>
    std::optional<T *> NodeContainer<T>::getAsFile() {
        return std::nullopt;
    }

    template<typename T>
    std::optional<NodeContainer<T> *> NodeContainer<T>::getFolder(const std::string &folderId) {
        if (children_.contains(folderId) && children_.at(folderId)->isFolder()) {
            return static_cast<NodeContainer<T> *>(children_.at(folderId).get());
        }

        for (const std::unique_ptr<ProjectNode<T> > &child: children_ | std::views::values) {
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

    template<typename T>
    std::optional<T *> NodeContainer<T>::getFile(const std::string &fileId) {
        if (children_.contains(fileId) && children_.at(fileId)->isFile()) {
            return static_cast<T *>(children_.at(fileId).get());
        }

        for (const std::unique_ptr<ProjectNode<T> > &child: children_ | std::views::values) {
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


    template<typename T>
    ProjectCategory<T>::ProjectCategory(std::string id, std::string name, std::string folderName)
        : NodeContainer<T>(std::move(id), nullptr, std::move(name)), folderName_(std::move(folderName)) {
    }

    template<typename T>
    const std::string &ProjectCategory<T>::getFolderName() const {
        return folderName_;
    }

    template<typename T>
    bool ProjectCategory<T>::canBeCopied() const { return false; }

    template<typename T>
    bool ProjectCategory<T>::canBeMoved() const { return false; }

    template<typename T>
    bool ProjectCategory<T>::canBeRemoved() const { return false; }

    template<typename T>
    bool ProjectCategory<T>::canBeRenamed() const { return false; }
}
