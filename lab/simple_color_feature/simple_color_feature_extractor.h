#pragma once

#include <opencv2/opencv.hpp>

namespace glamorous {

class SimpleColorFeatureExtractor {
public:
    struct SimpleColorFeature {
        double saturation_mean;
        double saturation_contrast;
        double brightness_mean;
        double brightness_contrast;
        double warm_cool;
        double clear_dull;
    };
    
    SimpleColorFeature extract(const cv::Mat &image_bgr) const;
};

} // namespace glamorous
