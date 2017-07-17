//#include "stdafx.h"
#include <fstream>
#include "ClassifyColor.h"
using namespace std;

void ClassifyColor(string srcfile, vector<int>& colorAttr)
{
	ColorAttribute ca(srcfile);
	ca.CalBriSatContrast();
	
	colorAttr.resize(6);
	// 纯度1-高，2-中，3-低
	if (ca.sat > 0.67)
		colorAttr[0]=1;
	else if (ca.sat > 0.33)
		colorAttr[0]=2;
	else
		colorAttr[0]=3;

	// 纯度长短调：1-纯度-长，2-纯度-短-都低，3-纯度-短-都高
	if (ca.satContrast > SAT_CONTRAST_THRESH)
		colorAttr[1]=1;
	else
	{
		if (ca.sat > 0.5)
			colorAttr[1]=3;
		else
			colorAttr[1]=2;
	}

	//影调（黑白灰）：1-高调，2-低调，3-中间调
	if (ca.bright > 0.67)
		colorAttr[2]=1;
	else if (ca.bright > 0.33)
		colorAttr[2]=3;
	else
		colorAttr[2]=2;

	//黑白灰的长短调：1-长，2-短
	if (ca.brightContrast > BRI_CONTRAST_THRESH)
		colorAttr[3]=1;
	else
		colorAttr[3]=2;

	//冷暖调：1-冷，2-中，3-暖
	if (ca.coolColorRatio > 0.6)
		colorAttr[4]=1;
	else if (ca.coolColorRatio < 0.33)
		colorAttr[4]=3;
	else
		colorAttr[4]=2;

	//清浊色：1-清色为主，2-浊色为主，3-清浊均衡
	if (ca.clearColorRatio > 0.6)
		colorAttr[5]=1;
	else if (ca.clearColorRatio < 0.33)
		colorAttr[5]=2;
	else
		colorAttr[5]=3;
}

void BrightnessValueandType(string srcfile, double& value, int& type)
{
	ColorAttribute ca(srcfile);
	ca.CalBriSatContrast();
	value = ca.bright;

	//影调（黑白灰）：1-高调，2-低调
	if (ca.bright > 0.5)
		type=1;
	else 
		type=2;
}

