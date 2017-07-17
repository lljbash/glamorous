#pragma once

#include "component/component.h"

namespace glamorous {

class InkTransferComponent : public Component {
public:
    InkTransferComponent(std::string name = "InkTransferComponent");
    virtual ~InkTransferComponent() = default;

protected:
    void process(RequestStatusPointer request) override;
};

} // namespace glamorous
