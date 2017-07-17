#pragma once

#include <string>
#include <opencv2/opencv.hpp>

namespace glamorous {

class StyleTrans {
private:
    cv::Mat content_;
    cv::Mat style_
    cv::Mat dst_;

    std::string prefix_;
    std::string contentName_;
    std::string styleName_;
    std::string dstName_;

    bool gpu_;

public:
    StyleTrans(std::string prefix);
    ~StyleTrans();

    void setPrefix(std::string prefix);
    void setGpu(bool gpu);

    void initialize(const cv::Mat &content, const cv::Mat &style);
    void initialize(string &contentName, string &styleName);
    int apply();

    cv::Mat get_dst() const;
};

} // namespace glamorous
