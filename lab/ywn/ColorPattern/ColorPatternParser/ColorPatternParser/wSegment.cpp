#include "StdAfx.h"
#include "wSegment.h"


wSegment::wSegment(void)
{
}


wSegment::~wSegment(void)
{
}

void wSegment::Init(const char* filename)
{
	srcImg = cvLoadImage(filename);

	const int maxSize = 800;

	cv::Mat tmpImg1;
	tmpImg1 = cv::Mat(srcImg);
	int width = tmpImg1.cols;
	int height = tmpImg1.rows;
	if (width * height > maxSize * maxSize)  // need to be shrink
	{
		printf("in wSeg, RESIZE: (%d, %d) -> ", width, height);
		double scale = height * 1.0 / maxSize;
		if (width > height)
			scale = width * 1.0 / maxSize;
		int mWidth = floor(width * 1.0 / scale);
		int mHeight = floor(height * 1.0 / scale);
		cv::resize(tmpImg1, tmpImg1, cv::Size(mWidth, mHeight));
		
		printf("(%d, %d)\n", tmpImg1.cols, tmpImg1.rows);
		*srcImg = IplImage(tmpImg1);
		cvSaveImage(filename, srcImg);
		srcImg = cvLoadImage(filename);
	}


	float sigma = 1.0f, kk = 500;
	int min_size = 300;
	center.x = srcImg->width / 2;
	center.y = srcImg->height / 2;
	imArea = srcImg->width*srcImg->height;
	
	imgSeg = segment_image(srcImg, sigma, kk, min_size, rgColors, imgInd);
	
/*	IplImage b = imgSeg;
	cvSaveImage("seg.png", &b);

	// save corresponding
	ColorImage imgCompIndex = ColorIndex2Image();
	IplImage cc = imgCompIndex;
	cvSaveImage("compIndex.png", &cc);*/
}

void wSegment::CalSegArea()
{
	meanRgbColors.resize(rgColors.size());

	vector<cv::Point3d> tmpPos;
	area.resize(rgColors.size());
	tmpPos.resize(rgColors.size());
	position.resize(rgColors.size());
	meanHsvColors.resize(rgColors.size());
	
	for (int i=0;i<imgInd.rows;i++)
		for (int j=0;j<imgInd.cols;j++)
		{
			meanRgbColors[imgInd[i][j]].m_fea[0] += CV_IMAGE_ELEM(srcImg, uchar, i, j * 3 + 2);
			meanRgbColors[imgInd[i][j]].m_fea[1] += CV_IMAGE_ELEM(srcImg, uchar, i, j * 3 + 1);
			meanRgbColors[imgInd[i][j]].m_fea[2] += CV_IMAGE_ELEM(srcImg, uchar, i, j * 3);
			area[imgInd[i][j]]++;

			// calc position
			tmpPos[imgInd[i][j]].x += j;
			tmpPos[imgInd[i][j]].y += i;
		}

	for (int i=0;i<area.size();i++)
	{
		position[i].x = cvRound(tmpPos[i].x / area[i]);
		position[i].y = cvRound(tmpPos[i].y / area[i]);

		for (int j=0;j<3;j++)
			meanRgbColors[i].m_fea[j] /= area[i];

		CVector3f tmpColorRGB;
		tmpColorRGB.m_fea[0] = meanRgbColors[i].m_fea[0] / 255.0;
		tmpColorRGB.m_fea[1] = meanRgbColors[i].m_fea[1] / 255.0;
		tmpColorRGB.m_fea[2] = meanRgbColors[i].m_fea[2] / 255.0;
		myRGB2HSV(tmpColorRGB,meanHsvColors[i]);
	}

	for (int i=0;i<area.size();i++)
		segArea.push_back(make_pair(area[i],i));

	sort(segArea.begin(), segArea.end());
}

