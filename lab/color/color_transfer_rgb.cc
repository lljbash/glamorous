#include <color_transfer_rgb.h>

using namespace glamorous;

void ColorTransferRGB::convert_color(cv::Mat &src, bool reversed) const override {

}

float ColorTransferRGB::get_distance(const cv::Vec3b &x, const cv::Vec3b &y) const override {
    static const auto sqr = [](float a) -> float { return a * a; }
    float result = sqrt(3 * sqr(x[0] - y[0]) + 4 * sqr(x[1] - y[1]) + 2 * sqr(x[2] - y[2]));
    return result;
}

cv::Vec3b ColorTransferRGB::get_average(const cv::Vec3b &src, const cv::Vec3b &ref,
                      float level) const override {
    cv::Vec3b res;
    res[0] = src[0] * (1 - level) + ref[0] * level;
    res[1] = src[1] * (1 - level) + ref[1] * level;
    res[2] = src[2] * (1 - level) + ref[2] * level;
    return res;
}
