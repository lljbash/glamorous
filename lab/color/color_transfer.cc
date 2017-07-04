#include "color_transfer.h"
#include <cassert>

using namespace glamorous;

ColorTransfer::ColorTransfer(ColorDifference *cd) : cd_(cd) {
    
}

ColorTransfer::~ColorTransfer() {
    
}

void ColorTransfer::initialize(const cv::Mat &src, const cv::Mat &colors) {
    assert(colors.rows == 1 && colors.cols == NUM_COLOR);
    src_ = src.clone();
    colors_ = colors.clone();
    dst_ = cv::Mat::zeros(src_.size(), CV_8UC3);
    cd_->convert_color(src_, false);
    cd_->convert_color(colors_, false);
    w_ = std::vector<float>(NUM_COLOR, 1.0);
}

void ColorTransfer::set_weight(const std::vector<float> &w) {
    assert(w.size() == NUM_COLOR);
    w_ = std::vector<float>(w);
}

void ColorTransfer::apply(float level) {
    for (int r = 0; r < src_.rows; ++r) {
        for (int c = 0; c < src_.cols; ++c) {
            if (r == 200 && c == 400) {
                printf("haha\n");
            }
            const cv::Vec3b &pix = src_.at<cv::Vec3b>(r, c);
            const cv::Vec3b ref = cd_->pick_most_similar(pix, colors_, w_);
            cv::Vec3b avg = cd_->get_average(pix, ref, level);
            memcpy(dst_.ptr<cv::Vec3b>(r) + c, &avg, 3);
        }
    }
}

cv::Mat ColorTransfer::get_dst() const {
    cv::Mat res = dst_.clone();
    cd_->convert_color(res, true);
    return res;
}
