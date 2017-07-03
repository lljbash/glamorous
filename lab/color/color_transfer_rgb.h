#pragma once

#include <color_transfer.h>

namespace glamorous {

class ColorTransferRGB : public ColorTransfer {
protected:
    virtual void convert_color(cv::Mat &src, bool reversed) const override;
    virtual float get_distance(const cv::Vec3b &x, const cv::Vec3b &y) const override;
};

}
