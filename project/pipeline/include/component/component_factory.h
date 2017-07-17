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

class ColorTransferComponentFactory : ComponentFactory {
public:
    ColorTransferComponentFactory(float level = 0.4);
    ComponentPointer create(std::string name = "ColorTransferComponent") override;
private:
    float level_;
};

class MeanContrastTransferComponentFactory : ComponentFactory {
public:
    ComponentPointer create(std::string name = "MeanContrastTransferComponent") override;
};

} // namespace glamorous
