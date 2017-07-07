#pragma once

#include "component.h"

namespace glamorous {

class ComponentFactory {
public:
    virtual ComponentPointer create() = 0;
};

class SimpleComponentFactory : ComponentFactory {
public:
    ComponentPointer create() override;
};

} // namespace glamorous
