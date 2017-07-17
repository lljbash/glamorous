#include "word2image_component.h"
#include "component/component_factory.h"
#include "log.h"

using namespace glamorous;

ComponentPointer Word2ImageComponentFactory::create(std::string name) {
    return std::make_shared<Word2ImageComponent>(name);
}

Word2ImageComponent::Word2ImageComponent(std::string name)
    : Component(name) {}

void Word2ImageComponent::process(RequestStatusPointer request) {
    
}
