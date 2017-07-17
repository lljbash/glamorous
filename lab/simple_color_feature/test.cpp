#include <cmath>
#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>

#include "simple_color_feature_extractor.h"

using namespace std;
using namespace glamorous;

void getCPAttr(std::string filename) {
    cv::Mat image_bgr = cv::imread(filename.c_str(), CV_LOAD_IMAGE_COLOR);
    
    SimpleColorFeatureExtractor extractor;
    auto feature = extractor.extract(image_bgr);
    
    cout << feature.saturation_mean << endl;
    cout << feature.saturation_contrast << endl;
    cout << feature.brightness_mean << endl;
    cout << feature.brightness_contrast << endl;
    cout << feature.warm_cool << endl;
    cout << feature.clear_dull << endl;
}

int main(int argc, char const *argv[]) {
    std::string filename("../zlj.png");
    getCPAttr(filename);
    return 0;
}
