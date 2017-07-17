#pragma once

#include "component/component.h"

namespace glamorous {

class DatabaseMatchComponent : public Component {
public:
    DatabaseMatchComponent(std::string root_dir, std::string name = "DatabaseMatchComponent");
    virtual ~DatabaseMatchComponent() = default;

protected:
    void process(RequestStatusPointer request) override;
    
private:
    std::string root_dir_;
};

} // namespace glamorous
