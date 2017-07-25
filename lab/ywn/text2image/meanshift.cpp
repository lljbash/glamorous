#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <cstdlib>
#include <ctime>

using std::string;
using std::vector;

struct LuvHist
{
    vector<double> vec;
    int size;
    string file;
    LuvHist() {};
    LuvHist(vector<double>& _vec, int _size, string _f) : size(_size), file(_f)
    {
        for (int i = 0; i < _size; ++i)
            vec.push_back(_vec[i]);
    }
    ~LuvHist() {};
};

class MeanShift{
public:
    MeanShift(double bandwidth=0.75, double stopthresh=1e-3*0.75);
    LuvHist get_cluster_center(vector<LuvHist>& all_points, int mode=0);
    
private:
    double bandwidth_square;
    double stopthresh_square;
    const double EPSILON = 1e-7;
    const double CLUSTER_EPSILON = 0.5;
    
    vector<LuvHist> centers;
    
    LuvHist& get_shift_point(vector<LuvHist>& points);
    double euclidean_distance_square(LuvHist& a, LuvHist& b);
    LuvHist add(LuvHist& a, LuvHist& b);
    LuvHist subtract(LuvHist& a, LuvHist& b);
};

MeanShift::MeanShift(double bandwidth, double stopthresh){
    this->bandwidth_square = bandwidth*bandwidth;
    this->stopthresh_square = stopthresh*stopthresh;
}

double MeanShift::euclidean_distance_square(LuvHist &a, LuvHist &b){
    double distance_square = 0;
    int dim = (int)a.vec.size();
    for (int i=0; i<dim; i++) {
        distance_square += (a.vec[i]-b.vec[i])*(a.vec[i]-b.vec[i]);
    }
    return distance_square;
}

LuvHist MeanShift::add(LuvHist &a, LuvHist &b){
    vector<double> sum;
    int dim = (int)a.vec.size();
    for (int i=0; i<dim; i++) {
        sum[i] = a.vec[i] + b.vec[i];
    }
    LuvHist res(sum, dim, "");
    return res;
}

LuvHist MeanShift::subtract(LuvHist &a, LuvHist &b){
    vector<double> sum;
    int dim = (int)a.vec.size();
    for (int i=0; i<dim; i++) {
        sum[i] = a.vec[i] - b.vec[i];
    }
    LuvHist res(sum, dim, "");
    return res;
}

LuvHist MeanShift::get_cluster_center(vector<LuvHist>& all_points, int mode){
    // mode 0: most points neighbor center
    // random select
    int index = rand()%all_points.size();
    LuvHist center_point = all_points[index];
    while (1) {
        vector<LuvHist> M;
        for (int i=0; i<all_points.size(); i++) {
            if (bandwidth_square > euclidean_distance_square(center_point, all_points[i])) {
                M.push_back(all_points[i]);
            }
        }
        LuvHist shift;
        for (int i=0; i<M.size(); i++) {
            LuvHist delta = subtract(M[i], center_point);
            add(shift, delta);
        }
        center_point = add(center_point, shift);
        
        double shift_square = 0;
        for (int i=0; i<shift.vec.size(); i++) {
            shift_square += shift.vec[i]*shift.vec[i];
        }
        if (stopthresh_square > shift_square) {
            int r = rand()%M.size();
            return M[r];
        }
    }
    return LuvHist();
}
