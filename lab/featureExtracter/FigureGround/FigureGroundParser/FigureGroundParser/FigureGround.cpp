#include <iostream>
#include <String>
#include <math.h>
#include <opencv2/opencv.hpp> 
#include "FigureGround.h"

using namespace std;

typedef unsigned char BYTE;
typedef unsigned char byte;

int FigureGround::char2int(char a)
{
	return ((int)a)&(0x000000ff);
}

uchar FigureGround::char2uchar(char a)
{
	return a-128;
}

cv::Mat FigureGround::globalProcess(const char* filename,const char* RCfilename,int threshold,int iteCount,int gcFact,int eroFact,int dilFact,bool show)
{
	//read the file;
	IplImage *img=cvLoadImage(filename,1);
	cv::Mat img2 = cvLoadImage(filename,1);
	IplImage *rc_img=cvLoadImage(RCfilename,1);

	//get the initial mask
	cv::Mat mask=getMask(rc_img,threshold);
	if (show) showMask(mask,"initmask");
	int n=0;
	int height=img->height;
	int width=img->width;
	CvRect rect;
	rect.x=0;
	rect.y=0;
	rect.height=height;
	rect.width=width;
	cv::Mat fgdModel,bgdModel;
	cv::Mat matErode=cv::Mat(height,width,CV_8UC1);
	cv::Mat matDilate=cv::Mat(height,width,CV_8UC1);
	IplImage iplMask=mask;
	IplImage iplErode=matErode;
	IplImage iplDilate=matDilate;
	
	while(true)
	{
		n++;
		cv::grabCut(img2,mask,rect,bgdModel,fgdModel,gcFact,cv::GC_INIT_WITH_MASK);
		if (show) showMask(mask,"result");
		
		//normalize
		for (int i=0;i<height;i++)
			for (int j=0;j<width;j++)
			{
				if (mask.data[mask.step*i+j]==cv::GC_FGD || mask.data[mask.step*i+j]==cv::GC_PR_FGD )
					mask.data[mask.step*i+j]=cv::GC_FGD;
				else
					mask.data[mask.step*i+j]=cv::GC_BGD;
			}
		if (show) showMask(mask,"normalize");
		if (n==iteCount)
			break;	
		cvErode(&iplMask,&iplErode,0,eroFact);
		cvDilate(&iplMask,&iplDilate,0,dilFact);
		if (show) showMask(matErode,"erode");
		if (show) showMask(matDilate,"dilate");
		
		//generate the new mask
		bool hasFGD=false;
		for (int i=0;i<height;i++)
			for (int j=0;j<width;j++)
			{
				if (matErode.data[matErode.step*i+j]==cv::GC_FGD)
				{
					mask.data[mask.step*i+j]=cv::GC_PR_FGD;
					hasFGD=true;
				}
				else if (matDilate.data[matDilate.step*i+j]==cv::GC_BGD)
					mask.data[mask.step*i+j]=cv::GC_BGD;
				else
					mask.data[mask.step*i+j]=cv::GC_PR_BGD;
			}
		if (show) showMask(mask,"newMask");
		if (!hasFGD) break;
	}
	return mask;
}

cv::Mat FigureGround::getMask(IplImage *rc_img,int ratio)
{
	int threshold=ratio;//getThreshold(rc_img,ratio);
	cv::Mat mask=cv::Mat(rc_img->height,rc_img->width,CV_8UC1);
	for (int i=0;i<rc_img->height;i++)
		for (int j=0;j<rc_img->width;j++)
		{
			if (char2int(rc_img->imageData[i*rc_img->widthStep+j*3])>threshold)
				mask.data[mask.step*i+j]=cv::GC_PR_FGD;
			else
				mask.data[mask.step*i+j]=cv::GC_PR_BGD;
		}
	return mask;
}

void FigureGround::showMask(cv::Mat mask,char * winname)
{
	cv::Mat show=cv::Mat(mask.rows,mask.cols,CV_8UC1);
	for(int i=0;i<show.rows;i++)
		for (int j=0;j<show.cols;j++)
			{
				int k=mask.data[i*mask.step+j];
				if (k==cv::GC_FGD)
					show.data[i*show.step+j]=255;
				else if (k==cv::GC_BGD)
					show.data[i*show.step+j]=0;
				else if (k==cv::GC_PR_BGD)
					show.data[i*show.step+j]=64;
				else if (k==cv::GC_PR_FGD)
					show.data[i*show.step+j]=198;
			}
	cv::namedWindow(winname, CV_WINDOW_AUTOSIZE); //创建窗口
	cv::imshow(winname, show); //显示图像
	cv::waitKey();
}

