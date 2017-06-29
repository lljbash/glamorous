#include "texture_transfer.h"

using namespace std;
using namespace glamorous;

void TextureTransfer::initialize(const cv::Mat &src, const cv::Mat &ref) {
    
}

void TextureTransfer::scan() {
    
}

cv::Mat TextureTransfer::get_dst() const {
    return dst.clone();
}

vector<cv::Point2d> TextureTransfer::build_Q(const cv::Point2d &r) {
    
}

float TextureTransfer::get_distance(const cv::Point2d &r, const cv::Point2d &q) const {
    
}

float TextureTransfer::calc_dN(const cv::Point2d &r, const cv::Point2d &q) const {
    
}

float TextureTransfer::calc_dL(const cv::Point2d &r, const cv::Point2d &q) const {
    
}

float TextureTransfer::calc_dI(const cv::Point2d &r, const cv::Point2d &q) const {
    
}
