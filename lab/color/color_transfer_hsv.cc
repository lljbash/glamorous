#include <color_transfer_hsv.h>

using namespace glamorous;

void ColorTransferHSV::convert_color(cv::Mat &src, bool reversed) const override {
    if(reversed) {
        cv::cvtColor(src, src, CV_BGR2HSV);
    } else {
        cv::cvtColor(src, src, CV_HSV2BGR);
    }
}

float ColorTransferHSV::get_distance(const cv::Vec3b &x, const cv::Vec3b &y) const override {
    static const auto sqr = [](float a) -> float { return a * a; }
    // float dh = min(abs(x[0] - y[0]), 360 - abs(x[0] - y[0]));
    // float ds = (x[1] - y[1]);
    constexpr float pi = 3.14159265358979323847;
    float d = sqr(x[1]) + sqr(y[1]) - 2 * x[1] * y[1] * cos(360 * (x[0] - y[0]) / pi) + sqr(x[2] - y[2]);

    float result = sqrt(d);
    return result;
}

cv::Vec3b ColorTransferHSV::get_average(const cv::Vec3b &src, const cv::Vec3b &ref,
                      float level) const override {
    cv::Vec3b res;
    float clock_dis = ref[0] - src[0];
    if(clock_dis < 0) {
        clock_dis += 180;
    }
    if(clock_dis > 90) {
        res[0] = src[0] + clock_dis * level;
        if(clock_dis > 180) {
            clock_dis -= 180;
        }
    }else {
        clock_dis = 180 - clock_dis;
        res[0] = src[0] - clock_dis * level;
        if(clock_dis < 0) {
            clock_dis += 180;
        }
    }

    level_s = level * 0.5；
    level_v = level * 0.2;

    res[1] = src[1] * (1 - level_s) + ref[1] * level_s;
    res[2] = src[2] * (1 - level_v) + ref[2] * level_v;
    return res;
}
