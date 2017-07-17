#pragma once

#include "component/component.h"

namespace glamorous {

class Word2ImageComponent : public Component {
public:
    Word2ImageComponent(std::string name = "Word2ImageComponent");
    virtual ~Word2ImageComponent() = default;

protected:
    void process(RequestStatusPointer request) override;
};

} // namespace glamorous
