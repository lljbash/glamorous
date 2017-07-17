#include "StdAfx.h"
#include "ColorAttribute.h"
#include "wSegment.h"

ColorAttribute::ColorAttribute(void)
{
}


ColorAttribute::~ColorAttribute(void)
{
}

ColorAttribute::ColorAttribute(string filename)
{
	this->filename = filename;
	nCoolColor=0;
	nClearColor=0;
}

void ColorAttribute::SaveRGBColorTheme(vector<CVector3f> rgbColors, string savename)
{
	int n = rgbColors.size();
	int width1;
	if (n==5 || n==3)
		width1=200;
	else
		width1=40;

	int height=400;
	IplImage* topn_img = cvCreateImage( cvSize(width1*n,height), 8, 3 );
	cvZero(topn_img);

	for (int i=0;i<n;i++)
	{
		CvScalar color;
		color.val[0] = floor(rgbColors[i].m_fea[2]);
		color.val[1] = floor(rgbColors[i].m_fea[1]);
		color.val[2] = floor(rgbColors[i].m_fea[0]);

		cvRectangle( topn_img, cvPoint(i*width1,height),cvPoint((i+1)*width1,0), color, -1);
	}
	
	cvSaveImage(savename.c_str(), topn_img);
	cvReleaseImage(&topn_img);
}

void ColorAttribute::ShowAllColorsAndColorTheme()
{
	// extract all colors
	wSegment wseg2;
	wseg2.Init(filename.c_str());
	allColors = wseg2.ColorThemeExtraction_our();

	// save all color themes
	string tmp, savename, savename_3c, savename_all;
	tmp = filename.substr(0,filename.find('.'));
	savename = tmp + "_5c.png";
	savename_3c = tmp + "_3c.png";
	savename_all = tmp + "_all.png";

/*	SaveRGBColorTheme(allColors, savename_all.c_str());

	vector<CVector3f> rgbColorTheme_3c;
	for (int i=0;i<3;i++)
		rgbColorTheme_3c.push_back(allColors[i]);

	SaveRGBColorTheme(rgbColorTheme_3c, savename_3c.c_str());
	*/
	vector<CVector3f> rgbColorTheme_5c;
	for (int i=0;i<5;i++)
		rgbColorTheme_5c.push_back(allColors[i]);

	SaveRGBColorTheme(rgbColorTheme_5c, savename.c_str());
}

// 亮度，亮度对比度，纯度，纯度对比度
void ColorAttribute::CalBriSatContrast()
{
	IplImage* srcImg = cvLoadImage(filename.c_str(),CV_LOAD_IMAGE_COLOR);
	imgSize = srcImg->width * srcImg->height;

	IplImage* dest1;
	dest1 = cvCreateImage(cvGetSize(srcImg),IPL_DEPTH_8U,3);
	cvCvtColor(srcImg,dest1,CV_RGB2HSV);

	// mean value
	double briSum=0, satSum=0, hueSum=0;
	nhue.resize(10);
	for (int y=0;y<dest1->height;y++)
		for (int x=0;x<dest1->width;x++)
		{
			double tmpHue = (double)CV_IMAGE_ELEM(dest1, uchar, y, x * 3);
			hueSum += tmpHue/180;
			if ((tmpHue>30) && (tmpHue<110))
				nCoolColor++;

			satSum += (double)CV_IMAGE_ELEM(dest1, uchar, y, x * 3 + 1) /255;
			double tmpBri = (double)CV_IMAGE_ELEM(dest1, uchar, y, x * 3 + 2) /255;
			briSum += tmpBri;
			if (tmpBri > 0.7)
				nClearColor++;
		}
	
	bright = briSum/imgSize;
	sat = satSum/imgSize;
	hue = hueSum/imgSize;

	// absolute value
	double briConSum=0, satConSum=0, hueConSum=0;
	for (int y=0;y<dest1->height;y++)
		for (int x=0;x<dest1->width;x++)
		{
			hueConSum += abs((double)CV_IMAGE_ELEM(dest1, uchar, y, x * 3)/180 - hue);
			satConSum += abs((double)CV_IMAGE_ELEM(dest1, uchar, y, x * 3 + 1)/255 - sat);
			briConSum += abs((double)CV_IMAGE_ELEM(dest1, uchar, y, x * 3 + 2)/255 - bright);
		}

	hueContrast = hueConSum/imgSize;
	brightContrast = briConSum/imgSize;
	satContrast = satConSum/imgSize;

	coolColorRatio = nCoolColor/imgSize;
	clearColorRatio = nClearColor/imgSize;

	cvReleaseImage(&srcImg);
	cvReleaseImage(&dest1);
}