// 按照面积比例显示前n个面积最大的颜色
void wSegment::ShowTopnColors(const char* savename)
{
	int n = rgColors.size();
	int width1=40;
	int height=400;
	IplImage* topn_img = cvCreateImage( cvSize(width1*n,height), 8, 3 );
	cvZero(topn_img);

	for (int i=1;i<=n;i++)
	{
		CvScalar color;
		int tmpInd = segArea[segArea.size()-i].second;
		color.val[0] = cvRound(meanRgbColors[tmpInd].m_fea[2]);
		color.val[1] = cvRound(meanRgbColors[tmpInd].m_fea[1]);
		color.val[2] = cvRound(meanRgbColors[tmpInd].m_fea[0]);

		cvRectangle( topn_img, cvPoint(i*width1,height),cvPoint((i+1)*width1,0), color, -1);

		// output some info
		cout<<i<<endl;
		cout<<"area: "<<segArea[segArea.size()-i].first<<endl;
		cout<<"position: "<<position[tmpInd].x<<" "<<position[tmpInd].y<<endl;
		cout<<"HSV: "<<meanHsvColors[tmpInd].m_fea[0]<<" "<<meanHsvColors[tmpInd].m_fea[1]<<" "<<meanHsvColors[tmpInd].m_fea[2]<<endl;
	}
	
	cvSaveImage(savename, topn_img);
	cvReleaseImage(&topn_img);
}

vector<CVector3f> wSegment::ColorThemeExtraction()
{
	CalSegArea();
	for (int i=1;i<6;i++)
		rgbColorTheme.push_back(meanRgbColors[segArea[segArea.size()-i].second]);

	return rgbColorTheme;
}

vector<CVector3f> wSegment::TopColorExtraction()
{
	CalSegArea();
	for (int i=1;i<=segArea.size();i++)
		rgbColorTheme.push_back(meanRgbColors[segArea[segArea.size()-i].second]);

	return rgbColorTheme;
}

// draw compIndex image
ColorImage wSegment::ColorIndex2Image()
{
	int hwidth = 40, height = 30;
	int cnum = int(rgColors.size());
	ColorImage img = ColorImage::zeros(height*cnum, hwidth*2);
	for(int i=0;i<cnum;i++)
	{
		int ystart=height*i;
		cxFillRect(img,cv::Rect(hwidth,ystart,hwidth,height),rgColors[i]);	
//		cv::putText(img,num_to_string(i),cv::Point(hwidth/2-10, ystart+height/2+5),cv::FONT_HERSHEY_SIMPLEX,0.7,cvScalar(200,200,200));
	}
	return img;
}

void wSegment::cxFillRect(ColorImage& img, cv::Rect rect, cv::Vec3b color)
{
	vector<cv::Point> pp;
	pp.push_back(cv::Point(rect.x,rect.y));
	pp.push_back(cv::Point(rect.x+rect.width-1,rect.y));
	pp.push_back(cv::Point(rect.x+rect.width-1,rect.y+rect.height-1));
	pp.push_back(cv::Point(rect.x,rect.y+rect.height-1));
	FillPoly(img,pp,color);
}

void wSegment::FillPoly(ColorImage& img, const vector<cv::Point>& points, cv::Vec3b color)
{
	const cv::Point* ppoints = &points[0];
	int npoints = (int)points.size();
	cv::fillPoly(img, &ppoints, &npoints, 1, cvScalar(color[0],color[1],color[2]));
}

string wSegment::num_to_string(int num)
{
	char buf[32];
	return _itoa(num,buf,10);
}

float wSegment::CalRgbDistance(CVector3f rgb1, CVector3f rgb2)
{
	float ret=0;
	for (int i=0;i<3;i++)
		ret += (rgb1.m_fea[i]-rgb2.m_fea[i])*(rgb1.m_fea[i]-rgb2.m_fea[i]);
	return sqrt(ret);
}

