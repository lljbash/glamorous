#include "ink_transfer_component.h"
#include "component/component_factory.h"
#include "ink_dispersion.h"
#include "log.h"

using namespace glamorous;

ComponentPointer InkTransferComponentFactory::create(std::string name) {
    return std::make_shared<InkTransferComponent>(name);
}

InkTransferComponent::InkTransferComponent(std::string name)
    : Component(name) {}

void InkTransferComponent::process(RequestStatusPointer request) {
    InkDispersion id;
    cv::imwrite("lajitongtong.jpg", request->res_img);
    id.initialize(request->res_img);
    id.apply();
    cv::Mat dst = id.get_dst();
//    cv::cvtColor(dst, dst, CV_BGR2GRAY);
    request->res_img = dst.clone();
}
