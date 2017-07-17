//#include "stdafx.h"
#include "CmShapeContext.h"
#include <stdarg.h>

CmShapeContext::CmShapeContext()
{
	/*************** Constants & Variables *****************/
	displayFlag	= true;	// whether display figures
	nSampling		= 500;		// number of sampling
	nIteration		= 1;		// number of iterations
	dummyFrac		= 0.25;		// fraction of dummy points

	displaySize	= 1000;

	logFile		= NULL;	// logFile out 

	findCdMap = false;

	expandNum = 10;
}

inline	double	sqrDist(const Point& A, const Point& B)
{
	return  sqr(A.x - B.x) + sqr(A.y - B.y);
}

inline	double	pointDist(const Point& A, const Point& B)
{
	return	sqrt( sqrDist(A, B) );
}

/*************** Configuration methods *****************/
void	CmShapeContext::setDisplay(bool displayFigure)
{
	displayFlag = displayFigure;
}

void	CmShapeContext::setSampling(int numberOfSampling)
{
	nSampling = numberOfSampling;
}

void	CmShapeContext::setInteration(int iterationTimes)
{
	nIteration = iterationTimes;
}

void	CmShapeContext::setDummyFraction(double dummyFraction)
{
	dummyFrac = dummyFraction;
}

void	CmShapeContext::setLogFile(FILE* newLogFile)
{
	logFile = newLogFile;
}

void	CmShapeContext::setDisplaySize(int size)
{
	displaySize = size;
}

/*************** Service methods *****************/
double	CmShapeContext::shapeMatch(const char *imgName1, const char *imgName2, double &matchCost, double &affCost)
{
	printf("shape match: ima1 = %s, img2 = %s\n", imgName1, imgName2);

	IplImage* img1_8U = cvLoadImage(imgName1, CV_LOAD_IMAGE_GRAYSCALE);
	IplImage* img2_8U = cvLoadImage(imgName2, CV_LOAD_IMAGE_GRAYSCALE);

/*	const int maxSize = 400;

	cv::Mat tmpImg1;
	tmpImg1 = cv::Mat(img1_8U);
	int width = tmpImg1.cols;
	int height = tmpImg1.rows;
	if (width * height > maxSize * maxSize)  // need to be shrink
	{
		printf("RESIZE: (%d, %d) -> ", width, height);
		double scale = height * 1.0 / maxSize;
		if (width > height)
			scale = width * 1.0 / maxSize;
		int mWidth = floor(width * 1.0 / scale);
		int mHeight = floor(height * 1.0 / scale);
		cv::resize(tmpImg1, tmpImg1, cv::Size(mWidth, mHeight));
		
		printf("(%d, %d)\n", tmpImg1.cols, tmpImg1.rows);
		*img1_8U = IplImage(tmpImg1);
	}
*/
	if (img1_8U == NULL)
	{
		printf("error 1\n");
		addLog("[error] File %s don't exist\n", imgName1);
		return 0;
	}
	if (img2_8U == NULL)
	{
		printf("error 2\n");
		addLog("[error] File %s don't exist\n", imgName2);
		return 0;
	}

//	displayImage(img1_8U, imgName1);
//	displayImage(img2_8U, imgName2);

	double scaleRatio = shapeMatch(img1_8U, img2_8U, matchCost, affCost);

	printf("shape match, scaleRatio = %lf\n", scaleRatio);

	cvReleaseImage(&img1_8U);
	cvReleaseImage(&img2_8U);
	return scaleRatio;
}

double	CmShapeContext::shapeMatch(const IplImage *img1_8U, const IplImage *img2_8U, double &matchCost, double &affCost)
{
	PointSet	pntSet1, pntSet2;
	vector<double>	tangentSet1, tangentSet2;

	extractBoundary(img1_8U, pntSet1, tangentSet1);
//	displayPointSetWithTangents(pntSet1, tangentSet1, "Boundary of img1");
	extractBoundary(img2_8U, pntSet2, tangentSet2);
//	displayPointSetWithTangents(pntSet2, tangentSet2, "Boundary of img2");

	return shapeMatch(pntSet1, tangentSet1, pntSet2, tangentSet2, matchCost, affCost);
}

