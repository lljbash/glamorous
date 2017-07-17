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
    
    RequestStatusPointer rs = std::make_shared<RequestStatus>();
    
    rs->src_img = cv::imread("../../../b.jpg");
    rs->res_img = rs->src_img.clone();
    
    rs->request_type = RequestStatus::RequestType::Pointillism;

    Pipeline p;
    auto output = std::make_shared<Component::WaitQueue>();
    //p.set_static_pipeline({cp_dm, cp_ce, cp_5c, cp_hsv, cp_mc, cp_oil});
    p.set_static_pipeline({cp_ink});
    p.start_serial(output, {rs});
    RequestStatusPointer rso;
    output->wait_and_pop(rso);
    
    cv::imwrite("res.jpg", rso->res_img);
    cv::imwrite("color_ref.jpg", rso->ref_img);
    return 0;
}
