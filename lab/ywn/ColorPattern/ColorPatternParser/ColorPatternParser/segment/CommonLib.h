#pragma once

#pragma warning(disable:4996)

/************************************************************************/
/* Common libraries and header files.                                    */
/************************************************************************/
#ifndef _AFXDLL
#define _AFXDLL
#endif

using namespace std;

/************************************************************************/
/* Precision control                                                    */
/*		Currently support for CmGMM										*/
/************************************************************************/
//#define DOUBLE_PRECISION
#ifdef DOUBLE_PRECISION
	#define Real double
	#define CV_TYPE CV_64FC1
#else
	#define Real float
	#define CV_TYPE CV_32FC1
#endif // DOUBLE_PRECISION

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif


typedef double Complex[2];

extern CPoint const DIRECTION8[8];
extern CPoint const DIRECTION4[4];

typedef float (*CmColorPtr32F)[3];
typedef double (*CmColorPtr64F)[3];
typedef byte (*CmColorPtr8U)[3];

/*************** Global functions *****************/
const	double		EPS			= 1e-8;		// Epsilon (zero value)
const	double		INFINITY		= 1e20;

inline	int	doubleCmp(double a, double b) { if (fabs(a - b) < EPS)  return 0; 	return a < b ? -1 : +1;}
void NormalizeVec(float *v, int num, float maxV, float minV);

struct	Point
{
	double	x, y;

	Point(){}
	Point(double x0, double y0) : x(x0), y(y0) {}
	Point(const Point& p0) : x(p0.x), y(p0.y) {}
};

/************************************************************************/
/* Useful template                                                      */
/************************************************************************/

template<typename T> inline int round(T x) { return ((x)>0 ? (int)((x)+0.5) : -(int)(0.5-(x)))	; }
template<typename T> inline T sqr(T x) { return x * x; }

struct rgb
{
	byte r, g, b; 
	rgb():r(0),g(0),b(0)
	{}
	rgb(byte r, byte g, byte b):r(r),g(g),b(b)
	{}
	rgb(DWORD val):r(GetRValue(val)),g(GetGValue(val)),b(GetBValue(val))
	{}
};

inline bool operator==(const rgb &a, const rgb &b) {
	return ((a.r == b.r) && (a.g == b.g) && (a.b == b.b));
}



template<typename T> vector<T> operator +(const vector<T>& v1, const vector<T> &v2)
{
	vector<T> result(v1);
	for (size_t i = 0, iEnd = v1.size(); i != iEnd; i++)
		result[i] = v1[i] + v2[i];
	return result;
}

template<typename T> vector<T> operator -(const vector<T>& v1, const vector<T> &v2)
{
	vector<T> result(v1);
	for (size_t i = 0, iEnd = v1.size(); i != iEnd; i++)
		result[i] = v1[i] - v2[i];
	return result;
}

template<typename T> vector<T> operator *(const vector<T>& v1, const vector<T> &v2)
{
	vector<T> result(v1);
	for (size_t i = 0, iEnd = v1.size(); i != iEnd; i++)
		result[i] = v1[i] * v2[i];
	return result;
}

template<typename T> vector<T> operator /(const vector<T>& v1, const vector<T> &v2)
{
	vector<T> result(v1);
	for (size_t i = 0, iEnd = v1.size(); i != iEnd; i++)
		result[i] = v1[i] * v2[i];
	return result;
}

// random color
__inline cv::Vec3b random_rgb(){ 
	cv::Vec3b c;

	c[0] = (uchar)(rand() % 200 + 55);
	c[1] = (uchar)(rand() % 200 + 55);
	c[2] = (uchar)(rand() % 200 + 55);

	return c;
}

