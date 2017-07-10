#pragma once

#include "component/component.h"

namespace glamorous {

class ColorTransferComponent : public Component {
public:
    ColorTransferComponent(float level, std::string name = "ColorTransferComponent");
    virtual ~ColorTransferComponent() = default;

protected:
    void process(RequestStatusPointer request) override;

private:
    float level_;
};

} // namespace glamorous
