#pragma once

#include "component/component.h"

namespace glamorous {

class IdleComponent : public Component {
public:
    IdleComponent(std::string name = "IdleComponent");
    virtual ~IdleComponent() = default;

protected:
    void process(RequestStatusPointer request) override;
};

} // namespace glamorous
