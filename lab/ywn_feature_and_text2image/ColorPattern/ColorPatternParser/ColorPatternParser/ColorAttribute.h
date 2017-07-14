#pragma once
#include "wglobalAdd.h"

// set threshold
#define HUE_PATTERN_DIFF 0.5 // hue�Ĳ�����ڸ�ֵ��is hue pattern

class ColorAttribute
{
public:
	string filename;
	double imgSize;
	vector<CVector3f> allColors; // �ϲ����������ɫ
	
	// normalized to [0,1]
	double bright;				// average value of all pixels
	double brightContrast;
	double sat;	 // ���Ͷ�
	double satContrast;
	
	// ��ɫ���� color pattern�̶�
	double coolColorRatio;
	double clearColorRatio;
	double huePat;  // true--hue, false--tone
	double unifiedPat;  // true--unified, false--sharp
	double clearPat; //true--clear, false--dull

	// �м����
	double hue;
	double hueContrast;
	double nCoolColor; // hue��60-230֮�������ɫ��ͳ�Ƹ���
	double nClearColor; //bright>0.7��clear color

	// hue-tone histogram(��ʱû����)
	vector<int> minHue, maxHue;
	vector<double> minSat, maxSat, minBri, maxBri;
	vector<double> nhue; // order by R, YR, Y, GY, G, BG, B, PB, P, RP
	vector<double> ntone; // order by Vp,Lgr,Gr,Dgr, P,L,Dl,Dk, B,S,Dp, V

public:
	ColorAttribute(void);
	ColorAttribute(string filename);
	~ColorAttribute(void);

	void ShowAllColorsAndColorTheme();
	void SaveRGBColorTheme(vector<CVector3f> rgbColors, string savename);

	void CalBriSatContrast();
	void CalColorPattern();

	// hue-tone histogram(��ʱû����)
	void CalHueToneHist();

	void getColorPattern(string srcpic, string desfile);
};

