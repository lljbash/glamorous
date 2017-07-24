#pragma once

#include "component/component.h"

namespace glamorous {

class SketchVideoComponent : public Component {
public:
    SketchVideoComponent(std::string name = "SketchVideoComponent");
    virtual ~SketchVideoComponent() = default;

protected:
    void process(RequestStatusPointer request) override;
};

} // namespace glamorous
