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
    void apply();
    
    void get_dst() const;

protected:
    virtual void convert_color(cv::Mat &src) const = 0;
    virtual void get_distance(const cv::Vec3b &x, const cv::Vec3b &y) const = 0;
};

}