float wSegment::CalHsvDistance(CVector3f hsv1, CVector3f hsv2)
{
	hsv1.m_fea[0] /= 360.0;
	hsv2.m_fea[0] /= 360.0;
	float ret=0;
	for (int i=0;i<3;i++)
		ret += (hsv1.m_fea[i]-hsv2.m_fea[i])*(hsv1.m_fea[i]-hsv2.m_fea[i]);
	return sqrt(ret);
}

void wSegment::CalSegPos()
{
	posDistance.resize(position.size());
	segPos.resize(position.size());

	double centerDis = sqrt((center.x*center.x+center.y*center.y)*1.0);
	for (int i=0;i<position.size();i++)
	{
		posDistance[i] = sqrt(((position[i].x-center.x)*(position[i].x-center.x)+(position[i].y-center.y)*(position[i].y-center.y))*1.0);
		posDistance[i] = 1 - posDistance[i] / centerDis;
		segPos[i] = make_pair(posDistance[i],i);
	}

	sort(segPos.begin(), segPos.end());
}

void wSegment::CalSegContrast()
{
	colorContrast.resize(position.size());
	segContrast.resize(position.size());

	for (int i=0;i<meanRgbColors.size();i++)
	{
		double tmp=0;
		for (int j=0;j<meanRgbColors.size();j++)
		{
			tmp += CalRgbDistance(meanRgbColors[i],meanRgbColors[j])*area[i]/imArea;
		}
		tmp /= (rgColors.size()-1);

		colorContrast[i] = tmp;
		segContrast[i] = make_pair(colorContrast[i],i);
	}

	sort(segContrast.begin(), segContrast.end());
}

void wSegment::CalAllTerms()
{
	CalSegArea();
	CombineRgbColor(); // combine colors
	CalSegPos();
	CalSegContrast();

	secTerm.resize(meanRgbColors.size());
	allterm.resize(meanRgbColors.size());
	areaProb.resize(meanRgbColors.size());
	double alpha1=0.5;
	for (int i=0;i<meanRgbColors.size();i++)
	{
		secTerm[i] = make_pair(posDistance[i]*colorContrast[i]*10,i);
		areaProb[i] = area[i]*1.0/imArea;
		double tmp = alpha1 * areaProb[i] + (1-alpha1)*secTerm[i].first;
		allterm[i] = make_pair(tmp,i);
	}
	sort(allterm.begin(),allterm.end());
	sort(secTerm.begin(),secTerm.end());
}

void wSegment::CombineRgbColor()
{
	double threshold = 10; // 合并的threshold
	combinedRgbColors.resize(meanRgbColors.size());
	for (int i=0;i<meanRgbColors.size();i++)
		combinedRgbColors[i] = meanRgbColors[i];

	vector<pair<double, pair<int,int>>> pairDis; 
	bool isFind = true;
	while (isFind)
	{
		pairDis.clear();
		for (int i=0;i<combinedRgbColors.size();i++)
		{
			for (int j=i+1;j<combinedRgbColors.size();j++)
			{
				double tmp = CalRgbDistance(combinedRgbColors[i],combinedRgbColors[j]);
				pairDis.push_back(make_pair(tmp,make_pair(i,j)));
			}
		}
		
		sort(pairDis.begin(),pairDis.end());

		// find less than threshold
		if (pairDis[0].first < threshold)
		{
			isFind=true;
			//update: area, position
			combinedRgbColors[pairDis[0].second.first].m_fea[0] = (combinedRgbColors[pairDis[0].second.first].m_fea[0] + combinedRgbColors[pairDis[0].second.second].m_fea[0])/2.0;
			combinedRgbColors[pairDis[0].second.first].m_fea[1] = (combinedRgbColors[pairDis[0].second.first].m_fea[1] + combinedRgbColors[pairDis[0].second.second].m_fea[1])/2.0;
			combinedRgbColors[pairDis[0].second.first].m_fea[2]  = (combinedRgbColors[pairDis[0].second.first].m_fea[2] + combinedRgbColors[pairDis[0].second.second].m_fea[2])/2.0;
			vector <CVector3f> ::iterator iter = combinedRgbColors.begin()+pairDis[0].second.second;
			combinedRgbColors.erase(iter);

			area[pairDis[0].second.first] += area[pairDis[0].second.second];
			vector <int> ::iterator iter2 = area.begin() + pairDis[0].second.second;
			area.erase(iter2);

			position[pairDis[0].second.first].x = cvRound((position[pairDis[0].second.first].x + position[pairDis[0].second.second].x)*1.0/2);
			position[pairDis[0].second.first].y = cvRound((position[pairDis[0].second.first].y + position[pairDis[0].second.second].y)*1.0/2);
			vector <cv::Point2d> ::iterator iter3 = position.begin() + pairDis[0].second.second;
			position.erase(iter3);
		}
		else
			isFind=false;
	}// while

	meanRgbColors.resize(combinedRgbColors.size());
	for (int i=0;i<combinedRgbColors.size();i++)
		meanRgbColors[i] = combinedRgbColors[i];

	//重新计算
	meanHsvColors.resize(meanRgbColors.size());
	for (int i=0;i<meanRgbColors.size();i++)
	{
		CVector3f tmpColorRGB;
		tmpColorRGB.m_fea[0] = meanRgbColors[i].m_fea[0] / 255.0;
		tmpColorRGB.m_fea[1] = meanRgbColors[i].m_fea[1] / 255.0;
		tmpColorRGB.m_fea[2] = meanRgbColors[i].m_fea[2] / 255.0;
		myRGB2HSV(tmpColorRGB,meanHsvColors[i]);
	}
}

