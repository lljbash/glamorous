#pragma once

#include <opencv2/opencv.hpp>

namespace glamorous {

class ETF {
private:
    cv::Size size;
    cv::Mat gradient;
	cv::Mat etf;

public:
    void init_etf(cv::Mat src); // src must be CV_8U grayscale image
    void refine_etf(int kernel, int times);
    cv::Mat get_etf() const;

private:
    void calc_new_etf(int kernel);
    
    float calc_phi(const cv::Vec3f &x, const cv::Vec3f &y) const;
    float calc_ws(const cv::Point2f &x, const cv::Point2f &y, int r) const;
    float calc_wm(float g_x, float g_y) const;
    float calc_wd(const cv::Vec3f &x, const cv::Vec3f &y) const;
};

} // namespace Glamorous