void ColorAttribute::CalColorPattern()
{
	// warm or cool: warm:GY, G, BG, B, PB
	double sum=0;

	// color pattern
	// hue or tone: hue diff + sat
	huePat = hueContrast + sat;

	// unified or sharp: unified: hue diff + sat diff + bright diff
	unifiedPat = hueContrast + satContrast + brightContrast;

	// clear or dull: clear:V B P Vp
	clearPat = nClearColor/imgSize;
}

void ColorAttribute::CalHueToneHist()
{
	IplImage* srcImg = cvLoadImage(filename.c_str(),CV_LOAD_IMAGE_COLOR);
	imgSize = srcImg->width * srcImg->height;

	IplImage* dest1;
	dest1 = cvCreateImage(cvGetSize(srcImg),IPL_DEPTH_8U,3);
	cvCvtColor(srcImg,dest1,CV_RGB2HSV);

	// load threshold
	minHue.resize(10); maxHue.resize(10);
	minSat.resize(12); maxSat.resize(12); 
	minBri.resize(12); maxBri.resize(12);

	ifstream fin("config.txt");
	for (int i=0;i<minHue.size();i++)
		fin>>minHue[i]>>maxHue[i];
	for (int i=0;i<minSat.size();i++)
		fin>>minSat[i]>>maxSat[i];
	for (int i=0;i<minBri.size();i++)
		fin>>minBri[i]>>maxBri[i];


	// mean value
	nhue.resize(10);
	for (int y=0;y<dest1->height;y++)
		for (int x=0;x<dest1->width;x++)
		{
			double tmpSat = (double)CV_IMAGE_ELEM(dest1, uchar, y, x * 3 + 1) /255;
			double tmpBri = (double)CV_IMAGE_ELEM(dest1, uchar, y, x * 3 + 2) /255;
			int tmp = (int)CV_IMAGE_ELEM(dest1, uchar, y, x * 3);

			// calculate the hue histogram
			int i;
			for (i=1;i<minHue.size();i++)
				if ((tmp >= minHue[i]) && (tmp <= maxHue[i]))
				{
					nhue[i]++;
					break;
				}

			if (i = minHue.size())
				nhue[0]++;

			// calculate the tone histogram
			for (int i=0;i<minSat.size();i++)
				if ((tmpSat>=minSat[i]) && (tmpSat< maxSat[i]) && (tmpBri>=minBri[i]) && (tmpBri<maxBri[i]))
				{
					ntone[i]++;
					break;
				}
		}

	cvReleaseImage(&srcImg);
	cvReleaseImage(&dest1);
}

void ColorAttribute::getColorPattern(string srcpic, string desfile)
{
	fstream fout(desfile, ios::app);

	CalBriSatContrast();
	fout << srcpic << ' ';
	fout<<sat<<" "<<satContrast<<" "<<bright<<" "<<brightContrast<<" "<<coolColorRatio<<" "<<clearColorRatio << " ";
	fout.close();

	fstream fout2(desfile,ios::app);
	ShowAllColorsAndColorTheme();
	for (int i=0;i<5;i++)
		for (int j=0;j<3;j++)
			fout2<<(int)(allColors[i].m_fea[j])<<" ";
	fout2 << endl;
	fout2.close();
}