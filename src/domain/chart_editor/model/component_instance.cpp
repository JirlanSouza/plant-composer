#include "component_instance.h"

namespace domain::chart_editor {
    ComponentInstance::ComponentInstance(std::string id, std::string name, std::string typeId,
                                         GraphicItemPosition position,
                                         std::shared_ptr<dcl::Component> definition,
                                         std::string assetsPath): id_(id),
                                                                  name_(name),
                                                                  componentTypeId_(typeId),
                                                                  position_(position),
                                                                  definition_(definition),
                                                                  assetsPath_(assetsPath) {
    }

    void ComponentInstance::rename(const std::string &newName) {
        name_ = newName;
    }


    void ComponentInstance::setConstantValue(const std::string &name, const std::string &value) {
        const auto it = std::find_if(definition_->constants.begin(), definition_->constants.end(),
                                     [&](const dcl::Constant &constant) {
                                         return constant.name == name;
                                     });

        if (it != definition_->constants.end()) {
            constants[name] = value;
        }
    }
}
