#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <set>
#include <stack>
#include "cv_inc.h"
using namespace std;

struct	Point
{
	double	x, y;

	Point(){}
	Point(double x0, double y0) : x(x0), y(y0) {}
	Point(const Point& p0) : x(p0.x), y(p0.y) {}
};

typedef	vector<Point> PointSet;

typedef cv::Mat_<cv::Vec3b> ColorImage;
typedef cv::Mat_<unsigned char> GreyImage;

const	double		EPS			= 1e-8;		// Epsilon (zero value)
const	double		INFINITY		= 1e20;

template<typename T> inline int round(T x) { return ((x)>0 ? (int)((x)+0.5) : -(int)(0.5-(x)))	; }
template<typename T> inline T sqr(T x) { return x * x; }

inline	int	doubleCmp(double a, double b) { if (fabs(a - b) < EPS)  return 0; 	return a < b ? -1 : +1;}

template <typename T>
void ShowImage(const char* name, const T& img)
{
	IplImage a = img;
	cvShowImage(name, &a);
}

template <typename T>
void ShowImage(const string& name, const T& img)
{
	ShowImage(name.c_str(),img);
}

template <typename T>
void SaveImage(const char* name, const T& img)
{
	IplImage a = img;
	cvSaveImage(name, &a);
}

template <typename T>
void SaveImage(const string& name, const T& img)
{
	SaveImage(name.c_str(), img);
}