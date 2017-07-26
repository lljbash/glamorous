#include "ink_video.h"

#include <algorithm>
#include <iostream>
#include <random>
#include <functional>
#include <vector>
#include <unordered_set>
#include <tuple>

using namespace glamorous;

void InkVideo::get_video(cv::Mat src_img, std::string videoname) {
    cv::VideoWriter writer(videoname.c_str(), CV_FOURCC('D', 'I', 'V', 'X'), 60, src_img.size(), 0);
    
    int h = src_img.rows;
    int w = src_img.cols;
    
    auto pt2int = [w](int r, int c) { return r * w + c; };
    auto int2pt = [w](int pos) { return std::make_tuple(pos / w, pos % w); };
    
    std::vector<int> min_pos;
    uchar max_gray = 0;
    uchar min_gray = 255;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (src_img.at<uchar>(i, j) > max_gray) {
                max_gray = src_img.at<uchar>(i, j);
            }
            if (src_img.at<uchar>(i, j) < min_gray) {
                min_gray = src_img.at<uchar>(i, j);
            }
        }
    }
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (src_img.at<uchar>(i, j) < min_gray + 20) {
                min_pos.push_back(pt2int(i, j));
            }
        }
    }
    
    std::unordered_set<int> current_pos;
    if (min_pos.size() > 10) {
        std::random_shuffle(min_pos.begin(), min_pos.end());
        min_pos = std::vector<int>(min_pos.begin(), min_pos.begin() + 10);
    }
    for (int pos : min_pos) {
        current_pos.insert(pos);
    }
    
    cv::Mat res_img(src_img.size(), CV_8UC1, cv::Scalar(max_gray));
    cv::Mat cnt_paint(src_img.size(), CV_8UC1, cv::Scalar(0));

    writer.write(res_img);
    
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(min_gray, max_gray + 10);
    auto dice = std::bind(distribution, generator);
    while (!current_pos.empty()) {
        std::vector<int> to_add;
        std::vector<int> to_remove;
        for (int pos : current_pos) {
            int r, c;
            std::tie(r, c) = int2pt(pos);
            if (dice() > src_img.at<uchar>(r, c)) {
                constexpr double step = 0.05;
                constexpr uchar max_step = 20;
                double opa = static_cast<double>(++cnt_paint.at<uchar>(r, c)) * step;
                res_img.at<uchar>(r, c) = src_img.at<uchar>(r, c) * opa + max_gray * (1 - opa);
                if (cnt_paint.at<uchar>(r, c) == max_step) {
                    to_remove.push_back(pos);
                }
                else if (cnt_paint.at<uchar>(r, c) == 1) {
                    for (int rr = std::max(r - 1, 0); rr <= std::min(r + 1, h - 1); ++rr) {
                        for (int cc = std::max(c - 1, 0); cc <= std::min(c + 1, w - 1); ++cc) {
                            if (cnt_paint.at<uchar>(rr, cc) == 0) {
                                to_add.push_back(pt2int(rr, cc));
                            }
                        }
                    }
                }
            }
        }
        for (int pos : to_add) {
            current_pos.insert(pos);
        }
        for (int pos : to_remove) {
            current_pos.erase(pos);
        }
        writer.write(res_img);
    }
    writer.release();
}

