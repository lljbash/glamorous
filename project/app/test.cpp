#include <opencv2/opencv.hpp>

#include "log.h"
#include "component/component_factory.h"

using namespace glamorous;

int main(int argc, char const *argv[]) {
    Log::info("hello");
    ColorTransferComponentFactory ctcf(0.4);
    ComponentPointer cp = ctcf.create();
    RequestStatusPointer rs = std::make_shared<RequestStatus>();
    rs->src_img = cv::imread("/mnt/c/run/obj-c/glamorous/lab/color/a.jpg");
    int five[5][3] = {{216, 175, 157}, {154, 170, 168}, {106, 113, 113}, {96, 108, 99}, {79, 76, 58}};
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
            rs->ref_attr.five_color[i][j] = five[i][j];
        }
    }
    cp->push_request(rs);
    cp->push_request(nullptr);
    cp->start();

    cv::imwrite("color.jpg", rs->res_img);
    return 0;
}
