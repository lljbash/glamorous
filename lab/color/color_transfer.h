#pragma once

#include <opencv2/opencv.hpp>

namespace glamorous {

class ColorTransfer {
private:
    static constexpr NUM_COLOR = 5;

    cv::Mat src;
    cv::Mat dst;
    cv::Mat color;
    std::vector<float> w;

public:
    void initialize(const cv::Mat &src, const cv::Mat &colors);
    void set_weight(const std::vector<float> &w);
    void apply(float level);
    
    void get_dst() const;

protected:
    virtual void convert_color(cv::Mat &src, bool reversed) const = 0;
    virtual float get_distance(const cv::Vec3b &x, const cv::Vec3b &y) const = 0;
    virtual cv::Vec3b get_average(const cv::Vec3b &src, const cv::Vec3b &ref,
                                  float level) const = 0;
};

}
