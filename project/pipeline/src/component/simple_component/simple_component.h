#pragma once

#include "component/component.h"

namespace glamorous {

class SimpleComponent : public Component {
public:
    SimpleComponent(std::string name = "SimpleComponent");
    virtual ~SimpleComponent() = default;

protected:
    void process(RequestStatusPointer request) override;
};

} // namespace glamorous
