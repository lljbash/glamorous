#pragma once

#include "component/component.h"

namespace glamorous {

class InkVideoComponent : public Component {
public:
    InkVideoComponent(std::string name = "InkVideoComponent");
    virtual ~InkVideoComponent() = default;
    
protected:
    void process(RequestStatusPointer request) override;
};

} // namespace glamorous
