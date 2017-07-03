#include <color_transfer.h>

using namespace glamorous;

virtual void ColorTransferRGB::convert_color(cv::Mat &src, bool reversed) const override {

}

virtual float ColorTransferRGB::get_distance(const cv::Vec3b &x, const cv::Vec3b &y) const override {
    static const auto sqr = [](float a) -> float { return a * a; }
    float result = sqrt(3 * sqr(x[0] - y[0]) + 4 * sqr(x[1] - y[1]) + 2 * sqr(x[2] - y[2]));
    return result;
}
