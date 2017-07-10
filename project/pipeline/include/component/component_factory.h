#pragma once

#include "component.h"

namespace glamorous {

class ComponentFactory {
public:
    virtual ComponentPointer create(std::string name) = 0;
};

class SimpleComponentFactory : ComponentFactory {
public:
    ComponentPointer create(std::string name = "SimpleComponent") override;
};

} // namespace glamorous
