//#include "stdafx.h"
#include <fstream>
#include "ClassifyColor.h"
using namespace std;

void ClassifyColor(string srcfile, vector<int>& colorAttr)
{
	ColorAttribute ca(srcfile);
	ca.CalBriSatContrast();
	
	colorAttr.resize(6);
	// ����1-�ߣ�2-�У�3-��
	if (ca.sat > 0.67)
		colorAttr[0]=1;
	else if (ca.sat > 0.33)
		colorAttr[0]=2;
	else
		colorAttr[0]=3;

	// ���ȳ��̵���1-����-����2-����-��-���ͣ�3-����-��-����
	if (ca.satContrast > SAT_CONTRAST_THRESH)
		colorAttr[1]=1;
	else
	{
		if (ca.sat > 0.5)
			colorAttr[1]=3;
		else
			colorAttr[1]=2;
	}

	//Ӱ�����ڰ׻ң���1-�ߵ���2-�͵���3-�м��
	if (ca.bright > 0.67)
		colorAttr[2]=1;
	else if (ca.bright > 0.33)
		colorAttr[2]=3;
	else
		colorAttr[2]=2;

	//�ڰ׻ҵĳ��̵���1-����2-��
	if (ca.brightContrast > BRI_CONTRAST_THRESH)
		colorAttr[3]=1;
	else
		colorAttr[3]=2;

	//��ů����1-�䣬2-�У�3-ů
	if (ca.coolColorRatio > 0.6)
		colorAttr[4]=1;
	else if (ca.coolColorRatio < 0.33)
		colorAttr[4]=3;
	else
		colorAttr[4]=2;

	//����ɫ��1-��ɫΪ����2-��ɫΪ����3-���Ǿ���
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

	//Ӱ�����ڰ׻ң���1-�ߵ���2-�͵�
	if (ca.bright > 0.5)
		type=1;
	else 
		type=2;
}

