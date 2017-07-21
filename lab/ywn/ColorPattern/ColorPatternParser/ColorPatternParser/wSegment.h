/*
graph basedͼ��ָ�õ�ÿһ���ƽ����ɫ�������λ�ã�
����threshold=10������0-255��RGBֵ���ϲ����Ƶ���ɫ�����¼�����ɫ�������λ�ã���
����paper�е��������w=0.5.
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
	
	vector<pair<int, int>> segArea; // pair<area, segindex> ��С��������
	vector<CVector3f> meanRgbColors; // ����R, G, B˳��, imgInd��˳��
	vector<cv::Point2d> position;   // ÿ��segm�����е�λ��ƽ��������segindex�洢��
	vector<CVector3f> meanHsvColors; // meanRgbColorsת��HSV
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
	vector<pair<double, int>> allterm; // pair< paper���������, segindex>
	vector<CVector3f> combinedRgbColors;

public:
	wSegment(void);
	~wSegment(void);
	void Init(const char* filename);
	void CalSegArea();
	void ShowTopnColors(const char* savename);
	vector<CVector3f> ColorThemeExtraction(); // �������ǰ���
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
	void CombineRgbColor(); // ������ɫ�����Ƴ̶Ⱥϲ�meanRgbColors

	// for color theme arrangement
	float CalHueWheelOrderDis(vector<CVector3f>& hsvColor);
	float CalChangeDistance(vector<CVector3f>& hsvColor);
	void Permutation(vector<vector<int>> &perm);
};

