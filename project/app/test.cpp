#include <opencv2/opencv.hpp>

#include "log.h"
#include "component/component_factory.h"
#include "pipeline.h"

using namespace glamorous;

int main(int argc, char const *argv[]) {
    Log::info("hello");
    
    ColorTransferComponentFactory ctcf(0.8);
    ComponentPointer cp = ctcf.create();
    MeanContrastTransferComponentFactory mctcf;
    ComponentPointer cp2 = mctcf.create();
    OilpaintTransferComponentFactory otcf;
    ComponentPointer cp_oil = otcf.create();
    ColorAttributeExtractorComponentFactory caecf;
    ComponentPointer cp_ce = caecf.create();
    SimilarPic5colorComponentFactory sp5cf;
    ComponentPointer cp_5c = sp5cf.create();
    DatabaseMatchComponentFactory dmcf("../../../../data");
    ComponentPointer cp_dm = dmcf.create();
    
    RequestStatusPointer rs = std::make_shared<RequestStatus>();
    
    rs->src_img = cv::imread("../../lab/color/a.jpg");
    rs->res_img = rs->src_img.clone();
    
    rs->request_type = RequestStatus::RequestType::PostImpressionism;

    Pipeline p;
    auto output = std::make_shared<Component::WaitQueue>();
    p.set_static_pipeline({cp_dm});
    p.start_serial({rs}, output);
    RequestStatusPointer rso;
    output->wait_and_pop(rso);
    
    cv::imwrite("color.jpg", rso->res_img);
    return 0;
}