double	CmShapeContext::shapeMatch(PointSet &pntSet1, vector<double>& tangentSet1,
					PointSet &pntSet2, vector<double>& tangentSet2,
					double &matchCost, double &affCost)
{
	if (pntSet1.size() > (size_t) nSampling)
		getSampling(pntSet1, tangentSet1);
	if (pntSet2.size() > (size_t) nSampling)
		getSampling(pntSet2, tangentSet2);
	
	int	n = pntSet1.size();
	int	m = pntSet2.size();
	int	leng = round(max(n, m) * (1 + dummyFrac));
	int	i, j;

	vector<ShapeContext>	scData1( n );
	vector<ShapeContext>	scData2( m );

	bool	*alive1, *alive2, *mx, *my;
	int	*cx, *cy;
	double	*lx, *ly;
	double	**cost;

	alive1	= new bool[leng];
	alive2	= new bool[leng];
	mx	= new bool[leng];
	my	= new bool[leng];
	cx	= new int[leng];
	cy	= new int[leng];
	lx	= new double[leng];
	ly	= new double[leng];

	cost = new double*[leng];
	for (i = 0; i < leng; i ++)
		cost[i] = new double[leng];
	
	memset(alive1, 1, leng);
	memset(alive2, 1, leng);

	PointSet	curtPntSet = pntSet1;

	vector<double>	scCostList;
	vector<double>	affCostList;

	PointSet	vpA, vpB;
	double scale2 = 1.0, scale1 = 1.0;

	for (int it = 1; it <= nIteration; it ++)
	{
		addLog("Iteration %d ----------------------\n" , it);
		double	meanDist1 = calcShapeContextData(curtPntSet, scData1, alive1);
		double	meanDist2 = calcShapeContextData(pntSet2, scData2, alive2);
		scale1 *= meanDist1;
		scale2 *= meanDist2;
		if (gridPnts.size())
		{
			for (size_t i = 0; i < gridPnts.size(); i++)
			{
				gridPnts[i].x /= meanDist1;
				gridPnts[i].y /= meanDist1;
			}
		}

		for (i = 0; i < leng; i ++)
			for (j = 0; j < leng; j ++)
				if (i >= n || j >= m)
					cost[i][j] = DUMMY_COST;
				else	
				{
					cost[i][j] = shapeContextCost(scData1[i], scData2[j]);
	//				_ASSERTE(cost[i][j]>=DUMMY_COST);
				}
		
		matchCost = hungaryMatch(cost, cx, cy, lx, ly, mx, my, leng);
		matchCost = matchCost/leng;
		addLog("Matching Cost = %.3lf\n" , matchCost);
		
//		cout<<"match cost: "<<matchCost<<endl;
//		displayMatchingResult(curtPntSet, pntSet2, cx);				

		double scCost;
		double	rowLower, rowMin;
		double	colLower, colMin;

		rowLower = 0;
		for (i = 0; i < n; i ++)
		{
			rowMin = INFINITY;
			for (j = 0; j < m; j ++)
				if (cost[i][j] < rowMin) rowMin = cost[i][j];
			rowLower += rowMin;
		}
		colLower = 0;
		for (j = 0; j < m; j ++)
		{
			colMin = INFINITY;
			for (i = 0; i < n; i ++)
				if (cost[i][j] < colMin) colMin = cost[i][j];
			colLower += colMin;
		}
		rowLower /= n; colLower /= m;
		scCost = max(rowLower, colLower);
		scCostList.push_back( scCost );

		addLog("scCost = %.3lf\n" , scCost);
//		cout<<"scCost: "<<scCost<<endl;

		vpA.clear();
		vpB.clear();
		for (i = 0; i < n; i ++)
		{
			if (cx[i] < m)
			{
				vpA.push_back( curtPntSet[i] );
				vpB.push_back( pntSet2[ cx[i] ] );
			}
		}

		double	beta_k = sqr(meanDist2) * (it == 1 ? 1000 : 1);
		findCdMap = (it == nIteration);
		affCost = calcAffineTransformation(curtPntSet, pntSet2, vpA, vpB, beta_k);
		affCostList.push_back(affCost);

		for (i = 0; i < n; i ++)
			alive1[i] = (cx[i] < m);
		for (i = 0; i < m; i ++)
			alive2[i] = (cy[i] < n);
	}

	delete[] alive1;
	delete[] alive2;
	delete[] mx;
	delete[] my;
	delete[] cx;
	delete[] cy;
	delete[] lx;
	delete[] ly;

	for (i = 0; i < leng; i ++)
		delete[] cost[i];
	delete[] cost;


	if (gridPnts.size())
	{
		for (size_t i = 0; i < gridPnts.size(); i++)
		{
			gridPnts[i].x *= scale2;
			gridPnts[i].y *= scale2;
		}
	}

//	addLog("scCost = %.3lf, affCost = %.3lf\n", scCost, affCost);
//	cout<<"affCost: "<<affCost<<endl;

	return scale1 / scale2;
}

/*************** Logging & Display *****************/
void	CmShapeContext::addLog(const char *fmt, ...)
{
	if (logFile != NULL)
	{
		va_list ap;
		va_start(ap, fmt);
		vfprintf(logFile, fmt, ap);
		va_end(ap);
	}
}

void	CmShapeContext::displayImage(const IplImage* img, const char* title)
{
	if (displayFlag)
	{
		cvNamedWindow(title);
		cvShowImage(title, img);
		cvWaitKey(1);
		//cvDestroyWindow(title);
	}
}

