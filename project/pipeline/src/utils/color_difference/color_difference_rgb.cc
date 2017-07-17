#include "color_difference_rgb.h"

using namespace glamorous;

void ColorDifferenceRGB::convert_color(cv::Mat &src, bool reversed) const {
    
}

float ColorDifferenceRGB::get_distance(const cv::Vec3b &x, const cv::Vec3b &y) const {
    static const auto sqr = [](float a) -> float { return a * a; };
    float result = sqrt(3 * sqr(x[0] - y[0]) + 4 * sqr(x[1] - y[1]) + 2 * sqr(x[2] - y[2]));
    return result;
}

cv::Vec3b ColorDifferenceRGB::get_average(const cv::Vec3b &src, const cv::Vec3b &ref,
                      float level) const {
    cv::Vec3b res;
    res[0] = src[0] * (1 - level) + ref[0] * level;
    res[1] = src[1] * (1 - level) + ref[1] * level;
    res[2] = src[2] * (1 - level) + ref[2] * level;
    return res;
}
