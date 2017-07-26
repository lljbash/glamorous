#pragma once

#include <string>
#include <opencv2/opencv.hpp>

namespace glamorous {

class InkVideo {
public:
    void get_video(cv::Mat src_img, std::string videoname);
};

}
