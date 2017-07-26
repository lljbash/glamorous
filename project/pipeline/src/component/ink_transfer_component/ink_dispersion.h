#pragma once

#include <opencv2/opencv.hpp>

namespace glamorous {

class InkDispersion {
private:
    cv::Mat src_;
    cv::Mat dst_;

    double lambda_;
    double alpha_;
    double canny_;

protected:
    int run_main(int argc, const char **argv);

public:
    static constexpr double DEFAULT_LAMBDA = 1;
    static constexpr double DEFAULT_ALPHA = 0.5;
    static constexpr double DEFAULT_CANNY = 0.3;

    InkDispersion();
    ~InkDispersion();

    void setLambda(double l);

    void setAlpha(double a);

    void setCanny(double c);

    void initialize(const cv::Mat &src);
    int apply();

    cv::Mat get_dst() const;
};

} // namespace glamorous
