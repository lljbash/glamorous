#include <cstdlib>
#include <iostream>
#include <string>
#include <algorithm>

#include <opencv2/opencv.hpp>

void getCPAttr(std::string filename) {
    double nCoolColor=0;
    double nClearColor=0;

    IplImage* srcImg = cvLoadImage(filename.c_str(), CV_LOAD_IMAGE_COLOR);
    double imgSize = srcImg->width * srcImg->height;

    IplImage* dest1;
    dest1 = cvCreateImage(cvGetSize(srcImg),IPL_DEPTH_8U,3);

    cvCvtColor(srcImg,dest1,CV_RGB2HSV);

    // mean value
    double briSum=0, satSum=0, hueSum=0;
    std::vector<double> nhue;
    nhue.resize(10);
    for (int y=0;y<dest1->height;y++)
        for (int x=0;x<dest1->width;x++)
        {
            double tmpHue = (double)CV_IMAGE_ELEM(dest1, uchar, y, x * 3);
            double tmpHue =
            hueSum += tmpHue/180;
            if ((tmpHue>30) && (tmpHue<110))
                nCoolColor++;

            satSum += (double)CV_IMAGE_ELEM(dest1, uchar, y, x * 3 + 1) /255;
            double tmpBri = (double)CV_IMAGE_ELEM(dest1, uchar, y, x * 3 + 2) /255;
            briSum += tmpBri;
            if (tmpBri > 0.7)
                nClearColor++;
        }

    double bright = briSum/imgSize;
    double sat = satSum/imgSize;
    double hue = hueSum/imgSize;

    // absolute value
    double briConSum=0, satConSum=0, hueConSum=0;
    for (int y=0;y<dest1->height;y++)
        for (int x=0;x<dest1->width;x++)
        {
            hueConSum += std::abs((double)CV_IMAGE_ELEM(dest1, uchar, y, x * 3)/180 - hue);
            satConSum += std::abs((double)CV_IMAGE_ELEM(dest1, uchar, y, x * 3 + 1)/255 - sat);
            briConSum += std::abs((double)CV_IMAGE_ELEM(dest1, uchar, y, x * 3 + 2)/255 - bright);
        }

    double hueContrast = hueConSum/imgSize;
	double brightContrast = briConSum/imgSize;
    double satContrast = satConSum/imgSize;

	double coolColorRatio = nCoolColor/imgSize;
    double clearColorRatio = nClearColor/imgSize;

    cvReleaseImage(&srcImg);
    cvReleaseImage(&dest1);

    std::cout<<sat<<" "<<satContrast<<" "<<bright<<" "<<brightContrast<<" "\
    <<coolColorRatio<<" "<<clearColorRatio << " "<<std::endl;
}

int main(int argc, char const *argv[]) {
    std::string filename("../zlj.png");
    getCPAttr();

    return 0;
}