void wSegment::ArrangeColorTheme()
{
	hsvColorTheme.resize(rgbColorTheme.size());
	for (int i=0;i<5;i++)
	{
		CVector3f tmpColorRGB;
		tmpColorRGB.m_fea[0] = rgbColorTheme[i].m_fea[0] / 255.0;
		tmpColorRGB.m_fea[1] = rgbColorTheme[i].m_fea[1] / 255.0;
		tmpColorRGB.m_fea[2] = rgbColorTheme[i].m_fea[2] / 255.0;
		myRGB2HSV(tmpColorRGB,hsvColorTheme[i]);
	}

	double alpha1=0.7, alpha2=0.3;
	vector<CVector3f> tmpHsv;
	vector<pair<float,int>> permDis;

	vector<vector<int>> perm;
	Permutation(perm);
	
	for (int pk=0;pk<perm.size();pk++)
	{
		tmpHsv.clear();
		for (int i=0;i<5;i++)
			tmpHsv.push_back(hsvColorTheme[perm[pk][i]]);

		float dis2 = CalHueWheelOrderDis(tmpHsv);
		float dis1 = CalChangeDistance(tmpHsv);

		permDis.push_back(make_pair(alpha1*dis1+alpha2*dis2, pk));
	//	cout<<pk<<": "<<dis1<<" "<<dis2<<" "<<alpha1*dis1+alpha2*dis2<<endl;
	}
	sort(permDis.begin(), permDis.end());

	tmpHsv.clear();
	for (int i=0;i<5;i++)
		tmpHsv.push_back(rgbColorTheme[perm[permDis[0].second][i]]);

	for (int i=0;i<5;i++)
		rgbColorTheme[i]=tmpHsv[i];
}

void wSegment::Permutation(vector<vector<int>> &perm)
{
	int a[] = {0,1,2,3,4};
	perm.resize(120);
	int ind=0;
	do
	{
		perm[ind].resize(5);
		for (int i=0;i<5;i++)
			perm[ind][i]=a[i];
		ind++;
	}
	while (next_permutation(a,a+5));
}

float wSegment::CalChangeDistance(vector<CVector3f>& hsvColor)
{
	float hd=0;
	for (int j=0;j<hsvColor.size()-1;j++)
//		hd += abs(hsvColor[j].m_fea[2]-hsvColor[j+1].m_fea[2]);
		hd += CalHsvDistance(hsvColor[j],hsvColor[j+1]);
	hd /= (hsvColor.size()-1);
	return hd;
}