void	CmShapeContext::displayPointSet(const PointSet& pntSet, const char* title)
{
	if (displayFlag)
	{
		int	size = 0;
		for (unsigned int i = 0; i < pntSet.size(); i ++)
		{
			if (pntSet[i].x > size) size = (int)pntSet[i].x;
			if (pntSet[i].y > size) size = (int)pntSet[i].y;
		}
		
		size ++;
		IplImage* img = cvCreateImage( cvSize(displaySize + 10, displaySize + 10), IPL_DEPTH_8U, 1 );

		addLog("Display PointList(size=%d), height=%d, width=%d\n", pntSet.size(), size, size);

		int	step = img->widthStep;
		uchar*	data = (uchar*)img->imageData;
		memset(data, 0, img->height * img->width);
		for (unsigned int i = 0; i < pntSet.size(); i ++)
			data[round(pntSet[i].y/*x*/ / size * displaySize) * step + round(pntSet[i].x/*y*/ / size * displaySize)] = 255;

		displayImage(img, title);
		cvReleaseImage( &img );
	}
}

void	CmShapeContext::displayPointSetWithTangents(const PointSet& pntSet, const vector<double>& tangentSet, const char* title)
{	
	if (displayFlag)
	{
		int	size = 0;
		for (unsigned int i = 0; i < pntSet.size(); i ++)
		{
			if (pntSet[i].x > size) size = (int)pntSet[i].x;
			if (pntSet[i].y > size) size = (int)pntSet[i].y;
		}
		
		size ++;
		IplImage* img = cvCreateImage( cvSize(displaySize + 10, displaySize + 10), IPL_DEPTH_8U, 3 );

		addLog("Display PointList(size=%d), height=%d, width=%d\n", pntSet.size(), size, size);

		int	step = img->widthStep;
		uchar*	data = (uchar*)img->imageData;
		memset(data, 0, img->height * img->width * img->nChannels);
		for (unsigned int i = 0; i < pntSet.size(); i ++)
		{
			cvCircle(img, cvPoint(round(pntSet[i].y / size * displaySize), round(pntSet[i].x / size * displaySize)), 
				2, cvScalar(255,255,255,0));

			// 打印切线方向(buggy)
			/*
			cvLine(img, cvPoint(round(pntSet[i].y / size * displaySize), round(pntSet[i].x / size * displaySize)),
				    cvPoint(round((pntSet[i].y + 4 * cos(tangentSet[i])) / size * displaySize),
				            round((pntSet[i].x + 4 * sin(tangentSet[i])) / size * displaySize)),
					CV_RGB(235,231,82) );
			*/
		}

		displayImage(img, title);
		cvReleaseImage( &img );
	}
}

ColorImage	CmShapeContext::GenMatchingResultImage(const PointSet& pntSet1, const PointSet& pntSet2,  const int *match)
{
	double	size = 0;
	for (unsigned int i = 0; i < pntSet1.size(); i ++)
	{
		if (pntSet1[i].x > size) size = pntSet1[i].x;
		if (pntSet1[i].y > size) size = pntSet1[i].y;
	}
	for (unsigned int i = 0; i < pntSet2.size(); i ++)
	{
		if (pntSet2[i].x > size) size = pntSet2[i].x;
		if (pntSet2[i].y > size) size = pntSet2[i].y;
	}
	
	size += 1.0;
	IplImage* img = cvCreateImage( cvSize(displaySize + 10, displaySize + 10), IPL_DEPTH_8U, 3 );

	for (unsigned int i = 0; i < pntSet1.size(); i ++)
	{
		cvCircle(img, cvPoint(round(pntSet1[i].x / size * displaySize), round(pntSet1[i].y / size * displaySize)), 
			2, CV_RGB(196,196,0));
	}
	//displayImage(img, "first sample image");
	for (unsigned int i = 0; i < pntSet2.size(); i ++)
	{
		cvCircle(img, cvPoint(round(pntSet2[i].x / size * displaySize), round(pntSet2[i].y / size * displaySize)), 
			2, CV_RGB(235,61,126));
	}
	//displayImage(img, "second sample image");
	for (unsigned  int i = 0; i < pntSet1.size(); i ++)
	{
		if (match[i] < (int)pntSet2.size())
		{
			cvLine( img,
				cvPoint(round(pntSet1[i].x / size * displaySize), round(pntSet1[i].y / size * displaySize)),
				cvPoint(round(pntSet2[match[i]].x / size * displaySize), round(pntSet2[match[i]].y / size * displaySize)),
				CV_RGB(64,232,223) );
		}
	}

	ColorImage rltImg = img;
	cvReleaseImage( &img );
	return rltImg;
}

