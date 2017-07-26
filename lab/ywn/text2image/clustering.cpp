#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <algorithm>
#include <math.h>

using namespace cv;
using namespace std;

struct mMat
{
	Mat m;
	string file;
	mMat() {};
	mMat(Mat _m, string _f) : m(_m), file(_f) {};
};

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

	LuvHist(const LuvHist& lh) 
	{
		size = lh.size;
		file = lh.file;	
		for (int i = 0; i < size; ++i)
			vec.push_back(lh.vec[i]);
	}

	LuvHist& operator=(const LuvHist &lh)
	{
		vec.clear();
		size = lh.size;
		file = lh.file;	
		for (int i = 0; i < size; ++i)
			vec.push_back(lh.vec[i]);
		
		return *this; 
	}
};

void getFiles(string path, vector<string>& files)  
{ 
    const char* basePath = path.c_str();

    DIR *dir;
    struct dirent *ptr;
    char base[1000];

    if ((dir = opendir(basePath)) == NULL)
    {
        perror("Open dir error...");
        exit(1);
    }

    while ((ptr=readdir(dir)) != NULL)
    {
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
            continue;
        else if(ptr->d_type == 8 || ptr->d_type == 10)    ///file
        {
            string foo(basePath);
            string bar(ptr->d_name);
            foo = foo + "/" + bar;
            files.push_back(foo);
            // cout << foo << endl;
        }   
        else if(ptr->d_type == 4)    ///dir
        {
            memset(base,'\0',sizeof(base));
            strcpy(base,basePath);
            strcat(base,"/");
            strcat(base,ptr->d_name);
            getFiles(string(base), files);
        }
    }
    closedir(dir);
}

int getAllLuvImage(std::vector<mMat>& _v, const string& path)
{
	vector<string> files;
	getFiles(path, files);
	
	int cnt = 0, filesNum = files.size();
	cout << filesNum << endl;
	for (int i = 0; i < filesNum; ++i)
	{
		Mat _m = imread(files[i]);
		if (_m.data != NULL)
		{
			cvtColor(_m, _m, CV_BGR2Luv);
			_v.push_back(mMat(_m, files[i]));	
			++cnt;
		}
	}
	return cnt;
}

void getMaxMin(const vector<mMat>& _v)
{
	int max1 = -1, max2 = -1, max3 = -1;
	int min1 = 123456, min2 = 123456, min3 = 123456;
	for (int i = 0; i < _v.size(); ++i)
	{
		for (int j = 0; j < _v[i].m.rows; ++j)
			for (int k = 0; k < _v[i].m.cols; ++k)
			{
				max1 = max(max1, (int)_v[i].m.at<Vec3b>(j, k)[0]);
				min1 = min(min1, (int)_v[i].m.at<Vec3b>(j, k)[0]);

				max2 = max(max2, (int)_v[i].m.at<Vec3b>(j, k)[1]);
				min2 = min(min2, (int)_v[i].m.at<Vec3b>(j, k)[1]);

				max3 = max(max3, (int)_v[i].m.at<Vec3b>(j, k)[2]);
				min3 = min(min3, (int)_v[i].m.at<Vec3b>(j, k)[2]);		
			}
	}

	cout << "1: (" << min1 << ", " << max1 << ")\n"
		<< "2: (" << min2 << ", " << max2 << ")\n"
		<< "3: (" << min3 << ", " << max3 << ")\n";
}

void getHistogram(vector<LuvHist>& lVec, const vector<mMat>& _v, int lNum, int uNum, int vNum)
{
	double* vec = new double[lNum * uNum * vNum];
	for (int i = 0; i < _v.size(); ++i)
	{
		// cout << "getHistogram, " << i << endl;
		string file = _v[i].file;
		
		for (int p = 0; p < lNum * uNum * vNum; ++p)
			vec[p] = 0;

		int vRows = _v[i].m.rows, vCols = _v[i].m.cols;
		for (int j = 0; j < vRows; ++j)
			for (int k = 0; k < vCols; ++k)
			{
				const int LSIZE = 256, USIZE = 256, VSIZE = 256;
				int lIndex = _v[i].m.at<Vec3b>(j, k)[0] / ceil((double)LSIZE / lNum),
					uIndex = _v[i].m.at<Vec3b>(j, k)[1] / ceil((double)USIZE / uNum),
					vIndex = _v[i].m.at<Vec3b>(j, k)[2] / ceil((double)VSIZE / vNum);
				int index = lIndex * uNum * vNum + uIndex * vNum + vIndex;
				vec[index] += 1;
			}

		for (int p = 0; p < lNum * uNum * vNum; ++p)
			vec[p] /= (vRows * vCols);
		
		vector<double> foo;
		for (int p = 0; p < lNum * uNum * vNum; ++p)
			foo.push_back(vec[p]);
		lVec.push_back(LuvHist(foo, lNum * uNum * vNum, file));
	}
	delete[] vec;
}

