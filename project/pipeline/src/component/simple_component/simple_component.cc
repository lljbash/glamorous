#include "simple_component.h"
#include "component/component_factory.h"
#include "log.h"

using namespace glamorous;

ComponentPointer SimpleComponentFactory::create(std::string name) {
    return std::make_shared<SimpleComponent>(name);
}

SimpleComponent::SimpleComponent(std::string name) : Component(name) {}

void SimpleComponent::process(RequestStatusPointer request) {
    Log::info("SimpleComponent is processing, but it's just an example and will do nothing.");
}
