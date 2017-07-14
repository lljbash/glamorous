#include "oilpaint_transfer_component.h"
#include "component/component_factory.h"
#include "log.h"
#include "oilpaint_transfer.h"

using namespace glamorous;

ComponentPointer OilpaintTransferComponentFactory::create(std::string name) {
    return std::make_shared<OilpaintTransferComponent>(name);
}

OilpaintTransferComponent::OilpaintTransferComponent(std::string name) : Component(name) {}

void OilpaintTransferComponent::process(RequestStatusPointer request) {
    cv::Mat dst = oilpaint::oilpaint_transfer(request->res_img);
    request->res_img = dst.clone();
}

