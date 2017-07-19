#include "CmShapeContext.h"
#include "ClassifyShapeType.h"
#include <iostream>

// need *_RCC.png in the same folder
// e.g. 1.jpg D:\SummerPractice\testDataSet\ani\bird\

int main(int argc, char* argv[])
{
	string imgName = argv[1];
	string imgPath = argv[2];
	ShapeParser sp;
	sp.CalParam(imgName, imgPath);
	return 0;
}

