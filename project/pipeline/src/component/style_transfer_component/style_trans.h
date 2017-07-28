#pragma once

#include <string>
#include <opencv2/opencv.hpp>

namespace glamorous {

class StyleTrans {
private:
    cv::Mat content_;
    cv::Mat style_;
    cv::Mat dst_;

    std::string prefix_;
    std::string contentName_;
    std::string styleName_;
    std::string dstName_;

    bool gpu_;

    double alpha_;

public:
    static const DEFAULT_ALPHA = 1;

    StyleTrans(std::string prefix);
    ~StyleTrans();

    void setPrefix(std::string prefix);
    void setGpu(bool gpu);
    void setAlpha(double alpha);

    void initialize(const cv::Mat &content, const cv::Mat &style);
    void initialize(const std::string &contentName, const std::string &styleName);
    int apply();

    cv::Mat get_dst() const;
};

} // namespace glamorous
