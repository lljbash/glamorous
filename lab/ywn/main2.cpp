#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

class mPoint{
public:
	int x, y;
	uchar gray;
	mPoint() {};
	mPoint(int _x, int _y, uchar _g) : x(_x), y(_y), gray(_g) {};
};


// global var
bool** isVisit;
int cnt;
Mat _m;


bool cmp(const mPoint& a, const mPoint& b)
{
    return a.gray < b.gray;
}

mPoint go(int x, int y, Mat& resMat, const int& NUM, int deep)
{
	if (isVisit[x][y])
		return mPoint(x,y,deep);

	isVisit[x][y] = true;
	resMat.at<uchar>(x, y) = _m.at<uchar>(x, y);
	++cnt;

	// recursion
	vector<mPoint> fooVec;
	if (!isVisit[min(x + 1, _m.rows - 1)][y])
		fooVec.push_back(mPoint(min(x + 1, _m.rows - 1), y, _m.at<uchar>(min(x + 1, _m.rows - 1), y)));
	if (!isVisit[x][ min(y + 1, _m.cols - 1)])
		fooVec.push_back(mPoint(x, min(y + 1, _m.cols - 1), _m.at<uchar>(x, min(y + 1, _m.cols - 1))));
	if (!isVisit[max(x - 1, 0)][y])
		fooVec.push_back(mPoint(max(x - 1, 0), y, _m.at<uchar>(max(x - 1, 0), y)));
	if (!isVisit[x][ max(y - 1, 0)])
		fooVec.push_back(mPoint(x, max(y - 1, 0), _m.at<uchar>(x, max(y - 1, 0))));	
	
	
	sort(fooVec.begin(), fooVec.end(), cmp);

	if (fooVec.size()==0)
		return mPoint(x,y,deep);
	for (int i = 0; i < 1; ++i)
		return go(fooVec[i].x, fooVec[i].y, resMat, NUM, deep + 1);

}

void generateVideo(const int& FRAME_NUM)
{ 
    string outputVideoName = "output.avi";  
      
    VideoWriter writer;  
      
    int codec = 0;  
    int frameRate = 20;  
    Size frameSize;  

    for (int i = 1; i <= FRAME_NUM; ++i)  
    {         
        stringstream ss;
        ss << i;
        string fileName = ss.str() + ".jpg";
        Mat frame = imread(fileName);
        if (!writer.isOpened())  
        {  
            frameSize.width  = frame.cols;  
            frameSize.height = frame.rows;  
  
            if (!writer.open(outputVideoName, CV_FOURCC('D','I','V','X') , frameRate, frameSize, true))  
            {  
                cout << "open writer error..." << endl;  
                return;  
            }  
        }   
        writer.write(frame); 
        system(("rm " + fileName).c_str());
	}
 
}


mPoint getNextPoint(mPoint& mp){
	int dist = 0;
	int x = mp.x;
	int y = mp.y;
	while (1)
	{
		++dist;
		int _l = max(0, y - dist);
		int _r = min(_m.cols - 1, y + dist);
		int _t = max(0, x - dist);
		int _b = min(_m.rows - 1, x + dist);
		if ((_r - _l) * (_b - _t) == (_m.cols - 1) * (_m.rows - 1))
			break;

		for (int i = _l; i <= _r; ++i)
		{
			if (!isVisit[_t][i])
				return mPoint(_t, i, -1);
			if (!isVisit[_b][i])
				return mPoint(_b, i, -1);
		}	
		for (int i = _t; i <= _b; ++i)
		{
			if (!isVisit[i][_l])
				return mPoint(i, _l, -1);
			if (!isVisit[i][_r])
				return mPoint(i, _r, -1);
		}	
	}
	return mPoint(-1, -1, -1);
}

int main(int argc, char* argv[])
{
	srand((unsigned int) time(0));

	string srcPath(argv[1]);
	_m = imread(srcPath, CV_LOAD_IMAGE_GRAYSCALE);
	vector<mPoint> pVec;
	for (int i = 0; i < _m.rows; ++i)
		for (int j = 0; j < _m.cols; ++j)
			if (_m.at<uchar>(i, j) != 255)
				pVec.push_back(mPoint(i, j, _m.at<uchar>(i, j)));
	
	cout << "total = " << _m.cols << " * " <<  _m.rows << ", points = " << pVec.size() << endl;

	const int FRAME_NUM = 400;
	const int NUM = pVec.size() / FRAME_NUM;	


	// init 
	isVisit = new bool*[_m.rows];
	for (int i = 0; i < _m.rows; ++i)
		isVisit[i] = new bool[_m.cols];
	for (int i = 0; i < _m.rows; ++i)
		for (int j = 0; j < _m.cols; ++j)
			isVisit[i][j] = (_m.at<uchar>(i, j) == 255);

	cnt = 0;
	Mat resMat = 255 * Mat::ones(_m.rows, _m.cols, CV_8UC1);
	int n_cnt = 0;
	int rx = rand() % _m.rows, ry = rand() % _m.cols;
	mPoint next(rx, ry, 0);
	while (cnt < pVec.size() * 999 / 1000)
	{
		mPoint end = go(next.x, next.y, resMat, NUM, 0);
		next = getNextPoint(end);
		if (next.x == -1 && next.y == -1)
			break;
		
		int fooDeep = end.gray;
		if (fooDeep > min(_m.cols / 8, _m.rows / 8))
		{
			n_cnt ++ ;
			stringstream ss;
			ss << n_cnt;
			string fooStr = ss.str() + ".jpg";
			imwrite(fooStr, resMat);
		}
	}
	

	generateVideo(n_cnt);


	// free memory
	for (int i = 0; i < _m.rows; ++i)
		delete[] isVisit[i];
	delete[] isVisit;

	return 0;
}