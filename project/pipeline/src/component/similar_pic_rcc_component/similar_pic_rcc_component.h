#pragma once

#include "component/component.h"

namespace glamorous {

class SimilarPicRccComponent : public Component {
public:
    SimilarPicRccComponent(std::string name = "SimilarPicRccComponent");
    virtual ~SimilarPicRccComponent() = default;

protected:
    void process(RequestStatusPointer request) override;
};

} // namespace glamorous
