#pragma once

#include "component/component.h"
#include "utils/color_difference/color_difference_hsv.h"
#include "utils/color_difference/color_difference_rgb.h"

namespace glamorous {

class SimilarPic5colorComponent : public Component {
private:
	const double weight[5] = {1,1,1,1,1};
public:
	SimilarPic5colorComponent(std::string name = "SimilarPic5colorComponent");
	virtual ~SimilarPic5colorComponent() = default;
	
protected:
	void process(RequestStatusPointer request) override;
};

} // namespace glamorous
