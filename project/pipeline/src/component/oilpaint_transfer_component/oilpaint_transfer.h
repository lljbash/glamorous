#pragma once

#include <opencv2/opencv.hpp>

namespace glamorous {
namespace oilpaint {

cv::Mat oilpaint_transfer(const cv::Mat &image, std::string pre_video_name, std::string output_video_name);

} // namespace oilpaint
} // namespace glamorous
