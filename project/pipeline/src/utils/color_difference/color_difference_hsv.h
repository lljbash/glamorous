#pragma once

#include "color_difference.h"

namespace glamorous {

class ColorDifferenceHSV : public ColorDifference {
protected:
    void convert_color(cv::Mat &src, bool reversed) const override;
    float get_distance(const cv::Vec3b &x, const cv::Vec3b &y) const override;
    cv::Vec3b get_average(const cv::Vec3b &src, const cv::Vec3b &ref,
                          float level) const override;
};

}
