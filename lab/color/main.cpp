#include "color_transfer.h"
#include "color_difference_rgb.h"
#include "color_difference_hsv.h"

using namespace glamorous;

int main(int argc, char const *argv[]) {
    cv::Mat image = cv::imread("../a.jpg", 1);
    ColorTransfer ct(new ColorDifferenceRGB());
    cv::Mat c = cv::Mat::zeros(1, 5, CV_8UC3);
    c.ptr<cv::Vec3b>()[0] = {216, 175, 157};
    c.ptr<cv::Vec3b>()[1] = {154, 170, 168};
    c.ptr<cv::Vec3b>()[2] = {106, 113, 113};
    c.ptr<cv::Vec3b>()[3] = {96, 108, 99};
    c.ptr<cv::Vec3b>()[4] = {79, 76, 58};
    ct.initialize(image, c);
    ct.apply(0.4);
    cv::Mat dst = ct.get_dst();
    cv::imwrite("color.png", dst);
    printf("Ha, you like it, ha?\n");
    return 0;
}