/************************************************************************/
/* Define Macros                                                        */
/************************************************************************/
#define CmAssertM(msg, Condition)                                        \
{                                                                       \
	if( !(Condition) )                                                  \
	{																	\
		cvError(CV_StsInternal, __FUNCTION__,  "Assertion: " #msg #Condition " failed", __FILE__, __LINE__); \
		exit(1);														\
	}																	\
}

#define CmAssert(Condition)                                        \
{                                                                       \
	if( !(Condition) )                                                  \
{																	\
	cvError(CV_StsInternal, __FUNCTION__,  "Assertion: " #Condition " failed", __FILE__, __LINE__); \
	exit(1);														\
}																	\
}

#define CmAssertImgFormatSame(img1, img2)								\
{																		\
	if ((img1)->width != (img2)->width || (img1)->height != (img2)->height)											\
	cvError(CV_StsInternal, __FUNCTION__, "Assertion images: " #img1 "&" #img2 " size match failed", __FILE__, __LINE__); 	\
	if ((img1)->nChannels != (img2)->nChannels)							\
	cvError(CV_StsInternal, __FUNCTION__, "Assertion images: " #img1 "&" #img2 " channel match failed", __FILE__, __LINE__); \
	if ((img1)->depth != (img2)->depth)							\
	cvError(CV_StsInternal, __FUNCTION__, "Assertion images: " #img1 "&" #img2 " depth match failed", __FILE__, __LINE__); \
}		

#define BUFFER_SIZE 1024
extern char gTmpStrBuf[BUFFER_SIZE];

const char* CmSprintf(const char* format, ...);

//template<typename T> inline T square(T x) {return x*x;}
int CmSgn(double number); 

#define CHECK_IND(c, r) (c >= 0 && c < m_nWidth && r >= 0 && r < m_nHeight)

typedef pair<double, CPoint*> ValPntP;
typedef pair<double, CPoint> ValPnt;

typedef cv::Mat_<cv::Vec3b> ColorImage;
typedef cv::Mat_<unsigned char> GreyImage;
typedef cv::Mat_<int> IntImage;
typedef	vector<Point> PointSet;
typedef pair<int,int> int_pair;

struct ImageSize
{
	int xlen,ylen;
	ImageSize():xlen(0),ylen(0)
	{}
	ImageSize(int xlen, int ylen)
	{
		Init(xlen,ylen);
	}
	void Init(int xlen, int ylen)
	{
		this->xlen = xlen;
		this->ylen = ylen;
	}
	int Index(int x, int y)
	{
		return y*xlen+x;
	}
	bool InRange(int x,int y)
	{
		return x>=0&&x<xlen&&y>=0&&y<ylen;
	}
	cv::Size ToCv()
	{
		return cv::Size(xlen,ylen);
	}
	int TotalSize()
	{
		return xlen*ylen;
	}
};

template<typename T>
struct Array2
{
	T* data;
	ImageSize box;
	Array2():data(NULL)
	{}
	Array2(int xlen, int ylen):data(NULL)
	{
		Init(xlen, ylen, false);
	}
	Array2(int xlen, int ylen, bool setZero):data(NULL)
	{
		Init(xlen, ylen, setZero);
	}
	T& operator () (int x, int y)
	{
		_ASSERTE(x>=0&&x<box.xlen&&y>=0&&y<box.ylen);
		return data[box.Index(x,y)];
	}
	T& operator [] (const cv::Point& v)
	{
		_ASSERTE(v.x>=0&&v.x<box.xlen&&v.y>=0&&v.y<box.ylen);
		return data[box.Index(v.x,v.y)];
	}
	~Array2()
	{
		delete[] data;
	}

	void FastSetZero()
	{
		if(data==NULL) return;
		memset(data,0,sizeof(T)*box.TotalSize());
	}

	void Init(int xlen, int ylen)
	{
		Init(xlen,ylen,false);
	}

	void Init(int xlen, int ylen, bool setZero)
	{
		box.Init(xlen,ylen);
		int total=box.TotalSize();
		data = new T[total];
		if(setZero)
			memset(data,0,sizeof(T)*total);
	}
};

#ifdef _DEBUG
#pragma comment(linker, "/NODEFAULTLIB:[mfc90ud.lib,mfcs90ud.lib]")
#else
#pragma comment(linker, "/NODEFAULTLIB:[mfc90u.lib,mfcs90u.lib]")
#endif // _DEBUG
