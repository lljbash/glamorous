#pragma once

#include <memory>
#include <opencv2/opencv.hpp>
#include "color_difference.h"

namespace glamorous {

class ColorTransfer {
private:
    static constexpr int NUM_COLOR = 5;

    cv::Mat src_;
    cv::Mat dst_;
    cv::Mat colors_;
    std::vector<float> w_;

    std::unique_ptr<ColorDifference> cd_;

public:
    ColorTransfer(ColorDifference *cd);
    virtual ~ColorTransfer();

    void initialize(const cv::Mat &src, const cv::Mat &colors);
    void set_weight(const std::vector<float> &w);
    void apply(float level);

    cv::Mat get_dst() const;
};

}
