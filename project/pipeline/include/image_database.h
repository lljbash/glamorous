#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include <opencv2/opencv.hpp>

#include "struct/image_attr.h"

namespace glamorous {

class ImageDataBase {
public:
    enum {
        OPEN_FAIL = -1,
        READ_SUCCESS = 0
    };

    int load_tsv(std::string db_path, std::string filename);
    const std::vector<ImageAttr> &get_image_list() const;
    cv::Mat read_image(int index) const;

private:
    std::vector<ImageAttr> img_list_;
    std::string db_path_;
};

using ImageDataBasePointer = std::shared_ptr<ImageDataBase>;

} // namespace glamorous
