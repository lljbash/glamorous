#include "oilpaint_transfer.h"

int main(int argc, char const *argv[]) {
    cv::Mat image = cv::imread("../imp0.jpg", 1);
    cv::Mat res = glamorous::oilpaint::oilpaint_transfer(image);
    cv::imshow("res", res);
    cv::waitKey();
    return 0;
}
