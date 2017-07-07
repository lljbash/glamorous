#pragma once

#include "component/component.h"

namespace glamorous {

class SimpleComponent : public Component {
protected:
    void process(RequestStatusPointer request) override;
};

} // namespace glamorous
