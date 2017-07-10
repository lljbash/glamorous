#include <opencv2/opencv.hpp>

#include "log.h"
#include "component/component_factory.h"

using namespace glamorous;

int main(int argc, char const *argv[]) {
    Log::info("hello");
    ColorTransferComponentFactory ctcf(0.8);
    ComponentPointer cp = ctcf.create();
    RequestStatusPointer rs = std::make_shared<RequestStatus>();
    rs->src_img = cv::imread("/mnt/c/run/obj-c/glamorous/lab/color/a.jpg");
    rs->res_img = rs->src_img.clone();
    int five[5][3] = {{216, 175, 157}, {154, 170, 168}, {106, 113, 113}, {96, 108, 99}, {79, 76, 58}};
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
            rs->ref_attr.five_color[i][j] = five[i][j];
        }
    }
    rs->src_attr.saturation_mean = 0.38143;
    rs->src_attr.saturation_contrast = 0.107267;
    rs->src_attr.brightness_mean = 0.738516;
    rs->src_attr.brightness_contrast = 0.217394;

    rs->ref_attr.saturation_mean = 0.231219;
    rs->ref_attr.saturation_contrast = 0.114508;
    rs->ref_attr.brightness_mean = 0.555568;
    rs->ref_attr.brightness_contrast = 0.197405;

    cp->push_request(rs);
    cp->push_request(nullptr);

    MeanContrastTransferComponentFactory mctcf;
    ComponentPointer cp2 = mctcf.create();

    cp->set_next_component(cp2);

    cp->start();
    cp2->start();


    cv::imwrite("color.jpg", rs->res_img);
    return 0;
}