void FigureGround::saveMask(cv::Mat mask,const char * filename)
{
	IplImage* show=cvCreateImage(cvSize(mask.cols,mask.rows),8,1);

	printf("%s\n", filename);
	printf("%d\n%d\n", show->height, show -> width);

	for(int i=0;i<show->height;i++)
		for (int j=0;j<show->width;j++)
			{
				int k=mask.data[i*mask.step+j];
				if (k==cv::GC_FGD)
					CV_IMAGE_ELEM(show,BYTE,i,j)=255;
				else if (k==cv::GC_BGD)
					CV_IMAGE_ELEM(show,BYTE,i,j)=0;
				else if (k==cv::GC_PR_BGD)
					CV_IMAGE_ELEM(show,BYTE,i,j)=64;
				else if (k==cv::GC_PR_FGD)
					CV_IMAGE_ELEM(show,BYTE,i,j)=198;
			}
		cvSaveImage(filename,show);
		cvReleaseImage(&show);
}

int FigureGround::getThreshold(IplImage *img,int ratio)
{
	int n[256];
	for (int i=0;i<256;i++)
		n[i]=0;
	for (int i=0;i<img->height;i++)
		for (int j=0;j<img->width;j++)
		{
			int value=char2int(img->imageData[i*img->widthStep+j*3]);
			n[value]++;
		}
	int size=img->width*img->height;
	cout<<size<<endl;
	int sum=0;
	int i=255;
	for (i=255;i>=0;i--)
	{
		sum+=n[i];
		cout<<i<<" "<<n[i]<<" "<<sum<<endl;
		if ( sum >= (size*ratio/100) )
			break;
	}
	cout<<"Threshold "<<i<<endl;
	return i;
}

// given an image, calculate it's texture complexity
double FigureGround::JudgeTextureComplexity(IplImage* imgRGB1)
{
//	IplImage* imgRGB1 = cvLoadImage(filename);
	IplImage* img_8U = cvCreateImage(cvGetSize(imgRGB1), IPL_DEPTH_8U, 1);
	cvCvtColor(imgRGB1,img_8U,CV_RGB2GRAY);
	IplImage*	pCannyImg = cvCreateImage(cvGetSize(img_8U), IPL_DEPTH_8U, 1);	
	cvCanny(img_8U, pCannyImg, 10, 400, 3); // 边缘检测

	cvSaveImage("canny.png",pCannyImg);

	int cnt=0;
	for (int x = 0; x < pCannyImg->height; x++)
	{
		byte* data = (byte*)(pCannyImg->imageData + pCannyImg->widthStep * x);
		for (int y = 0; y < pCannyImg->width; y++)
		{
			if (data[y])
			{
				cnt++;
			}
		}
	}
	double textureComplexity = cnt * 1.0/(imgRGB1->height * imgRGB1->width);

//	cout<<cnt<<endl;
//	cout<<imgRGB1->height * imgRGB1->width<<endl;
//	cout<<textureComplexity<<endl;

	cvReleaseImage(&pCannyImg);
//	cvReleaseImage(&imgRGB1);
	cvReleaseImage(&img_8U);
	return textureComplexity;
}

// calculate it's texture complexity in the salient region
void FigureGround::JudgeTextureComplexity_saliency(IplImage* srcImg, IplImage* salImg, double &srcComp, double &salComp)
{
	IplImage* img_8U = cvCreateImage(cvGetSize(srcImg), IPL_DEPTH_8U, 1);
	cvCvtColor(srcImg,img_8U,CV_RGB2GRAY);
	IplImage*	pCannyImg = cvCreateImage(cvGetSize(img_8U), IPL_DEPTH_8U, 1);	
	cvCanny(img_8U, pCannyImg, 200, 400, 3);

	int cnt=0, cnt2=0, cntsal=0;
	for (int x = 0; x < pCannyImg->height; x++)
	{
		byte* data = (byte*)(pCannyImg->imageData + pCannyImg->widthStep * x);
		byte* saldata=(byte*)(salImg->imageData + salImg->widthStep * x);
		for (int y = 0; y < pCannyImg->width; y++)
		{
			if (saldata[y])
				cntsal++;

			if (data[y])
			{
				cnt++;
				if (saldata[y])
					cnt2++;
			}
		}
	}
	srcComp = cnt*1.0/(srcImg->height * srcImg->width);
	if (cntsal==0) // no saliency
		salComp=1;
	else
		salComp = cnt2*1.0/cntsal;

	cvReleaseImage(&pCannyImg);
	cvReleaseImage(&img_8U);
}

