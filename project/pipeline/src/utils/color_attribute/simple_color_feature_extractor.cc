#include "simple_color_feature_extractor.h"

using namespace glamorous;

auto SimpleColorFeatureExtractor::extract(const cv::Mat &image_bgr) const
        -> SimpleColorFeature {
    int img_size = image_bgr.cols * image_bgr.rows;
    cv::Mat image_hsv;
    cv::cvtColor(image_bgr, image_hsv, CV_BGR2HSV);
    
    // mean value
    double bri_sum = 0.;
    double sat_sum = 0.;
    // double hue_sum = 0.;
    int cool_color_count = 0;
    int clear_color_count = 0;
    for (int y = 0; y < image_hsv.rows; y++) {
        for (int x = 0; x < image_hsv.cols; x++) {
            const cv::Vec3b &hsb = image_hsv.at<cv::Vec3b>(y, x);
            // hue_sum += static_cast<double>(hsb[0]) / 180.;
            sat_sum += static_cast<double>(hsb[1]) / 255.;
            bri_sum += static_cast<double>(hsb[2]) / 255.;
            if (hsb[0] >= 30 && hsb[0] <= 110) {
                cool_color_count++;
            }
            if (hsb[2] > 0.7 * 255) {
                clear_color_count++;
            }
        }
    }
    
    double bri = bri_sum / img_size;
    double sat = sat_sum / img_size;
    // double hue = hue_sum / img_size;
    double cool_color_ratio = static_cast<double>(cool_color_count) / img_size;
    double clear_color_ratio = static_cast<double>(clear_color_count) / img_size;
    
    // absolute value
    double bri_con_sum = 0.;
    double sat_con_sum = 0.;
    // double hue_con_sum = 0.;
    for (int y = 0; y < image_hsv.rows; y++) {
        for (int x = 0; x < image_hsv.cols; x++) {
            const cv::Vec3b &hsb = image_hsv.at<cv::Vec3b>(y, x);
            // hue_con_sum += std::abs(static_cast<double>(hsb[0]) / 180. - hue);
            sat_con_sum += std::abs(static_cast<double>(hsb[1]) / 255. - sat);
            bri_con_sum += std::abs(static_cast<double>(hsb[2]) / 255. - bri);
        }
    }
    
    // double hue_contrast = hue_con_sum / img_size;
    double bri_contrast = bri_con_sum / img_size;
    double sat_contrast = sat_con_sum / img_size;
    
    return {sat, sat_contrast, bri, bri_contrast, cool_color_ratio, clear_color_ratio};
}
