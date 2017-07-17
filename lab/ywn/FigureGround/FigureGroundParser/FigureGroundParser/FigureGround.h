#pragma once
#include <opencv2/highgui.hpp>
#include <fstream>
#include <string>

using std::string;

typedef cv::Mat_<cv::Vec3b> ColorImage;


class FigureGround
{
public:
	//在cmm cvpr得到的灰度图的基础上，得到多个前景的黑白图
	int char2int(char a);
	uchar char2uchar(char a);
	void showMask(cv::Mat mask,char * winname);
	cv::Mat getMask(IplImage *rc_img,int threshold);
	cv::Mat globalProcess(const char* filename,const char* RCfilename,int threshold,int iteCount,int gcFact,int eroFact,int dilFact,bool show);
	void saveMask(cv::Mat mask,const char * filename);
	int getThreshold(IplImage *img,int ratio);

	//在saliency image的参考下，判断figure-ground class：图底分离，图底共同构成画面
	double JudgeTextureComplexity(IplImage* imgRGB1);
	void JudgeTextureComplexity_saliency(IplImage* srcImg, IplImage* salImg, double &srcComp, double &salComp);

	double CalColorDif(ColorImage& srcImg, ColorImage& salImg, /*output*/double& areaDif, double& colorDif);
	int ClassifyFigure_ground(string srcname, string salname,double times=1);
	void testPerformance_figureGround(int times=14);
	void testClassifyFigure_ground(); // for testing one image
	void testClassifyFigure_ground_bat(); //for testing many images
	void CalParamforOneImage(string srcname, string salname,/*output*/double& areaDif, double& colorDif, double &srcComp, double &salComp);
	void CalParam(string srcpic);
};