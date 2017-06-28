#include "etf.h"

using namespace glamorous;

void ETF::init_etf(cv::Mat src) {
    size = src.size();
    gradient = cv::Mat::zeros(size, CV_32FC3);
    etf = cv::Mat::zeros(size, CV_32FC3);
    
    cv::Mat src_n, dx, dy;
    cv::normalize(src, src_n, 0., 1., cv::NORM_MINMAX, CV_32FC1);
    cv::Sobel(src_n, dx, CV_32FC1, 1, 0, 5);
    cv::Sobel(src_n, dy, CV_32FC1, 0, 1, 5);
    cv::magnitude(dx, dy, gradient);
    cv::normalize(gradient, gradient, 0., 1., cv::NORM_MINMAX);
    
    for (int i = 0; i < size.height; ++i) {
        for (int j = 0; j < size.width; ++j) {
            float u = dx.at<float>(i, j);
            float v = dy.at<float>(i, j);
            etf.at<cv::Vec3f>(i, j) = normalize(cv::Vec3f(-u, v, 0));
        }
    }
}

void ETF::refine_etf(int kernel, int times) {
    for (int i = 0; i < times; ++i) {
        calc_new_etf(kernel);
    }
}

cv::Mat ETF::get_etf() const {
    return etf.clone();
}

void ETF::calc_new_etf(int kernel) {
    for (int i = 0; i < size.height; ++i) {
        for (int j = 0; j < size.width; ++j) {
            cv::Vec3f t_new(0, 0, 0);
            const cv::Vec3f &t_cur_x = etf.at<cv::Vec3f>(i, j);
            for (int ii = i - kernel; ii <= i + kernel; ++ii) {
                for (int jj = j - kernel; jj <= j + kernel; ++jj) {
                    if (ii < 0 || ii >= size.height || jj < 0 || jj > size.width) {
                        continue;
                    }
                    const cv::Vec3f &t_cur_y = etf.at<cv::Vec3f>(ii, jj);
                    float phi = calc_phi(t_cur_x, t_cur_y);
        			float ws = calc_ws({float(i), float(j)}, {float(ii), float(jj)}, kernel);
        			float wm = calc_wm(gradient.at<float>(i, j), gradient.at<float>(ii, jj));
        			float wd = calc_wd(t_cur_x, t_cur_y);
        			t_new += phi * t_cur_y * ws * wm * wd;
                }
            }
            etf.at<cv::Vec3f>(i, j) = normalize(t_new);
        }
    }
}

float ETF::calc_phi(const cv::Vec3f &x, const cv::Vec3f &y) const {
    return x.dot(y) > 0 ? 1. : -1.;
}

float ETF::calc_ws(const cv::Point2f &x, const cv::Point2f &y, int r) const {
    return norm(x - y) < r ? 1. : 0.;
}

float ETF::calc_wm(float g_x, float g_y) const {
    float wm = (1. + tanh(g_y - g_x)) * 0.5;
	return wm;
}

float ETF::calc_wd(const cv::Vec3f &x, const cv::Vec3f &y) const {
    return cv::abs(x.dot(y));
}
