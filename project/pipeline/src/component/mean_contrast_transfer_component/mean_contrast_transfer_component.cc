#include "mean_contrast_transfer_component.h"
#include "component/component_factory.h"
#include "log.h"

using namespace glamorous;

ComponentPointer MeanContrastTransferComponentFactory::create(std::string name) {
    return std::make_shared<MeanContrastTransferComponent>(name);
}

MeanContrastTransferComponent::MeanContrastTransferComponent(std::string name) : Component(name) {}

void MeanContrastTransferComponent::process(RequestStatusPointer request) {
    cv::Mat res;
    cv::cvtColor(request->res_img, res, CV_BGR2HSV);

    double saturation_rate = request->ref_attr.saturation_contrast / request->src_attr.saturation_contrast;
    double saturation_bias = request->ref_attr.saturation_mean - request->src_attr.saturation_mean * saturation_rate;

    double brightness_rate = request->ref_attr.brightness_contrast / request->src_attr.brightness_contrast;
    double brightness_bias = request->ref_attr.brightness_mean - request->src_attr.brightness_mean * brightness_rate;

    for(int i = 0; i < request->res_img.rows; i++) {
        for(int j = 0; j < request->res_img.cols; j++) {
            int newSaturation = saturation_rate * res.at<cv::Vec3b>(i, j)[1] + saturation_bias;
            int newBrightness = brightness_rate * res.at<cv::Vec3b>(i, j)[2] + brightness_bias;

            if(newSaturation > 255) {
                newSaturation = 255;
            }
            if(newSaturation < 0) {
                newSaturation = 0;
            }
            if(newBrightness > 255) {
                newBrightness = 255;
            }
            if(newBrightness < 0) {
                newBrightness = 0;
            }

            res.at<cv::Vec3b>(i, j)[1] = newSaturation;
            res.at<cv::Vec3b>(i, j)[2] = newBrightness;
        }
    }

    cv::cvtColor(res, request->res_img, CV_HSV2BGR);
}
