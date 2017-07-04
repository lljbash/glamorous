#include "color_difference_lab.h"

using namespace glamorous;

void ColorDifferenceLAB::convert_color(cv::Mat &src, bool reversed) const {
    if (!reversed) {
        cv::cvtColor(src, src, CV_BGR2Lab);
    } else {
        cv::cvtColor(src, src, CV_Lab2BGR);
    }
}

float ColorDifferenceLAB::get_distance(const cv::Vec3b &x, const cv::Vec3b &y) const {
    static const auto sqr = [](float a) -> float { return a * a; };
    float result = sqrt(sqr(x[0] - y[0]) + sqr(x[1] - y[1]) + sqr(x[2] - y[2]));
    return result;
}

cv::Vec3b ColorDifferenceLAB::get_average(const cv::Vec3b &src, const cv::Vec3b &ref,
                      float level) const {
    cv::Vec3b res;
    res[0] = src[0] * (1 - level) + ref[0] * level;
    res[1] = src[1] * (1 - level) + ref[1] * level;
    res[2] = src[2] * (1 - level) + ref[2] * level;
    return res;
}
