#include "idle_component.h"
#include "component/component_factory.h"
#include "log.h"

using namespace glamorous;

ComponentPointer IdleComponentFactory::create(std::string name) {
    return std::make_shared<IdleComponent>(name);
}

IdleComponent::IdleComponent(std::string name) : Component(name) {}

void IdleComponent::process(RequestStatusPointer request) {
    
}
