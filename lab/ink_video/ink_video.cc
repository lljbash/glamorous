#include "ink_video.h"

#include <cstdlib>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace glamorous;

void InkVideo::get_video(std::string filename, std::string videoname) {
    cv::Mat src_img = cv::imread(filename);
    cv::resize(src_img, src_img, cv::Size(IMAGE_SIZE, IMAGE_SIZE));
    cv::Mat res_img = cv::Mat::zeros(IMAGE_SIZE, IMAGE_SIZE, CV_8UC3);
    cv::VideoWriter writer(videoname.c_str(), CV_FOURCC('D', 'I', 'V', 'X'), 60, res_img.size(), 1);
    
    
    for (int i = 0; i < IMAGE_SIZE; i++) {
        for(int j = 0; j < IMAGE_SIZE; j++) {
            res_img.at<cv::Vec3b>(i, j)[0] = 255;
            res_img.at<cv::Vec3b>(i, j)[1] = 255;
            res_img.at<cv::Vec3b>(i, j)[2] = 255;
        }
    }
    memset(cnt_paint_, 0, sizeof(cnt_paint_));

    for (int i = 45; i < 55; i++){
        for(int j = 45; j < 55; j++){
            cnt_paint_[i][j]++;
            res_img.at<cv::Vec3b>(i, j)[0] = src_img.at<cv::Vec3b>(i, j)[0];
            res_img.at<cv::Vec3b>(i, j)[1] = src_img.at<cv::Vec3b>(i, j)[1];
            res_img.at<cv::Vec3b>(i, j)[2] = src_img.at<cv::Vec3b>(i, j)[2];
        }
    }
    writer.write(res_img);
    
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1, 2);
    auto dice = std::bind(distribution, generator);
    for (int iter = 0; iter < ITER_TIME; iter++) {
        for (int i = 0; i < IMAGE_SIZE; i++) {
            for(int j = 0; j < IMAGE_SIZE; j++) {
                if (check_neighbor(i, j) && dice() == 1) {
                    double opa = cnt_paint_[i][j] * 0.1;
                    if (opa > 1) {
                        opa = 1.;
                    }
                    res_img.at<cv::Vec3b>(i, j)[0] = src_img.at<cv::Vec3b>(i, j)[0] * opa + 255 * (1 - opa);
                    res_img.at<cv::Vec3b>(i, j)[1] = src_img.at<cv::Vec3b>(i, j)[1] * opa + 255 * (1 - opa);
                    res_img.at<cv::Vec3b>(i, j)[2] = src_img.at<cv::Vec3b>(i, j)[2] * opa + 255 * (1 - opa);
                    
                    cnt_paint_[i][j]++;
                }
            }
        }
        writer.write(res_img);
    }
    cv::imwrite("out.png", res_img);
    writer.release();
}

bool InkVideo::check_neighbor(int i, int j) {
    if (i > 0) {
        if (cnt_paint_[i - 1][j] > 0) {
            return true;
        }
    }
    if (i < IMAGE_SIZE) {
        if (cnt_paint_[i + 1][j] > 0) {
            return true;
        }
    }
    if (j > 0) {
        if (cnt_paint_[i][j - 1] > 0) {
            return true;
        }
    }
    if (j < IMAGE_SIZE) {
        if (cnt_paint_[i][j + 1] > 0) {
            return true;
        }
    }
    return false;
}
