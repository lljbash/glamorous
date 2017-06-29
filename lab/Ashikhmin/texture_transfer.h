#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

namespace glamorous;

class TextureTransfer {
private:
    static const float wL = 1;
    static const int radius = 2;
    
    cv::Mat src;
	cv::Mat ref;
	cv::Mat dst;
    cv::Mat g; // mapping coordinate from dst to ref

public:
    void initialize(const cv::Mat &src, const cv::Mat &ref);
    void scan(); // scan once
    cv::Mat get_dst() const;

private:
    std::vector<cv::Point2d> build_Q(const cv::Point2d &r);
    float get_distance(const cv::Point2d &r, const cv::Point2d &q) const;
    
    float calc_dN(const cv::Point2d &r, const cv::Point2d &q) const;
    float calc_dL(const cv::Point2d &r, const cv::Point2d &q) const;
    float calc_dI(const cv::Point2d &r, const cv::Point2d &q) const;
};

} // namespace glamorous