void CmShapeContext::DisplayMatchingResultImage(const PointSet& pntSet1, const PointSet& pntSet2,  const int *match)
{
	ColorImage img = GenMatchingResultImage(pntSet1,pntSet2,match);
	ShowImage("matching samples", img);
}


/*************** Private Methods *****************/

#define byte unsigned char

/* 摘取边界Canny */
void	CmShapeContext::extractBoundary(const IplImage* img_8U, PointSet& pntSet, vector<double>& tangentSet)
{
	IplImage*	pCannyImg = cvCreateImage(cvGetSize(img_8U), IPL_DEPTH_8U, 1);	
	cvCanny(img_8U, pCannyImg, 200, 400, 3); // 边缘检测
	pntSet.clear();
	tangentSet.clear();
	for (int x = 0; x < pCannyImg->height; x++)
	{
		byte* data = (byte*)(pCannyImg->imageData + pCannyImg->widthStep * x);
		for (int y = 0; y < pCannyImg->width; y++)
		{
			if (data[y])
			{
				pntSet.push_back(Point(y, x));
			}
		}
	}

	// 算切线角度(buggy), 先大家都算0(即不考虑切线代价)
	/*
	CvMat*	xDiff = cvCreateMat(img_8U->height, img_8U->width, CV_32FC1);
	CvMat*	yDiff = cvCreateMat(img_8U->height, img_8U->width, CV_32FC1);
	cvSobel(img_8U, xDiff, 1, 0, 7);
	cvSobel(img_8U, yDiff, 0, 1, 7);
	*/

	for (unsigned int i = 0; i < pntSet.size(); i ++)
	{
	/*	tangentSet.push_back( atan2(cvmGet(xDiff, pntSet[i].y, pntSet[i].x),
						cvmGet(yDiff, pntSet[i].y, pntSet[i].x) )  );
	*/
		tangentSet.push_back( 0 );
	}

//	displayPointSet(pntSet, "before sampling");
//	getSampling(pntSet, tangentSet);
//	cvReleaseImage(&pCannyImg);
//	displayPointSet(pntSet, "after sampling");


/*	CvMemStorage *stor=cvCreateMemStorage(0);
	CvSeq *cont=NULL;
	cvFindContours(pCannyImg,stor,&cont,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));  //对二值图像提轮廓
	cvZero(img_8U);
	cvDrawContours(img_8U,cont,cvScalarAll(255),cvScalarAll(255),100);
	cvNamedWindow("Contours",1);
	cvShowImage("Contours",img_8U);
	cvWaitKey();
	cvReleaseImage(&img_8U);
	cvDestroyWindow("Contours");*/
}

void	CmShapeContext::extractOuterBoundary(const IplImage* img, PointSet& pntSet)
{
	IplImage*	pCannyImg = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);	
	cvCanny(img, pCannyImg, 200, 400, 3); // 边缘检测

	pntSet.clear();

	int v, i, j, k, m, tmpI, tmpJ;
	stack<int> vertexS;
	int xIdx[4] = {1, -1, 0, 0};
	int yIdx[4] = {0, 0, 1, -1};

	vector<vector<bool>> compIndex2;
	compIndex2.resize(img->height);
	for (i = 0; i < img->height; i++)
		compIndex2[i].resize(img->width);
	for (i = 0; i < img->height; i++)
		for (k = 0; k < img->width; k++)
			compIndex2[i][k] = false;

	vertexS.push(0);
	while (!vertexS.empty())
	{
		v = vertexS.top();
		vertexS.pop();
		i = v / img->width;
		j = v % img->width;
		compIndex2[i][j] = true;

		for (m = 0; m < 4; m++)
		{
			tmpI = i + xIdx[m];
			tmpJ = j + yIdx[m];
			if ((tmpI >= 0) && (tmpI < img->height) && (tmpJ >= 0) && (tmpJ < img->width) && (!compIndex2[tmpI][tmpJ]))
			{
				byte* data = (byte*)(pCannyImg->imageData + pCannyImg->widthStep * tmpI);
				if (data[tmpJ])
				{
					pntSet.push_back(Point(tmpJ, tmpI));
				}
				else
				{
					vertexS.push(tmpI * pCannyImg->width + tmpJ);
				}
			} //if
		}
	} //while

	displayPointSet(pntSet, "Outer Edge");

	cvReleaseImage( &pCannyImg );
}

