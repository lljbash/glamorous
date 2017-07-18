#pragma once

#include <vector>

#include <opencv2/opencv.hpp>

namespace glamorous{

class ColorKMeans {
public:
    void initialize(const std::vector<cv::Vec3b> &points, int k);
    void cluster(int iter);
    std::vector<cv::Vec3b> get_cluster() const;
private:
    double get_lab_distance(const cv::Vec3b &a, const cv::Vec3b &b);
    cv::Vec3b get_center(const std::vector<cv::Vec3b> &group);
    bool single_step_cluster();
    cv::Vec3b deal_with_void_cluster() const;
    
    std::vector<cv::Vec3b> points_;
    std::vector<cv::Vec3b> cluster_centers_;
};

}
