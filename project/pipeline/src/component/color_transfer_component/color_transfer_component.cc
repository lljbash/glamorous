#include "color_transfer_component.h"
#include "component/component_factory.h"
#include "color_transfer.h"
#include "utils/color_difference/color_difference_hsv.h"
#include "log.h"

using namespace glamorous;

ColorTransferComponentFactory::ColorTransferComponentFactory(float level)
    : level_(level) {}

ComponentPointer ColorTransferComponentFactory::create(std::string name) {
    return std::make_shared<ColorTransferComponent>(level_, name);
}

ColorTransferComponent::ColorTransferComponent(float level, std::string name)
    : Component(name), level_(level) {}

void ColorTransferComponent::process(RequestStatusPointer request) {
    cv::Mat image = request->res_img;
    ColorTransfer ct(new ColorDifferenceHSV());
    cv::Mat c = cv::Mat::zeros(1, ColorTransfer::NUM_COLOR, CV_8UC3);
    for (int i = 0; i < ColorTransfer::NUM_COLOR; i++) {
        for (int j = 0; j < 3; j++) {
            c.ptr<cv::Vec3b>()[i][j] = request->ref_attr.five_color[i][j];
        }
    }
    ct.initialize(image, c);
    ct.apply(level_);
    request->res_img = ct.get_dst();
}
