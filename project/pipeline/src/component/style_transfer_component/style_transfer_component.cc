#include "style_transfer_component.h"
#include "component/component_factory.h"
#include "log.h"

using namespace glamorous;

ComponentPointer StyleTransferComponentFactory::create(std::string name) {
    return std::make_shared<StyleTransferComponent>(name);
}

StyleTransferComponent::StyleTransferComponent(std::string name) : Component(name) {}

void StyleTransferComponent::process(RequestStatusPointer request) {
    
}
