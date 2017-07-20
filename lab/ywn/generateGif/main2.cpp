#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <queue>

using namespace std;
using namespace cv;

class mPoint{
public:
	int x, y;
	uchar gray;
	mPoint() {};
	mPoint(int _x, int _y, uchar _g) : x(_x), y(_y), gray(_g) {};
};


bool** isVisit;
bool** bfs_flag;
int cnt;
Mat _m;

int randomDraw(vector<mPoint>& pVec, const int& N, Mat& resMat)
{
	int NUM = min(N, int(pVec.size()));
	// cout << NUM << endl;
	for (int i = 0; i < NUM; ++i)
	{
		int randIndex = rand() % pVec.size();
		resMat.at<uchar>(pVec[randIndex].x, pVec[randIndex].y) = pVec[randIndex].gray;
		pVec.erase(pVec.begin() + randIndex);
	}
	
	return NUM;
}

bool cmp(const mPoint& a, const mPoint& b)
{
    return a.gray < b.gray;
}

mPoint go(int x, int y, Mat& resMat, const int& NUM, int deep)
{
	if (isVisit[x][y] || deep > 5000) 
		return mPoint(x,y,deep);
	
	// cout << "GO: " << x << ", " << y << endl;
	isVisit[x][y] = true;
	resMat.at<uchar>(x, y) = _m.at<uchar>(x, y);
	++cnt;
	// if ((++cnt) % NUM == 0)
	// {
	// 	stringstream ss;
	// 	ss << (cnt / NUM);
	// 	string fooStr = ss.str() + ".jpg";
	// 	imwrite(fooStr, resMat);
	// }

	// recursion
	vector<mPoint> fooVec;
	if (!isVisit[min(x + 1, _m.rows - 1)][y])
	{
		fooVec.push_back(mPoint(min(x + 1, _m.rows - 1), y, _m.at<uchar>(min(x + 1, _m.rows - 1), y)));
	}
	if (!isVisit[x][ min(y + 1, _m.cols - 1)])
	{
		fooVec.push_back(mPoint(x, min(y + 1, _m.cols - 1), _m.at<uchar>(x, min(y + 1, _m.cols - 1))));
	}

	if (!isVisit[max(x - 1, 0)][y])
	{
		fooVec.push_back(mPoint(max(x - 1, 0), y, _m.at<uchar>(max(x - 1, 0), y)));
	}
	if (!isVisit[x][ max(y - 1, 0)])
	{
		fooVec.push_back(mPoint(x, max(y - 1, 0), _m.at<uchar>(x, max(y - 1, 0))));	
	}
	
	
	
	
	sort(fooVec.begin(), fooVec.end(), cmp);
	// for (int i = 0; i < fooVec.size(); ++i)
	// 	cout << int(fooVec[i].gray) << ", ";
	// cout << endl;
	if (fooVec.size()==0)
	{
		return mPoint(x,y,deep);
	}
	for (int i = 0; i < 1; ++i)
		return go(fooVec[i].x, fooVec[i].y, resMat, NUM, deep + 1);

}