void	CmShapeContext::extractContour(const IplImage* img_8U, PointSet& pntSetbfSample, PointSet& pntSet, 
									   vector<int>& isSample, vector<int>& contourEndIdx, vector<double>& tangentSet)
{
	pntSetbfSample.clear();
	pntSet.clear();
	isSample.clear();
	tangentSet.clear();
	contourEndIdx.clear();

	CvMemStorage *stor=cvCreateMemStorage(0);
	CvMemStorage *mstor=cvCreateMemStorage(0);
	CvSeq *cont=NULL, *mcont=NULL;
//	cvThreshold(img_8U,img_8U,50,255,CV_THRESH_BINARY);
	IplImage*	pCannyImg = cvCreateImage(cvGetSize(img_8U), IPL_DEPTH_8U, 1);
	cvCanny(img_8U, pCannyImg, 200, 400, 3); // 边缘检测

	cvNamedWindow("pCannyImg",1);
	cvShowImage("pCannyImg",pCannyImg);
	cvWaitKey(100);
//	cvReleaseImage(&pCannyImg);
//	cvDestroyWindow("pCannyImg");

	cvFindContours(pCannyImg,stor,&cont,sizeof(CvContour),/*CV_RETR_LIST*/CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0)); //对二值图像提轮廓
	
	IplImage *imgS = cvCreateImage(cvGetSize(img_8U), IPL_DEPTH_8U, 1);
	cvZero(imgS);
	cvDrawContours(imgS,cont,cvScalarAll(255),cvScalarAll(255),100);
	cvNamedWindow("Contours",1);
	cvShowImage("Contours",imgS);
	cvWaitKey(100);
//	cvReleaseImage(&imgS);
//	cvDestroyWindow("Contours");

	PointSet pntSetTemp; // 每个轮廓的点
	double areaThresh = 0.05 * img_8U->width * img_8U->height;
	int cnt = 0, seqCnt = 0, i;
	for(;cont;cont = cont->h_next){
		CvRect r = ((CvContour*)cont)->rect;
		if( r.height * r.width < areaThresh ) continue ; // 忽略小块的轮廓

		// -------------方法1：直接计算轮廓面积-----------
		float m_fArea=fabs(cvContourArea(cont,CV_WHOLE_SEQ));
		printf("%f\n",m_fArea);
		cnt++;

		pntSetTemp.clear();

		CvMemStorage *stortmp=cvCreateMemStorage(0);
		CvSeq* contTemp = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , stortmp);
		CvPoint pt ;
		CvSeqReader reader;
		cvStartReadSeq(cont,&reader);
		for( i = 0 ; i < cont->total; i++ ){
			CV_READ_SEQ_ELEM(pt, reader);
			cvSeqPush( contTemp, &pt ) ;

			pntSetbfSample.push_back(Point(pt.x, pt.y));
			pntSetTemp.push_back(Point(pt.x, pt.y));
	//		printf("%d %d\n", pt.x, pt.y);
		}
		displayPointSet(pntSetTemp,"one contour");

		// extract dominant points of contours
	/*	mcont = cvFindDominantPoints(contTemp, mstor, 1, 0, 0, 0, 0);
		PointSet pntSet2;
		pntSet2.clear();
		int idx;
		CvPoint pt2;
		for( i = 0 ; i < mcont->total; i++ ){
			idx = *(int *) cvGetSeqElem(mcont, i);
			pt2 = *(CvPoint *) cvGetSeqElem(contTemp, idx);

			isSample.push_back(idx + seqCnt); //对应pntSetbfSample的序号
			pntSet.push_back(Point(pt2.x, pt2.y));
			pntSet2.push_back(Point(pt2.x, pt2.y));
			printf("%d ", idx + seqCnt);
		}
		displayPointSet(pntSet2,"dominant");*/

		seqCnt += cont->total;  
		contourEndIdx.push_back(seqCnt); // contour end index

		//sampling
	}
	printf("%d\n",cnt);

	// 均匀sampling
/*	int interv;
	if (pntSetbfSample.size() < nSampling)
	{
		pntSet = pntSetbfSample;
	}
	else
	{
		interv = (int)(pntSetbfSample.size() / nSampling) + 1;
		PointSet pntSet2;
		pntSet2.clear();
		for (i = 0; i < pntSetbfSample.size(); i++)
		{
			isSample.push_back(i); //对应pntSetbfSample的序号
			pntSet.push_back(pntSetbfSample[i]);
			pntSet2.push_back(pntSetbfSample[i]);
			i += interv;
		}
		displayPointSet(pntSet2,"dominant");
	}*/

	// sampling(扔掉距离近的点)
	if (pntSetbfSample.size() < nSampling)
	{
		pntSet = pntSetbfSample;
	}
	else
	{
		pntSet = pntSetbfSample;
		getSamplingForExtractContour(pntSet, isSample);
		displayPointSet(pntSet,"sampling");
	}

	for (i = 0; i < pntSet.size(); i++)
	{
		tangentSet.push_back( 0 );
	}
}

