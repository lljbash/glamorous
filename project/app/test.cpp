#include <opencv2/opencv.hpp>

#include "log.h"
#include "component/component_factory.h"
#include "pipeline.h"

using namespace glamorous;

int main(int argc, char const *argv[]) {
    Log::info("hello");
    ColorTransferComponentFactory ctcf(0.8);
    ComponentPointer cp = ctcf.create();
    RequestStatusPointer rs = std::make_shared<RequestStatus>();
    rs->src_img = cv::imread("../../lab/color/a.jpg");
    rs->res_img = rs->src_img.clone();
    int five[5][3] = {{216, 175, 157}, {154, 170, 168}, {106, 113, 113}, {96, 108, 99}, {79, 76, 58}};
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
            rs->ref_attr.five_color[i][j] = five[i][j];
        }
    }
    Pipeline p;
    auto output = std::make_shared<Component::WaitQueue>();
    p.set_static_pipeline({cp});
    p.start_parallel(output);
    p.new_request(rs);
    p.stop_parall();
    RequestStatusPointer rso;
    output->wait_and_pop(rso);
    cv::imwrite("color.jpg", rso->res_img);
    return 0;
}
