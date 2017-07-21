#pragma once

#include "component/component.h"

namespace glamorous {

class StyleTransferComponent : public Component {
public:
    StyleTransferComponent(std::string name = "StyleTransferComponent");
    virtual ~StyleTransferComponent() = default;

protected:
    void process(RequestStatusPointer request) override;
};

} // namespace glamorous