/* 边界取点 */
void  CmShapeContext::getSampling(PointSet& pntSet, vector<double> tangentSet)
{
	addLog("Start sampling : nSampling = %d, pntSet.size() = %d\n", nSampling, pntSet.size());

	if (pntSet.size() > (size_t)nSampling)
	{
		int	startSize = min(int(pntSet.size()), nSampling * 3);

		vector<int>	idlist(pntSet.size());
		vector<bool>	alive(pntSet.size(), true);

		for (unsigned int i = 0; i < idlist.size(); i ++)
			idlist[i] = i;

		random_shuffle(idlist.begin(), idlist.end());
		
		vector< pair< double, pair<int,int> > >		pwDist(startSize * (startSize - 1) / 2);
		int						cc = 0;

		for (int i = 0; i < startSize; i ++)
			for (int j = i + 1; j < startSize; j ++)
				pwDist[cc ++] = make_pair( sqrDist(pntSet[idlist[i]], pntSet[idlist[j]]),
							make_pair(idlist[i], idlist[j]) );
		sort(pwDist.begin(), pwDist.end());

		for (int nodeLeft = startSize, i = 0; nodeLeft > nSampling && i < cc; i ++)
		{
			if (alive[pwDist[i].second.first] && alive[pwDist[i].second.second])
				alive[pwDist[i].second.first] = false, nodeLeft --;
		}

		PointSet	pntSetNew(nSampling);
		vector<double>	tangentSetNew(nSampling);

		cc = 0;
		for (int i = 0; i < startSize; i ++)
			if (alive[idlist[i]])
			{
				pntSetNew[cc] = pntSet[idlist[i]];
				tangentSetNew[cc] = tangentSet[idlist[i]];
				cc ++;
			}

		pntSet = pntSetNew;
		tangentSet = tangentSetNew;
	}
}

/* 轮廓取点，返回对应采样前的index */
void  CmShapeContext::getSamplingForExtractContour(PointSet& pntSet, vector<int>& isSample)
{
	if (pntSet.size() > (size_t)nSampling)
	{
		int	startSize = min(int(pntSet.size()), nSampling * 3);

		vector<int>	idlist(pntSet.size());
		vector<bool>	alive(pntSet.size(), true);

		for (unsigned int i = 0; i < idlist.size(); i ++)
			idlist[i] = i;

		random_shuffle(idlist.begin(), idlist.end());
		
		vector< pair< double, pair<int,int> > >		pwDist(startSize * (startSize - 1) / 2);
		int						cc = 0;

		for (int i = 0; i < startSize; i ++)
			for (int j = i + 1; j < startSize; j ++)
				pwDist[cc ++] = make_pair( sqrDist(pntSet[idlist[i]], pntSet[idlist[j]]),
							make_pair(idlist[i], idlist[j]) );
		sort(pwDist.begin(), pwDist.end());

		for (int nodeLeft = startSize, i = 0; nodeLeft > nSampling && i < cc; i ++)
		{
			if (alive[pwDist[i].second.first] && alive[pwDist[i].second.second])
				alive[pwDist[i].second.first] = false, nodeLeft --;
		}

		for (int i = 0; i < startSize; i ++)
		{
			if (alive[idlist[i]])
			{
				isSample.push_back(idlist[i]);
			}
		}
		sort(isSample.begin(), isSample.end());

		PointSet	pntSetNew(nSampling);
		cc = 0;
		for (int i = 0; i < isSample.size(); i ++)
		{
			pntSetNew[i] = pntSet[isSample[i]];
		}

		pntSet = pntSetNew;
	}
	else
	{
		for (int tt = 0; tt < pntSet.size();tt++)
			isSample.push_back(tt);
	}
}

double	CmShapeContext::calcShapeContextData(PointSet& pntSet, vector<ShapeContext>& scData, bool *alive)
{
	int	n = pntSet.size();
	int	i, j, cc = 0;
	double	meanDist = 0;

	for (i = 0; i < n; i ++)
		if (alive[i])
			for (j = i + 1; j < n; j ++)
				if (alive[j])
					meanDist += pointDist(pntSet[i], pntSet[j]), cc ++;
	meanDist /= cc;

	// 距离规范化
	for (i = 0; i < n; i ++)
	{
		pntSet[i].x /= meanDist;
		pntSet[i].y /= meanDist;
	}

	double	d, lv, theta;
	double	sectorAngle = PI * 2.0 / SHAPE_CONTEXT_SECTOR;
	int	r, s;

	// 计算Shape Context
	for (i = 0; i < n; i ++)
	{
		memset(scData[i].h, 0, sizeof(scData[i].h));
		for (j = 0; j < n; j ++)
			if (alive[j] && j != i)
			{
				d = pointDist(pntSet[i], pntSet[j]);
				for (r = 0, lv = SHAPE_CONTEXT_INNER_R; r < SHAPE_CONTEXT_REGION; r ++, lv += lv)
					if (d <= lv) break;
				if (r < SHAPE_CONTEXT_REGION)
				{
					theta = atan2(pntSet[j].y - pntSet[i].y, pntSet[j].x - pntSet[i].x);
					for (s = 0, lv = - PI + sectorAngle; s + 1 < SHAPE_CONTEXT_SECTOR; s ++, lv += sectorAngle)
						if (theta <= lv) break;
					scData[i].h[r][s] ++;
				}
			}
	}

	return	meanDist;
}

