#include "color_difference.h"
#include <limits>

cv::Vec3b ColorDifference::pick_most_similar(const cv::Vec3b &src,
                                             const cv::Mat &colors,
                                             const std::vector<float> &w) const {
    float min_dist = std::numric_limits<float>::infinity();
    int min_index = 0;
    for (int i = 0; i < color.cols; ++i) {
        if (w[i] == 0) {
            continue;
        }
        const cv::Vec3b &ref = color.ptr<cv::Vec3b>()[i];
        int dist = get_distance(pix, ref) / w[i];
        if (dist < min_dist) {
            min_dist = dist;
            min_index = i;
        }
    }
    return color.ptr<cv::Vec3b>()[min_index];
}
