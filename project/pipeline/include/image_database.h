#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "struct/image_attr.h"

namespace glamorous {

class ImageDataBase {
public:
    enum {
        OPEN_FAIL = -1,
        READ_SUCCESS = 0
    };

    int load_tsv(const char* filename);
    const std::vector<ImageAttr> &get_image_list() const;

private:
    std::vector<ImageAttr> img_list_;
};

using ImageDataBasePointer = std::shared_ptr<ImageDataBase>;

} // namespace glamorous
