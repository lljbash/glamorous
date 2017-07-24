#include "five_color_extractor.h"

#include "utils/color_attribute/color_kmeans.h"

using namespace glamorous;

auto FiveColorExtractor::extract(const cv::Mat &image_bgr) const
        -> FiveColor {
    ColorKMeans ckm;
    cv::Mat src = image_bgr.clone();
    cv::cvtColor(src, src, CV_BGR2Lab);
    std::vector<cv::Vec3b> points;
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            points.push_back(src.at<cv::Vec3b>(i, j));
        }
    }
    ckm.initialize(points, 5);
    ckm.cluster(MAX_ITER_TIMES);
    std::vector<cv::Vec3b> centers = ckm.get_cluster();
    cv::Mat result(1, 5, CV_8UC3);
    for (int i = 0; i < 5; i++) {
        result.at<cv::Vec3b>(0, i) = centers[i];
    }
    cv::cvtColor(result, result, CV_Lab2BGR);
    FiveColor fc;
    for (int i = 0; i < 5; i++) {
        fc.five_color[i][0] = (int)result.at<cv::Vec3b>(0, i)[2];
        fc.five_color[i][1] = (int)result.at<cv::Vec3b>(0, i)[1];
        fc.five_color[i][2] = (int)result.at<cv::Vec3b>(0, i)[0];
    }
    return fc;
}
