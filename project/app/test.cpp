#include <opencv2/opencv.hpp>

#include "log.h"
#include "component/component_factory.h"
#include "pipeline.h"

using namespace glamorous;

int main(int argc, char const *argv[]) {
    Log::info("hello");
    
    ColorTransferComponentFactory ctcf(0.8);
    ComponentPointer cp_hsv = ctcf.create();
    MeanContrastTransferComponentFactory mctcf;
    ComponentPointer cp_mc = mctcf.create();
    OilpaintTransferComponentFactory otcf;
    ComponentPointer cp_oil = otcf.create();
    ColorAttributeExtractorComponentFactory caecf;
    ComponentPointer cp_ce = caecf.create();
    SimilarPic5colorComponentFactory sp5cf;
    ComponentPointer cp_5c = sp5cf.create();
    DatabaseMatchComponentFactory dmcf("../../../data");
    ComponentPointer cp_dm = dmcf.create();
    InkTransferComponentFactory itcf;
    ComponentPointer cp_ink = itcf.create();
    IdleComponentFactory icf;
    ComponentPointer cp_idle1 = icf.create("Idle1");
    ComponentPointer cp_idle2 = icf.create("Idle2");
    
    std::vector<ComponentPointer> cps = {cp_dm, cp_ce, cp_5c, cp_hsv, cp_mc, cp_idle1, cp_oil, cp_ink, cp_idle2};
    cp_dm->set_next_component(cp_ce);
    cp_ce->set_next_component(cp_5c);
    cp_5c->set_next_component(cp_hsv);
    cp_hsv->set_next_component(cp_mc);
    cp_mc->set_next_component(cp_idle1);
    cp_idle1->set_next_component_func([&cp_ink, &cp_oil](RequestStatusPointer rsp) {
        return rsp->request_type == RequestStatus::RequestType::Shuimo ? cp_ink : cp_oil;
    });
    cp_oil->set_next_component(cp_idle2);
    cp_ink->set_next_component(cp_idle2);
    
    RequestStatusPointer rs = std::make_shared<RequestStatus>();
    
    rs->src_img = cv::imread("../../../b.jpg");
    rs->res_img = rs->src_img.clone();
    
    rs->request_type = RequestStatus::RequestType::Shuimo;
    
    Pipeline p;
    auto output = std::make_shared<Component::WaitQueue>();
    p.set_custom_pipeline(cps);
    p.start_serial(output, {rs});
    RequestStatusPointer rso;
    output->wait_and_pop(rso);
    
    cv::imwrite("res.jpg", rso->res_img);
    cv::imwrite("color_ref.jpg", rso->ref_img);
    return 0;
}
