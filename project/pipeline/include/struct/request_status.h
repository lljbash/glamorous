#pragma once

#include <memory>
#include <opencv2/opencv.hpp>
#include "image_database.h"
#include "image_attr.h"

namespace glamorous {

struct RequestStatus {
    enum RequestType {
        Abstract = 0,
        Impressionism,
        Pointillism,
        PostImpressionism,
        Shuimo,
        Suprematism
    };
    
    std::string id;
    RequestType request_type;
    std::string text;
    
    ImageDataBasePointer image_db;
    
    cv::Mat src_img;
    cv::Mat res_img;
    cv::Mat res_rcc_img;
    cv::Mat ref_img;
    
    std::string video_name;
    
    ImageAttr src_attr;
    ImageAttr ref_attr;
};

using RequestStatusPointer = std::shared_ptr<RequestStatus>;

} // namespace glamorous
