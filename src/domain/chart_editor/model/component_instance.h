#pragma once
#include <unordered_map>

#include "graphic_item_position.h"
#include "../../components_library/model/component.h"

namespace dcl = domain::components_library;

namespace domain::chart_editor {
    class ComponentInstance {
    public:
        ComponentInstance(
            std::string id,
            std::string name,
            std::string typeId,
            GraphicItemPosition position,
            std::shared_ptr<dcl::Component> definition,
            std::string assetsPath
        );

        [[nodiscard]] std::shared_ptr<dcl::Component> getDefinition() const { return definition_; }

        [[nodiscard]] std::string getId() const { return id_; }

        [[nodiscard]] std::string getName() const { return name_; }

        [[nodiscard]] std::string getTypeId() const { return componentTypeId_; }

        [[nodiscard]] GraphicItemPosition getPosition() const { return position_; }

        [[nodiscard]] std::string getSymbolPath() const { return assetsPath_ + "/" + definition_->symbolPath; };


        void rename(const std::string &newName);

        void setConstantValue(const std::string &name, const std::string &value);

    private:
        std::string id_;
        std::string name_;
        std::string componentTypeId_;
        GraphicItemPosition position_;
        const std::shared_ptr<dcl::Component> definition_;
        std::string assetsPath_;

        std::unordered_map<std::string, std::string> constants;
    };
}