// 
float wSegment::CalHueWheelOrderDis(vector<CVector3f>& hsvColor)
{
	int cntClockwise=0, cntAntiClockwise=0;
	//假设是顺时针
	for (int i=1;i<hsvColor.size();i++)
		if (hsvColor[i].m_fea[0] < hsvColor[0].m_fea[0])
			hsvColor[i].m_fea[0]+=360;
	for (int i=0;i<hsvColor.size()-1;i++)
	{
		if (abs(hsvColor[i].m_fea[0]-hsvColor[i+1].m_fea[0]) < 30)
			continue;
		else if (hsvColor[i].m_fea[0]<hsvColor[i+1].m_fea[0])
			continue;
		else
			cntClockwise++;
	}

	// 假设是逆时针
	for (int i=1;i<hsvColor.size();i++)
		if (hsvColor[i].m_fea[0] > hsvColor[0].m_fea[0])
			hsvColor[i].m_fea[0] -= 360;
	for (int i=0;i<hsvColor.size()-1;i++)
	{
		if (abs(hsvColor[i].m_fea[0]-hsvColor[i+1].m_fea[0]) < 30)
			continue;
		else if (hsvColor[i].m_fea[0]>hsvColor[i+1].m_fea[0])
			continue;
		else
			cntAntiClockwise++;
	}

	if (cntClockwise < cntAntiClockwise)
		return cntClockwise*1.0/5.0;
	else
		return cntAntiClockwise*1.0/5.0;
}

vector<CVector3f> wSegment::ColorThemeExtraction_our()
{
	// debug得到中间结果
	//分割后的结果
/*	CalSegArea();
	for (int i=1;i<=meanRgbColors.size();i++)
		rgbColorTheme.push_back(meanRgbColors[i-1]); 

	// combine的结果
	CalSegArea();
	CombineRgbColor();
	for (int i=1;i<=meanRgbColors.size();i++)
		rgbColorTheme.push_back(meanRgbColors[i-1]); 

	// area排序的结果
	CalSegArea();
	CombineRgbColor();
	allterm.resize(meanRgbColors.size());
	for (int i=0;i<meanRgbColors.size();i++)
	{
		allterm[i] = make_pair(area[i],i);
	}
	sort(allterm.begin(),allterm.end());
	for (int i=1;i<=meanRgbColors.size();i++)
		rgbColorTheme.push_back(meanRgbColors[allterm[allterm.size()-i].second]);

	// second term排序的结果
	CalAllTerms();
	for (int i=1;i<=allterm.size();i++)
		rgbColorTheme.push_back(meanRgbColors[secTerm[secTerm.size()-i].second]);*/


	// 最终结果
	CalAllTerms();
	if (meanRgbColors.size()>=5)
	{
		for (int i=1;i<=5;i++)
			rgbColorTheme.push_back(meanRgbColors[allterm[allterm.size()-i].second]);
	}
	else
	{
		for (int i=1;i<=meanRgbColors.size();i++)
			rgbColorTheme.push_back(meanRgbColors[allterm[allterm.size()-i].second]);
		for (int i=meanRgbColors.size()+1;i<=5;i++)
			rgbColorTheme.push_back(rgbColorTheme[0]);
	}
	ArrangeColorTheme();

	return rgbColorTheme;
}

vector<CVector3f> wSegment::ColorThemeExtraction_ourWithoutArrangement()
{
	CalAllTerms();
	for (int i=1;i<=/*allterm.size()*/5;i++)
		rgbColorTheme.push_back(meanRgbColors[allterm[allterm.size()-i].second]);

	return rgbColorTheme;
}

vector<CVector3f> wSegment::AllColors()
{
	vector<CVector3f> allcolors;
	for (int i=1;i<=allterm.size();i++)
		allcolors.push_back(meanRgbColors[allterm[allterm.size()-i].second]);

	return allcolors;
}
