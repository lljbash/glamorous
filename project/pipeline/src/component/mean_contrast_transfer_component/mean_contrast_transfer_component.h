#pragma once

#include "component/component.h"

namespace glamorous {

class MeanContrastTransferComponent : public Component {
public:
    MeanContrastTransferComponent(std::string name = "MeanContrastTransferComponent");
    virtual ~MeanContrastTransferComponent() = default;

protected:
    void process(RequestStatusPointer request) override;
};

} // namespace glamorous
