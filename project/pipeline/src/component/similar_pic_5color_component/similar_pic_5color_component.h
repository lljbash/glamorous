#pragma once

#include "component/component.h"

namespace glamorous {

class SimilarPic5colorComponent : public Component
{
public:
	SimilarPic5colorComponent(std::string name = "SimilarPic5colorComponent");
	virtual ~SimilarPic5colorComponent() = default;
	
protected:
	void process(RequestStatusPointer request) override;
};

} // namespace glamorous
