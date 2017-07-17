#include "ink_transfer_component.h"
#include "component/component_factory.h"
#include "log.h"

using namespace glamorous;

ComponentPointer InkTransferComponentFactory::create(std::string name) {
    return std::make_shared<InkTransferComponent>(name);
}

InkTransferComponent::InkTransferComponent(std::string name)
    : Component(name) {}

void InkTransferComponent::process(RequestStatusPointer request) {
    cv::Mat res;
    cv::cvtColor(request->res_img, res, CV_BGR2GRAY);
    
}