double FigureGround::CalColorDif(ColorImage& srcImg, ColorImage& salImg, /*output*/double& areaDif, double& colorDif)
{
	int area = salImg.cols*salImg.rows;
	double groundR=0, groundG=0, groundB=0, figureR=0, figureG=0, figureB=0;
	int groundCnt=0, figureCnt=0;
	for (int y=0;y<salImg.rows;y++)
		for (int x=0;x<salImg.cols;x++)
		{
			int t=salImg[y][x].val[0];
			double tmpB = srcImg[y][x].val[0]*1.0/255;
			double tmpG = srcImg[y][x].val[1]*1.0/255;
			double tmpR = srcImg[y][x].val[2]*1.0/255;
			if (t==255)
			{
				figureCnt++;
				figureB += tmpB;
				figureR += tmpR;
				figureG += tmpG;
			}
			else
			{
				groundCnt++;
				groundB += tmpB;
				groundG += tmpG;
				groundR += tmpR;
			}
		}
	
	if (figureCnt != 0)
	{
		figureG/=figureCnt; figureB/=figureCnt; figureR/=figureCnt;
	}
	if (groundCnt != 0)
	{
		groundR/=groundCnt; groundB/=groundCnt; groundG/=groundCnt;
	}
	colorDif = sqrt((figureG-groundG)*(figureG-groundG)+(figureR-groundR)*(figureR-groundR)+(figureB-groundB)*(figureB-groundB));
	
	if (figureCnt*2<area)
		areaDif = figureCnt*1.0/area;
	else
		areaDif = (area*1.0-figureCnt)/area;

	if (areaDif < 0.05)
		return 0;
	double ratio=0.5;
	return (1-ratio)*colorDif + ratio*areaDif;
}

// 1--共同构成画面，2--图底分离
int FigureGround::ClassifyFigure_ground(string srcname, string salname,double times)
{
	ColorImage srcImg = cvLoadImage(srcname.c_str(), CV_LOAD_IMAGE_COLOR);
	ColorImage salImg = cvLoadImage(salname.c_str(), CV_LOAD_IMAGE_COLOR);

/*	double tmp = JudgeTextureComplexity(srcImg);
	double srcComp, salComp;
	JudgeTextureComplexity_saliency(srcImg, salImg, srcComp, salComp);

	int type;
	if ((srcComp*times) > salComp)
		type=1;
	else
		type=2;*/

	int type; 
	double colorDif, areaDif;
	double ret=CalColorDif(srcImg,salImg,areaDif, colorDif);
	if (ret>0.5)
		type = 2;  //图底分离
	else
		type=1; //图底共同构成画面

	return type;
}

void FigureGround::CalParamforOneImage(string srcname, string salname,/*output*/double& areaDif, double& colorDif, double &srcComp, double &salComp)
{
	ColorImage srcImg = cvLoadImage(srcname.c_str(), CV_LOAD_IMAGE_COLOR);
	ColorImage salImg = cvLoadImage(salname.c_str(), CV_LOAD_IMAGE_COLOR);

	IplImage* srcImg1 = cvLoadImage(srcname.c_str(), CV_LOAD_IMAGE_COLOR);
	IplImage* salImg1 = cvLoadImage(salname.c_str(), CV_LOAD_IMAGE_COLOR);
	JudgeTextureComplexity_saliency(srcImg1, salImg1, srcComp, salComp);
	CalColorDif(srcImg,salImg,areaDif, colorDif);
}

void FigureGround::CalParam(string srcpic)
{
	string rccPic = (srcpic.substr(0, srcpic.rfind('.')) + "_RCC.png");
	string srcname = srcpic;
	string salname = rccPic;
	double areaDif, colorDif, srcComp, salComp;
	CalParamforOneImage(srcname,salname,areaDif, colorDif, srcComp, salComp);

	string outFile = (srcpic.substr(0, srcpic.rfind('.')) + "_fg.txt");
	ofstream fout(outFile.c_str());
	fout << areaDif << " " << colorDif << " " << srcComp << " " << salComp << endl;
	fout.close();
}