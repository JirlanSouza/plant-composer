#include "component_instance.h"

namespace domain::diagram {
    ComponentInstance::ComponentInstance(
        std::string id,
        std::string name,
        std::string typeId,
        GraphicItemPosition position,
        const dcl::Component *component,
        std::string assetsPath
    ): id_(id),
        name_(name),
        componentTypeId_(typeId),
        position_(position),
        component_(component),
        assetsPath_(assetsPath) {
    }

    void ComponentInstance::rename(const std::string &newName) {
        name_ = newName;
    }

    void ComponentInstance::setPosition(const GraphicItemPosition &newPosition) {
        position_ = newPosition;
    }


    void ComponentInstance::setConstantValue(const std::string &name, const std::string &value) {
        const auto it = std::ranges::find_if(
            component_->constants,
            [&](const dcl::Constant &constant) {
                return constant.name == name;
            }
        );

        if (it != component_->constants.end()) {
            constants[name] = value;
        }
    }
}
