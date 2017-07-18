#include "color_kmeans.h"

#include <algorithm>
#include <limits>
#include <tuple>

using namespace glamorous;

void ColorKMeans::initialize(const std::vector<cv::Vec3b> &points, int k) {
    points_ = std::vector<cv::Vec3b>(points);
    std::random_shuffle(points_.begin(), points_.end());
    k = std::min(k, static_cast<int>(points_.size()));
    cluster_centers_ = std::vector<cv::Vec3b>(points_.begin(), points_.begin() + k);
}

void ColorKMeans::cluster(int iter) {
    bool ifChange;
    for (int i = 0; i < iter; i++) {
        ifChange = single_step_cluster();
        if(!ifChange) {
            break;
        }
    }
}

std::vector<cv::Vec3b> ColorKMeans::get_cluster() const {
    return cluster_centers_;
}

double ColorKMeans::get_lab_distance(const cv::Vec3b &a, const cv::Vec3b &b) {
    constexpr auto sqr = [](double a) -> double { return a * a; };
    constexpr double wL = (100. / 255.) * (100. / 255.);
    double result = sqrt(sqr(a[0] - b[0]) * wL + sqr(a[1] - b[1]) + sqr(a[2] - b[2]));
    return result;
}

cv::Vec3b ColorKMeans::get_center(const std::vector<cv::Vec3b> &cluster) {
    int L = 0;
    int a = 0;
    int b = 0;
    int weight = cluster.size();
    for (cv::Vec3b point : cluster) {
        L += point[0];
        a += point[1];
        b += point[2];
    }
    if (weight != 0) {
        return cv::Vec3b(L / weight, a / weight, b/ weight);
    }
    else {
        cv::Vec3b new_center = deal_with_void_cluster();
        return new_center;
    }
}

cv::Vec3b ColorKMeans::deal_with_void_cluster() const {
    double max_distance = 0.;
    int index = 0;
    for (int i = 0; i < points_.size(); i++) {
        double distance = 0.;
        for (cv::Vec3b cluster_center : cluster_centers_) {
            distance += get_lab_distance(points_[i], cluster_center);
        }
        if (distance > max_distance) {
            max_distance = distance;
            index = i;
        }
    }
    return points[i].clone();
}

bool ColorKMeans::single_step_cluster() {
    int k = cluster_centers_.size();
    std::vector<std::vector<cv::Vec3b>> all_clusters(k);
    for (cv::Vec3b point : points_) {
        double min_distance = std::numeric_limits<double>::infinity();
        int index = 0;
        for (int i = 0; i < k; i++) {
            double distance = get_lab_distance(point, cluster_centers_[i]);
            if (distance < min_distance) {
                min_distance = distance;
                index = i;
            }
        }
        all_clusters[index].push_back(point);
    }
    std::vector<cv::Vec3b> new_cluster_centers_;
    using IdSize = std::tuple<int, size_t>;
    std::vector<IdSize> id_size;
    for (int i = 0; i < k; i++) {
        id_size.emplace_back(i, all_clusters[i].size());
    }
    auto cmp = [](const IdSize &a, const IdSize &b) {
        return std::get<size_t>(a) > std::get<size_t>(b);
    };
    std::sort(id_size.begin(), id_size.end(), cmp);
    for (int i = 0; i < k; i++) {
        new_cluster_centers_.push_back(get_center(all_clusters[std::get<int>(id_size[i])]));
    }
    bool ifChange = false;
    for (int i = 0; i < k; i++) {
        if (cluster_centers_[i][0] != new_cluster_centers_[i][0]) {
            ifChange = true;
            break;
        }
        if (cluster_centers_[i][1] != new_cluster_centers_[i][1]) {
            ifChange = true;
            break;
        }
        if (cluster_centers_[i][2] != new_cluster_centers_[i][2]) {
            ifChange = true;
            break;
        }
    }
    cluster_centers_ = std::move(new_cluster_centers_);
    return ifChange;
}
