#pragma once
#include "ColorAttribute.h"
#define SAT_CONTRAST_THRESH 0.4
#define BRI_CONTRAST_THRESH 0.4

/*
for classify color
���ȣ�1-�ߣ�2-�У�3-��
���ȳ��̵���1-����-����2-����-��-���ͣ�3-����-��-����
Ӱ�����ڰ׻ң���1-�ߵ���2-�͵���3-�м��
�ڰ׻ҵĳ��̵���1-����2-��
��ů����1-�䣬2-�У�3-ů
����ɫ��1-��ɫΪ����2-��ɫΪ����3-���Ǿ���
*/

void ClassifyColor(string srcfile, vector<int>& colorAttr); // colorAttr��˳��ͬ��
void BrightnessValueandType(string srcfile, double& value, int& type);
void TagColor(string srcfile); // ����ͼ���עcolor attribute

// process the training data maually labeled
void CopyImageForColorAttr();