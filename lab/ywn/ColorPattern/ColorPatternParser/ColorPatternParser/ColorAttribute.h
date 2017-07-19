#pragma once
#include "wglobalAdd.h"

// set threshold
#define HUE_PATTERN_DIFF 0.5 // hue的差异大于该值，is hue pattern

class ColorAttribute
{
public:
	string filename;
	double imgSize;
	vector<CVector3f> allColors; // 合并后的所有颜色
	
	// normalized to [0,1]
	double bright;				// average value of all pixels
	double brightContrast;
	double sat;	 // 饱和度
	double satContrast;
	
	// 配色方案 color pattern程度
	double coolColorRatio;
	double clearColorRatio;
	double huePat;  // true--hue, false--tone
	double unifiedPat;  // true--unified, false--sharp
	double clearPat; //true--clear, false--dull

	// 中间变量
	double hue;
	double hueContrast;
	double nCoolColor; // hue在60-230之间的是冷色，统计个数
	double nClearColor; //bright>0.7是clear color

	// hue-tone histogram(暂时没用上)
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

	// hue-tone histogram(暂时没用上)
	void CalHueToneHist();

	void getColorPattern(string srcpic, string desfile);
};

