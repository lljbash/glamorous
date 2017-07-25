#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <algorithm>

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
	cout << "SIZE: " << _v.size() << endl;

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
				int lIndex = _v[i].m.at<Vec3b>(j, k)[0] / (LSIZE / lNum),
					uIndex = _v[i].m.at<Vec3b>(j, k)[1] / (USIZE / uNum),
					vIndex = _v[i].m.at<Vec3b>(j, k)[2] / (VSIZE / vNum);
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

int main()
{
	string folderPath = "dogs_and_cats_in_beach";
	vector<mMat> _v;
	int imageNum = getAllLuvImage(_v, folderPath);
	cout << imageNum << endl;

	// getMaxMin(_v);

	vector<LuvHist> luvHistVec;
	getHistogram(luvHistVec, _v, 2, 2, 2);

	for (int i = 0; i < luvHistVec.size(); ++i)
	{
		cout << "(";
		for (int j = 0; j < luvHistVec[i].size - 1; ++j)
			cout << luvHistVec[i].vec[j] << ", ";
		cout <<  luvHistVec[i].vec[luvHistVec[i].size - 1] << ")\n";
	}

	// cout << _v[0] << endl;

	return 0;
}