#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

namespace glamorous {

class TextureTransfer {
private:
    static constexpr float wL = 1;
    static constexpr int radius = 2;
    static constexpr float p_rand = 0.05;
    
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;
    
    cv::Mat src;
	cv::Mat ref;
	cv::Mat dst;
    std::vector<cv::Point2d> g; // mapping coordinate from dst to ref
    cv::Mat edge;
    cv::Mat etf;

public:
    TextureTransfer();
    
    void initialize(const cv::Mat &src, const cv::Mat &ref);
    void scan(); // scan once
    cv::Mat get_dst() const;

private:
    std::vector<cv::Point2d> build_Q(const cv::Point2d &r);
    float get_distance(const cv::Point2d &r, const cv::Point2d &q) const;
    
    void try_add_Q(const cv::Point2d &r, int x, int y, std::vector<cv::Point2d> *Q);
    float calc_dN(const cv::Point2d &r, const cv::Point2d &q) const;
    float calc_dL(const cv::Point2d &r, const cv::Point2d &q) const;
    float calc_dI(const cv::Point2d &r, const cv::Point2d &q) const;
    
    std::vector<cv::Point2d> line(int x0, int x1, int y0, int y1, int num) const;
};

} // namespace glamorous
