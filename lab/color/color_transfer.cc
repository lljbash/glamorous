#include "color_trasfer.h"

using namespace glamorous;

void ColorTransfer::initialize(const cv::Mat &src, const cv::Mat &colors) {
    
}

void ColorTransfer::set_weight(const std::vector<float> &w);

void ColorTransfer::apply(float level);

void ColorTransfer::get_dst() const;
