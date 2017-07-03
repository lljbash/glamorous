#pragma once

#include <opencv2/opencv.hpp>

namespace glamorous {

class ColorDifference {
protected:
    virtual void convert_color(cv::Mat &src, bool reversed) const = 0;
    virtual float get_distance(const cv::Vec3b &x, const cv::Vec3b &y) const = 0;
    virtual cv::Vec3b get_average(const cv::Vec3b &src, const cv::Vec3b &ref,
                                  float level) const = 0;
    
    cv::Vec3b pick_most_similar(const cv::Vec3b &src,
                                const cv::Mat &colors,
                                const std::vector<float> &w) const;
};

}
