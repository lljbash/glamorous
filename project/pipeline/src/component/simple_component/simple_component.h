#pragma once

#include "component/component.h"

namespace glamorous {

class SimpleComponent : public Component {
public:
    friend class SimpleComponentFactory;

protected:
    void process(RequestStatusPointer request) override;
};

} // namespace glamorous
