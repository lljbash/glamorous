#pragma once

#include <string>

namespace glamorous {

struct ImageAttr {
    std::string filename;
    std::string rcc_filename;

    // color pattern
    float saturation_mean;
    float saturation_contrast;
    float brightness_mean;
    float brightness_contrast;
    float warm_cool;
    float clear_dull;
    int five_color[5][3]; // RGB Color

    // figure ground
    float area_difference;
    float color_difference;
    float texture_complexity[2];

    // shape
    float shape_match[14];

    int shape_big;
    int shape_small;
};

} // namespace glamorous