double	CmShapeContext::shapeContextCost(const ShapeContext& A, const ShapeContext& B)
{
	double	ret = 0;
	double	ca = EPS, cb = EPS;

	for (int r = 0; r < SHAPE_CONTEXT_REGION; r ++)
		for (int s = 0; s < SHAPE_CONTEXT_SECTOR; s ++)
			ca += A.h[r][s], cb += B.h[r][s];

	// Both no neighbors
	if (ca < 1 - EPS && cb < 1 - EPS) return 0;

	for (int r = 0; r < SHAPE_CONTEXT_REGION; r ++)
		for (int s = 0; s < SHAPE_CONTEXT_SECTOR; s ++)
			if (A.h[r][s] || B.h[r][s])
				ret += sqr(A.h[r][s] / ca - B.h[r][s] / cb) / (A.h[r][s] / ca + B.h[r][s] / cb);

	_ASSERTE(ret>=0);
	return ret / 2.0;
}

bool	CmShapeContext::hungaryExtendPath(int u, double **cost, int *cx, int *cy, double* lx, double* ly, bool *mx, bool *my, int n)
{
	mx[u] = 1;
	for (int v = 0; v < n; v ++)
		if (doubleCmp(cost[u][v], lx[u] + ly[v]) == 0 && ! my[v])
		{
			my[v] = 1;
			if (cy[v] < 0 || hungaryExtendPath(cy[v], cost, cx, cy, lx, ly, mx, my, n))
				return cx[u] = v, cy[v] = u, true;
		}
	return false;
}

double	CmShapeContext::hungaryMatch(double **cost, int *cx, int *cy, double* lx, double* ly, bool *mx, bool *my, int n)
{
	int	u, i, j;
	double	alpha;	

	for (i = 0; i < n; i ++)
	{
		cx[i] = cy[i] = -1;	
		lx[i] = INFINITY;
		ly[i] = 0;

		for (j = 0; j < n; j ++)
			if (cost[i][j] < lx[i])
				lx[i] = cost[i][j];
	}

	for (u = 0; u < n; u ++)
	{
		while (cx[u] < 0)
		{
			memset(mx, 0, n);
			memset(my, 0, n);

			if (hungaryExtendPath(u, cost, cx, cy, lx, ly, mx, my, n))
				break;

			alpha = INFINITY;
			for (i = 0; i < n; i ++)
				if (mx[i])
					for (j = 0; j < n; j ++)
						if (!my[j])
							alpha = min(alpha, cost[i][j] - lx[i] - ly[j]);
			for (i = 0; i < n; i ++)
				if (mx[i]) lx[i] += alpha;
			for (i = 0; i < n; i ++)
				if (my[i]) ly[i] -= alpha;
		}
	}

	double	matchCost = 0;
	for (i = 0; i < n; i ++)
		matchCost += lx[i] + ly[i];

	return	matchCost;
}

