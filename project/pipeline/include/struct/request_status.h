#pragma once

#include <memory>
#include <opencv2/opencv.hpp>
#include "image_attr.h"

namespace glamorous {

struct RequestStatus {
    std::string id;
    std::string request_type;
    
    // std::shared_ptr<ImageDataBase> image_db;
    
    cv::Mat src_img;
    cv::Mat res_img;
    cv::Mat ref_img;
    
    ImageAttr src_attr;
    ImageAttr ref_attr;
};

using RequestStatusPointer = std::shared_ptr<RequestStatus>;

} // namespace glamorous
