#include "simple_component.h"
#include "component/component_factory.h"
#include "log.h"

using namespace glamorous;

ComponentPointer SimpleComponentFactory::create() {
    return std::make_shared<SimpleComponent>();
}

void SimpleComponent::process(RequestStatusPointer request) {
    Log::info("SimpleComponent is processing, but it's just an example and will do nothing.");
}