void showLuvVec(const vector<LuvHist>& luvHistVec)
{
	for (int i = 0; i < luvHistVec.size(); ++i)
	{
		cout << "(";
		for (int j = 0; j < luvHistVec[i].size - 1; ++j)
			cout << luvHistVec[i].vec[j] << ", ";
		cout <<  luvHistVec[i].vec[luvHistVec[i].size - 1] << ")\n";
	}	
}

double getDistance(LuvHist& a, LuvHist& b)
{
	// Bhattacharyya 
	double sum = 0;
	for (int i = 0; i < a.size; ++i)
		sum +=  sqrt(a.vec[i] * b.vec[i]);
	return sqrt(1 - sum);
}


// Mean Shift
double bandwidth_square = 0.75 * 0.75;
double stopthresh_square = 0.075;
const double EPSILON = 1e-7;
const double CLUSTER_EPSILON = 0.5;
 

double euclidean_distance_square(LuvHist &a, LuvHist &b){\
	// Bhattacharyya 
	double sum = 0;
	for (int i = 0; i < a.size; ++i)
		sum +=  sqrt(a.vec[i] * b.vec[i]);
	return sqrt(1 - sum);


    double distance_square = 0;
    int dim = (int)a.vec.size();
    for (int i=0; i<dim; i++) {
        distance_square += (a.vec[i]-b.vec[i])*(a.vec[i]-b.vec[i]);
    }
    return distance_square;
}

LuvHist add(const LuvHist &a, const LuvHist &b){
    vector<double> sum;

    int dim = (int)a.vec.size();
    // cout << dim << ", " <<  b.vec.size() << endl;
    for (int i=0; i<dim; i++) {
        sum.push_back(a.vec[i] + b.vec[i]);
    }
    // cout << "fuck1.2.1\n";
    return LuvHist(sum, dim, "");
    // return res;
}

LuvHist subtract(const LuvHist &a, const LuvHist &b){
    vector<double> sum;
    int dim = (int)a.vec.size();
    // cout << dim << ", " <<  b.vec.size() << endl;
    for (int i = 0; i < dim; i++) {
    	// cout << a.vec[i] << ", " << b.vec[i] << endl;
        sum.push_back(a.vec[i] - b.vec[i]);
    }
    // cout << "fuck 1.1.1\n";
    return LuvHist(sum, dim, "");
}

LuvHist get_cluster_center(vector<LuvHist>& all_points, int mode){
    // mode 0: most points neighbor center
    // random select
    cout << "sss\n";
    int index = rand() % all_points.size();
    cout << index << endl;

    LuvHist center_point = all_points[index];
    while (1) {
        vector<LuvHist> M;
        for (int i=0; i<all_points.size(); i++) {
            if (bandwidth_square > euclidean_distance_square(center_point, all_points[i])) {
                M.push_back(all_points[i]);
            }
        }
        // cout << "fuck1\n";
        vector<double> foo;
        for (int i = 0; i < all_points[0].size; ++i)
        	foo.push_back(0);
        LuvHist shift(foo, all_points[0].size, "");
        for (int i=0; i<M.size(); i++) {
        	// cout << "fuck1.1\n";
            LuvHist delta = subtract(M[i], center_point);
            // cout << "fuck1.2\n";
            shift = add(shift, delta);
            
        }
                // cout << "fuck2\n";
        center_point = add(center_point, shift);
                // cout << "fuck3\n";
        double shift_square = 0;
        for (int i=0; i<shift.vec.size(); i++) {
            shift_square += shift.vec[i]*shift.vec[i];
        }
        cout << "distance " << shift_square << endl;
                // cout << "fuck4\n";
        if (stopthresh_square > shift_square) {
        	// cout << "size = " << M.size() 
        		// << " threshold = " << stopthresh_square << endl;
        	if (M.size() == 0)
        	{
        		// stopthresh_square *= 2;
        		// cout << "new "
        	}
        	else
        	{
        		int r = rand() % M.size();
        	    return M[r];
        	}
            
        }
    }
    return LuvHist();
}

int main()
{
	string folderPath = "dogs_and_cats_in_beach";
	vector<mMat> _v;
	int imageNum = getAllLuvImage(_v, folderPath);
	cout << imageNum << endl;

	// getMaxMin(_v);

	vector<LuvHist> luvHistVec;
	getHistogram(luvHistVec, _v, 3, 2, 4);

	// showLuvVec(luvHistVec);	

	get_cluster_center(luvHistVec, 0);
	// cout << res.file << endl;

	return 0;
}