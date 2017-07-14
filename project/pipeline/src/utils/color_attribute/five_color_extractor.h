#pragma once

#include <opencv2/opencv.hpp>

namespace glamorous {

class FiveColorExtractor {
public:
    static constexpr int MAX_ITER_TIMES = 50;
    
    struct FiveColor {
        int five_color[5][3]; //RGB Color
    };
    
    FiveColor extract(const cv::Mat &image_bgr) const;
};

} // namespace glamorous
