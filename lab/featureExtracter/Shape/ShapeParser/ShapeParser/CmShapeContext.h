#pragma once
#include "def_w.h"

const	double		PI			= 3.1415926535897932384626433832795;
const	int		SHAPE_CONTEXT_SECTOR	= 12;
const	int		SHAPE_CONTEXT_REGION	= 5;		// 5 * 12 bins(as ShapeContext Definition)
const	double		SHAPE_CONTEXT_INNER_R	= 0.125;	// Regions = 0.125 * (1,2,4,8,16)
const	double		DUMMY_COST		= 0.15;		// Dummy matching cost
const	double	MAX_COST_NO_TARGET = 1000;	//当没有找到目标object时的cost


struct	ShapeContext
{
	int	h[SHAPE_CONTEXT_REGION][SHAPE_CONTEXT_SECTOR];
};

class	CmShapeContext
{
private:
	/* configuration variables */
	bool	displayFlag;
	int	nSampling;
	int	nIteration;
	double	dummyFrac;

	int	displaySize;	// 显示图像的放缩大小

	FILE*	logFile;

	bool findCdMap; 

	//// xiaohui ////
	int expandNum;

public:
	CmShapeContext();
	/*************** Configuration methods *****************/
	void	setDisplay(bool displayFigure);
	void	setSampling(int numberOfSampling);
	void	setInteration(int iterationTimes);
	void	setDummyFraction(double dummyFraction);
	void	setDisplaySize(int size);
	void	setLogFile(FILE* newLogFile);
	
	void	extractBoundary(const IplImage* img_8U, PointSet& pntSet, vector<double>& tangentSet);
	void	getSampling(PointSet& pntSet, vector<double> tangentSet);
	void	extractOuterBoundary(const IplImage* img, PointSet& pntSet);

	PointSet gridPnts;


	/*************** Service methods *****************/
	double	shapeMatch(PointSet &pntSet1, vector<double>& tangentSet1, PointSet &pntSet2, vector<double>& tangentSet2, double& matchCost, double& affCost);	
	double	shapeMatch(const IplImage* img1_8U, const IplImage* img2_8U, double& matchCost, double& affCost);
	double	shapeMatch(const char* imgName1, const char* imgName2, double& matchCost, double& affCost);
	
	void	extractContour(const IplImage* img_8U, PointSet& pntSetbfSample, PointSet& pntSet, 
		vector<int>& isSample, vector<int>& contourEndIdx, vector<double>& tangentSet);

	void  getSamplingForExtractContour(PointSet& pntSet, vector<int>& isSample);

/*private:*/

	/*************** Logging & Display *****************/
	void	addLog(const char *fmt, ...);
	void	displayImage(const IplImage* img, const char* title);
	void	displayPointSet(const PointSet& pntSet, const char* title);
	void	displayPointSetWithTangents(const PointSet& pntSet, const vector<double>& tangentSet, const char* title);
	ColorImage	GenMatchingResultImage(const PointSet& pntSet1, const PointSet& pntSet2,  const int *match);
	void DisplayMatchingResultImage(const PointSet& pntSet1, const PointSet& pntSet2,  const int *match);
	
/*private:*/
	/*************** Private Methods *****************/
	double	calcShapeContextData(PointSet& pntSet, vector<ShapeContext>& scData, bool *alive);
	double	shapeContextCost(const ShapeContext& A, const ShapeContext& B);
	double	hungaryMatch(double **cost, int *cx, int *cy, double* lx, double* ly, bool *mx, bool *my, int n);
	bool	hungaryExtendPath(int u, double **cost, int *cx, int *cy, double* lx, double* ly, bool *mx, bool *my, int n);
	double	calcAffineTransformation(PointSet& curtPntSet, const PointSet& tarPntSet, const PointSet& pA, const PointSet& pB, double beta_k);
	void	makeAffineTransformation(PointSet& pntSet, const PointSet& pA, double *cx, double *cy);
	
};


inline	double	sqrDist(const Point& A, const Point& B);
inline	double	pointDist(const Point& A, const Point& B);
