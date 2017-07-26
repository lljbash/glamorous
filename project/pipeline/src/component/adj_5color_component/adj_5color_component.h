#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "component/component.h"

namespace glamorous {

class Adj5colorComponent : public Component {
public:
    Adj5colorComponent(std::string name = "Adj5colorComponent");
    virtual ~Adj5colorComponent() = default;
	
protected:
    void process(RequestStatusPointer request) override;

private:
    static const std::unordered_map<std::string,std::vector<unsigned char>> map_5c_;
};

} // namespace glamorous
