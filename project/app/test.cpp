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
    
    RequestStatusPointer rs = std::make_shared<RequestStatus>();
    
    rs->src_img = cv::imread("../../lab/color/a.jpg");
    rs->res_img = rs->src_img.clone();
    
    rs->src_attr.saturation_mean = 0.38143;
    rs->src_attr.saturation_contrast = 0.107267;
    rs->src_attr.brightness_mean = 0.738516;
    rs->src_attr.brightness_contrast = 0.217394;

    rs->ref_attr.saturation_mean = 0.231219;
    rs->ref_attr.saturation_contrast = 0.114508;
    rs->ref_attr.brightness_mean = 0.555568;
    rs->ref_attr.brightness_contrast = 0.197405;
    
    int five[5][3] = {{216, 175, 157}, {154, 170, 168}, {106, 113, 113}, {96, 108, 99}, {79, 76, 58}};
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
            rs->ref_attr.five_color[i][j] = five[i][j];
        }
    }
    
    Pipeline p;
    auto output = std::make_shared<Component::WaitQueue>();
    p.set_static_pipeline({cp, cp2});
    p.start_parallel(output);
    p.new_request(rs);
    p.stop_parallel();
    RequestStatusPointer rso;
    output->wait_and_pop(rso);
    
    cv::imwrite("color.jpg", rs->res_img);
    return 0;
}
