/*
graph based图像分割，得到每一块的平均颜色，面积和位置；
设置threshold=10（度量0-255的RGB值）合并相似的颜色（重新计算颜色，面积和位置）；
计算paper中的两项，排序，w=0.5.
*/

#pragma once
#include "segment\segment-image.h"
#include "wglobalAdd.h"

class wSegment
{
public:
	IplImage* srcImg;
	ColorImage imgSeg; 
	cv::Mat_<int> imgInd; // index from 0
	vector<cv::Vec3b> rgColors;
	cv::Point2d center;
	int imArea;
	
	vector<pair<int, int>> segArea; // pair<area, segindex> 从小到大排列
	vector<CVector3f> meanRgbColors; // 按照R, G, B顺序, imgInd的顺序
	vector<cv::Point2d> position;   // 每个segm的所有点位置平均，根据segindex存储的
	vector<CVector3f> meanHsvColors; // meanRgbColors转成HSV
	vector<CVector3f> rgbColorTheme; // 5-color theme
	vector<CVector3f> hsvColorTheme; // 5-color theme

	// for our method to extract themes
	vector<int> area;   // pixel number
	vector<double> areaProb;   // pixel number
	vector<double> posDistance; //P(ti) in paper
	vector<double> colorContrast; //S(ti) in paper
	vector<pair<double, int>> segPos; // pair<P(ti) in paper, segindex>
	vector<pair<double, int>> segContrast; // pair<S(ti) in paper, segindex>
	vector<pair<double, int>> secTerm; //pair< second term in paper, segindex>
	vector<pair<double, int>> allterm; // pair< paper中两项相加, segindex>
	vector<CVector3f> combinedRgbColors;

public:
	wSegment(void);
	~wSegment(void);
	void Init(const char* filename);
	void CalSegArea();
	void ShowTopnColors(const char* savename);
	vector<CVector3f> ColorThemeExtraction(); // 面积最大的前五个
	vector<CVector3f> TopColorExtraction();

	vector<CVector3f> ColorThemeExtraction_our();
	vector<CVector3f> ColorThemeExtraction_ourWithoutArrangement();
	vector<CVector3f> AllColors();
	void ArrangeColorTheme();
	
private:
	ColorImage ColorIndex2Image();
	void cxFillRect(ColorImage& img, cv::Rect rect, cv::Vec3b color);
	void FillPoly(ColorImage& img, const vector<cv::Point>& points, cv::Vec3b color);
	string num_to_string(int num);

	float CalRgbDistance(CVector3f rgb1, CVector3f rgb2);
	float CalHsvDistance(CVector3f hsv1, CVector3f hsv2);
	void CalSegPos();
	void CalSegContrast();
	void CalAllTerms();
	void CombineRgbColor(); // 按照颜色的相似程度合并meanRgbColors

	// for color theme arrangement
	float CalHueWheelOrderDis(vector<CVector3f>& hsvColor);
	float CalChangeDistance(vector<CVector3f>& hsvColor);
	void Permutation(vector<vector<int>> &perm);
};