double	CmShapeContext::calcAffineTransformation(PointSet& curtPntSet, const PointSet& tarPntSet, const PointSet& pA, const PointSet& pB, double beta_k)
{
	int	n = pA.size();
	int	i, j;

	CvMat	*L = cvCreateMat(n + 3, n + 3, CV_32FC1);
	CvMat	*invL = cvCreateMat(n + 3, n + 3, CV_32FC1);
	CvMat	*V = cvCreateMat(n + 3, 2, CV_32FC1);	

	// Set L
	for (i = 0; i < n; i ++)
	{
		for (j = 0; j < n; j ++)
		{
			if (i == j)
			{
				cvmSet(L, i, j, beta_k);
			}
			else
			{
				double	d = sqrDist(pA[i], pA[j]);
				cvmSet(L, i, j, d * log(d));
			}
		}
	}

	for (i = 0; i < n; i ++)
	{
		cvmSet(L, i, n, 1.0);
		cvmSet(L, i, n+1, pA[i].x);
		cvmSet(L, i, n+2, pA[i].y);

		cvmSet(L, n, i, 1.0);
		cvmSet(L, n+1, i, pA[i].x);
		cvmSet(L, n+2, i, pA[i].y);
	}

	for (i = n; i < n + 3; i ++)
		for (j = n; j < n + 3; j ++)
			cvmSet(L, i, j, 0);

	// Set V
	for (i = 0; i < n; i ++)
	{
		cvmSet(V, i, 0, pB[i].x);
		cvmSet(V, i, 1, pB[i].y);
	}
	for (i = n; i < n + 3; i ++)
	{
		cvmSet(V, i, 0, 0);
		cvmSet(V, i, 1, 0);
	}

	cvInvert(L, invL);

	CvMat *C = cvCreateMat(n+3, 2, CV_32FC1);
	cvMatMul(invL, V, C);

	// bennding energy
	double	E = 0;
	for (int p = 0; p < 2; p ++)
	{
		double	val = 0;
		for (i = 0; i < n; i ++)
			for (j = 0; j < n; j ++)
				if (i != j)
					val += cvmGet(C, i, p) * cvmGet(L, i, j) * cvmGet(C, j, p);
		E += val;
	}
	E /= 2.0;
	addLog("Bending energy = %.3lf\n" , E);

	CvMat	*A = cvCreateMat(2, 2, CV_32FC1);
	cvmSet(A, 0, 0, cvmGet(C, n + 1 , 0));
	cvmSet(A, 1, 0, cvmGet(C, n + 2 , 0));
	cvmSet(A, 0, 1, cvmGet(C, n + 1 , 1));
	cvmSet(A, 1, 1, cvmGet(C, n + 2 , 1));

	CvMat	*S = cvCreateMat(2, 1, CV_32FC1);
	cvSVD(A, S);

	double affCost = log(cvmGet(S, 0, 0) / cvmGet(S, 1, 0));
	addLog("AffineCost = %.3lf\n" , affCost);

	double	* cx = new double[n + 3];
	double	* cy = new double[n + 3];

	for (i = 0; i < n + 3; i ++)
	{
		cx[i] = cvmGet(C, i, 0);
		cy[i] = cvmGet(C, i, 1);
	}

	//// DO Affine Transformation & Display

	makeAffineTransformation(curtPntSet, pA, cx, cy);

	if (displayFlag && gridPnts.size() == 0)
	{
		gridPnts.resize(30 * 30);
		int		cc = 0;

		double		size = 0;
		for (i = 0; i < (int)curtPntSet.size(); i ++)
			size = max(size, max(curtPntSet[i].x, curtPntSet[i].y));
		for (i = 0; i < (int)tarPntSet.size(); i ++)
			size = max(size, max(tarPntSet[i].x, tarPntSet[i].y));
		size /= 25;
		for (i = 0; i < 30; i ++)
			for (j = 0; j < 30; j ++)
			{
				gridPnts[cc].x = i * size;
				gridPnts[cc].y = j * size;
				cc ++;
			}

	}

	if (gridPnts.size() != 0)
		makeAffineTransformation(gridPnts, pA, cx, cy);

	//if (displayFlag)
	//	displayAffine(gridPnts, curtPntSet, tarPntSet);

	if (findCdMap && gridPnts.size() > 0)
		makeAffineTransformation(gridPnts, pA, cx, cy);


	delete[] cx;
	delete[] cy;

	cvReleaseMat(&L);
	cvReleaseMat(&invL);
	cvReleaseMat(&V);
	cvReleaseMat(&C);
	cvReleaseMat(&A);
	cvReleaseMat(&S);
	return affCost;
}

void	CmShapeContext::makeAffineTransformation(PointSet& pntSet, const PointSet& pA, double *cx, double *cy)
{
	int	leng = max(pntSet.size(), pA.size() + 3);
	double	* aff = new double[ leng ];
	double	* wrp = new double[ leng ];

	PointSet	dst(pntSet.size());

	int	i , j;
	int	n = pntSet.size();
	int	n_good = pA.size();

	double	d2, u;

	for (i = 0; i < n; i ++)
		aff[i] = cx[n_good] + cx[n_good + 1] * pntSet[i].x + cx[n_good + 2] * pntSet[i].y;

	memset(wrp, 0, leng * sizeof(double));
	for (i = 0; i < n_good; i ++)
		for (j = 0; j < n; j ++)
		{
			d2 = sqrDist(pA[i], pntSet[j]);
			u = d2 * log(d2 + EPS);
			wrp[j] += cx[i] * u;
		};
	for (i = 0; i < n; i ++)
		dst[i].x = aff[i] + wrp[i];

	for (i = 0; i < n; i ++)
		aff[i] = cy[n_good] + cy[n_good + 1] * pntSet[i].x + cy[n_good + 2] * pntSet[i].y;

	memset(wrp, 0, leng * sizeof(double));
	for (i = 0; i < n_good; i ++)
		for (j = 0; j < n; j ++)
		{
			d2 = sqrDist(pA[i], pntSet[j]);
			u = d2 * log(d2 + EPS);
			wrp[j] += cy[i] * u;
		};
	for (i = 0; i < n; i ++)
		dst[i].y = aff[i] + wrp[i];

	pntSet = dst;
	delete[] aff;
	delete[] wrp;
}
