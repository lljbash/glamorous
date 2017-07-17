#pragma once
#include "ColorAttribute.h"
#define SAT_CONTRAST_THRESH 0.4
#define BRI_CONTRAST_THRESH 0.4

/*
for classify color
纯度：1-高，2-中，3-低
纯度长短调：1-纯度-长，2-纯度-短-都低，3-纯度-短-都高
影调（黑白灰）：1-高调，2-低调，3-中间调
黑白灰的长短调：1-长，2-短
冷暖调：1-冷，2-中，3-暖
清浊色：1-清色为主，2-浊色为主，3-清浊均衡
*/

void ClassifyColor(string srcfile, vector<int>& colorAttr); // colorAttr的顺序同上
void BrightnessValueandType(string srcfile, double& value, int& type);
void TagColor(string srcfile); // 对新图像标注color attribute

// process the training data maually labeled
void CopyImageForColorAttr();