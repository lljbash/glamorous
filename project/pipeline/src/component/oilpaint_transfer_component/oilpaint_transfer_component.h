#pragma once

#include "component/component.h"

namespace glamorous {

class OilpaintTransferComponent : public Component {
public:
    OilpaintTransferComponent(std::string name = "OilpaintTransferComponent");
    virtual ~OilpaintTransferComponent() = default;

protected:
    void process(RequestStatusPointer request) override;
};

} // namespace glamorous
