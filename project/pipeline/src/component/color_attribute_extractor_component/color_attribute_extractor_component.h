#pragma once

#include "component/component.h"

namespace glamorous {

class ColorAttributeExtractorComponent : public Component {
public:
    ColorAttributeExtractorComponent(std::string name = "ColorAttributeExtractorComponent");
    virtual ~ColorAttributeExtractorComponent() = default;

protected:
    void process(RequestStatusPointer request) override;
};

} // namespace glamorous
