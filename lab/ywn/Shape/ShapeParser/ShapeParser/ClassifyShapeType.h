#pragma once
#include "CmShapeContext.h"
#include <fstream>
#include <string>


class ShapeParser
{
public:
	void ClassifyShape(std::string salfilename, int& smallclass, int& bigclass);
	// ���غ�ÿ��baseƥ���matchCost
	void ClassifyShape(std::string salfilename, vector<double>& matchCost, int& smallclass, int& bigclass);
	void CalParam(std::string imgName, std::string imgPath);
};