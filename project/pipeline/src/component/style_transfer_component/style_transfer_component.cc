#include "style_transfer_component.h"
#include "component/component_factory.h"
#include "log.h"
#include "style_trans.h"

using namespace glamorous;

ComponentPointer StyleTransferComponentFactory::create(std::string name) {
    return std::make_shared<StyleTransferComponent>(name);
}

StyleTransferComponent::StyleTransferComponent(std::string name) : Component(name) {}

void StyleTransferComponent::process(RequestStatusPointer request) {
    StyleTrans st(request->id);
    st.setAlpha(0.3);
    st.initialize(request->res_img, request->ref_img);
    st.apply();
    cv::Mat dst = st.get_dst();
    cv::resize(dst, dst, request->res_img.size());
    request->res_img = dst.clone();
}
