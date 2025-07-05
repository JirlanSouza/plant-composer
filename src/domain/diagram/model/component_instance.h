#pragma once
#include <unordered_map>

#include "graphic_item_position.h"
#include "domain/components_library/model/component.h"

namespace dcl = domain::components_library;

namespace domain::diagram {
    class ComponentInstance {
    public:
        ComponentInstance(
            std::string id,
            std::string name,
            std::string typeId,
            GraphicItemPosition position,
            const dcl::Component *component,
            std::string assetsPath
        );

        [[nodiscard]] const dcl::Component *getDefinition() const { return component_; }

        [[nodiscard]] std::string getId() const { return id_; }

        [[nodiscard]] std::string getName() const { return name_; }

        [[nodiscard]] std::string getTypeId() const { return componentTypeId_; }

        [[nodiscard]] GraphicItemPosition getPosition() const { return position_; }

        [[nodiscard]] std::string getSymbolPath() const { return assetsPath_ + "/" + component_->symbolPath; };


        void rename(const std::string &newName);

        void setPosition(const GraphicItemPosition &newPosition);

        void setConstantValue(const std::string &name, const std::string &value);

    private:
        std::string id_;
        std::string name_;
        std::string componentTypeId_;
        GraphicItemPosition position_;
        const dcl::Component *component_;
        std::string assetsPath_;

        std::unordered_map<std::string, std::string> constants;
    };
}