void generateVideo(const int& FRAME_NUM)
{ 
    string outputVideoName = "output.avi";  
      
    VideoWriter writer;  
      
    int codec = 0;  
    int frameRate = 25;  
    Size frameSize;  

    for (int i = 1; i < FRAME_NUM; ++i)  
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

mPoint bfs_next(mPoint& mp){

	for (int i = 0; i < _m.rows; ++i)
		for (int j = 0; j < _m.cols; ++j)
			bfs_flag[i][j] = 0;

	queue<mPoint> q;
	q.push(mp);
	int cyc_cnt = 0;
	int size = q.size();
	while(!q.empty()){
		cyc_cnt++;
		if(cyc_cnt>10000) {
			cout<<cyc_cnt<<" "<<q.size()<<endl;
			break;
		}

		if (q.size()-size > 1)
		{
			cout<<cyc_cnt<<" "<<q.size()<<endl;	

		}
		size = q.size();
		mPoint p = q.front();
		q.pop();
		bfs_flag[p.x][p.y] = 1;

		// find neighbor if possible
		int x = p.x;
		int y = p.y;
		if (!isVisit[min(x + 1, _m.rows - 1)][y])
		{
			return mPoint(min(x + 1, _m.rows - 1), y, _m.at<uchar>(min(x + 1, _m.rows - 1), y));
		}
		if (!isVisit[x][ min(y + 1, _m.cols - 1)])
		{
			return mPoint(x, min(y + 1, _m.cols - 1), _m.at<uchar>(x, min(y + 1, _m.cols - 1)));
		}

		if (!isVisit[max(x - 1, 0)][y])
		{
			return mPoint(max(x - 1, 0), y, _m.at<uchar>(max(x - 1, 0), y));
		}
		if (!isVisit[x][ max(y - 1, 0)])
		{
			return mPoint(x, max(y - 1, 0), _m.at<uchar>(x, max(y - 1, 0)));	
		}
		if(!bfs_flag[min(x+1,_m.rows-1)][y])
			q.push(mPoint(min(x + 1, _m.rows - 1), y, _m.at<uchar>(min(x + 1, _m.rows - 1), y)));
		if(!bfs_flag[x][ min(y + 1, _m.cols - 1)])
			q.push(mPoint(x, min(y + 1, _m.cols - 1), _m.at<uchar>(x, min(y + 1, _m.cols - 1))));
		if(!bfs_flag[max(x - 1, 0)][ y])
			q.push(mPoint(max(x - 1, 0), y, _m.at<uchar>(max(x - 1, 0), y)));
		if(!bfs_flag[x] [max(y - 1, 0)])
			q.push(mPoint(x, max(y - 1, 0), _m.at<uchar>(x, max(y - 1, 0))));	
	}

	return mPoint(-1,-1,0);
}

mPoint ch_next(mPoint& mp){
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

int main()
{
	srand((unsigned int) time(0));

	string srcPath = "a.jpg";
	_m = imread(srcPath, CV_LOAD_IMAGE_GRAYSCALE);
	vector<mPoint> pVec;
	for (int i = 0; i < _m.rows; ++i)
		for (int j = 0; j < _m.cols; ++j)
			if (_m.at<uchar>(i, j) != 255)
				pVec.push_back(mPoint(i, j, _m.at<uchar>(i, j)));
	
	cout << "total = " << _m.cols << " * " <<  _m.rows << ", points = " << pVec.size() << endl;

	const int FRAME_NUM = 400;
	const int NUM = pVec.size() / FRAME_NUM;	
/*	Mat resMat = 255 * Mat::ones(_m.rows, _m.cols, CV_8UC1);
	cout << "resMat = " << resMat.cols << " * " <<  resMat.rows << endl;
	int cnt = 0;
	while (1)
	{
		cout << cnt << endl;
		int ret = randomDraw(pVec, NUM, resMat);
		if (ret == 0)
			break;
		stringstream ss;
		ss << (cnt++);
		string fooStr = ss.str() + ".jpg";
		imwrite(fooStr, resMat);
		// waitKey(0);
	}
*/

	bfs_flag = new bool*[_m.rows];
	for (int i = 0; i < _m.rows; ++i)
		bfs_flag[i] = new bool[_m.cols];
	for (int i = 0; i < _m.rows; ++i)
		for (int j = 0; j < _m.cols; ++j)
			bfs_flag[i][j] = 0;

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
	mPoint next(rx,ry,0);
	while (cnt < pVec.size() * 95 / 100)
	{
		// cout << cnt << endl;
		// int rx = rand() % _m.rows, ry = rand() % _m.cols;
		
		// cout << rx << ", " << ry << ", " << int(_m.at<uchar>(rx, ry)) << endl;
		mPoint end = go(next.x, next.y, resMat, NUM, 0);
		// next = bfs_next(end);
		next = ch_next(end);
		// printf("%d %d\n", next.x, next.y);
		if (next.x==-1 && next.y==-1)
		{	
			int sum_bfs = 0;
			for (int i = 0; i < _m.rows; ++i)
				for (int j = 0; j < _m.cols; ++j)
					sum_bfs += bfs_flag[i][j];
			printf("%d\n", sum_bfs);
			break;
		}
		// cout<<d<<endl;
		if (end.gray>60)
		{
			n_cnt ++ ;
			stringstream ss;
			ss << n_cnt;
			string fooStr = ss.str() + ".jpg";
			imwrite(fooStr, resMat);
		}
	}
	

	generateVideo(n_cnt);

	for (int i = 0; i < _m.rows; ++i)
		delete[] isVisit[i];
	delete[] isVisit;

	for (int i = 0; i < _m.rows; ++i)
		delete[] bfs_flag[i];
	delete[] bfs_flag;

	return 0;
}