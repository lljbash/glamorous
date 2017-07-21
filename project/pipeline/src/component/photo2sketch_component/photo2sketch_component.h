#pragma once

#include "component/component.h"

namespace glamorous {

class Photo2SketchComponent : public Component {
public:
    Photo2SketchComponent(std::string name = "Photo2SketchComponent");
    virtual ~Photo2SketchComponent() = default;

protected:
    void process(RequestStatusPointer request) override;
};

} // namespace glamorous
