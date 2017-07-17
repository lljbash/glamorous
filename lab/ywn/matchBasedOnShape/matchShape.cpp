#include "opencv2/opencv.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

using namespace cv;
using std::cout;
using std::endl;
using std::ifstream;
using std::vector;
using std::string;

const string attCutPath = "../imgsaliency/pyimgsaliency/photo2sketch.py";
const string databasePath = "./database/";
const string styleName[] = {"abstract", "Impressionism", "pointillism", "Post_Impressionism", "shuimo", "suprematism"};

string getRCCImg(const string& path)
{
	string RCCPath = (path.substr(0, path.rfind('.')) + "_RCC.png");
	cout << RCCPath << endl;
	ifstream tmpFin(RCCPath.c_str());
	if (!tmpFin) 
	{
		// create Img_RCC.png
		string attCutCmd = "python " + attCutPath;
		attCutCmd += (" " + path + " " + RCCPath);
		cout << attCutCmd << endl;
		system(attCutCmd.c_str());	
	}

	return RCCPath;
}

void getFiles( string path, vector<string>& files )  
{ 
    const char* basePath = path.c_str();

    // cout << "BASE PATH: " << basePath << endl;

    DIR *dir;
    struct dirent *ptr;
    char base[1000];

    if ((dir=opendir(basePath)) == NULL)
    {
        perror("Open dir error...");
        exit(1);
    }

    while ((ptr=readdir(dir)) != NULL)
    {
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
            continue;
        else if(ptr->d_type == 8 || ptr->d_type == 10)    ///file
        {
            string foo(basePath);
            string bar(ptr->d_name);
            foo = foo + "/" + bar;
            files.push_back(foo);
            cout << foo << endl;
        }   
        else if(ptr->d_type == 4)    ///dir
        {
            memset(base,'\0',sizeof(base));
            strcpy(base,basePath);
            strcat(base,"/");
            strcat(base,ptr->d_name);
            getFiles(string(base), files);
        }
    }
    closedir(dir);
}

double getRCCMatchRatio(const string& srcRccPath, const string& dbRccPath)
{
	Mat srcMat = imread(srcRccPath.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
	Mat dbMat = imread(dbRccPath.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
	
	// resize
	resize(dbMat, dbMat, srcMat.size());;
	int cnt = 0;
	for (int i = 0; i < srcMat.rows; ++i)
		for (int j = 0; j < srcMat.cols; ++j)
		{
			//cout << (int)srcMat.at<uchar>(i,j) << " " << (int)dbMat.at<uchar>(i,j) << endl;
			if (srcMat.at<uchar>(i,j) != 0 && dbMat.at<uchar>(i,j) != 0)
				++cnt;
		}

	return cnt * 1.0 / (srcMat.rows * srcMat.cols);
}

void showBestMatch(const string& srcRCCPath, const string& bestMatchPath)
{
	Mat srcMat = imread(srcRCCPath.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
	Mat matchMat = imread(getRCCImg(bestMatchPath).c_str(), CV_LOAD_IMAGE_GRAYSCALE);
	
	// resize
	resize(matchMat, matchMat, srcMat.size());

	Mat ret = Mat::zeros(srcMat.rows, 3 * srcMat.cols + 2, CV_8UC1 );

	for (int i = 0; i < srcMat.rows; ++i)
	{
		ret.at<uchar>(i, srcMat.cols) = 255;
		ret.at<uchar>(i, 2*srcMat.cols+1) = 255;
		for (int j = 0; j < srcMat.cols; ++j)
		{
			ret.at<uchar>(i, j) = srcMat.at<uchar>(i, j);
			ret.at<uchar>(i, srcMat.cols + 1 + j) = matchMat.at<uchar>(i, j);
			
			ret.at<uchar>(i, 2*srcMat.cols + 2 + j) = 0;
			if (srcMat.at<uchar>(i, j) != 0 && matchMat.at<uchar>(i, j) != 0)
				ret.at<uchar>(i, 2*srcMat.cols + 2 + j) = 255;
		}
	}

	resize(ret, ret, Size(ret.cols/2, ret.rows/2));


	namedWindow("ret");
	imshow("ret",ret);

	waitKey(0);
}


int main(int argc, char* argv[])
{
	/*
	TODO: 
		1. argc, argv -> srcPath, styleSelector 	
		2. if we failed to get the figure from srcPath(that's equal to bestMatchRaio == 0), 
		   maybe we should select a random one rather than the first one.
	*/

	string srcPath = "./aaa.jpg";
	int styleSelector = 4; // 0 - 5
	string styleFolder = (databasePath + styleName[styleSelector]);
	cout << styleFolder << endl;

	Mat src;   
	src=  imread(srcPath.c_str(), CV_LOAD_IMAGE_COLOR);    
    if(!src.data)
    {  
         cout<<"Could not open or find the image"<< endl;  
         return -1;  
    }  

	// get RCC image
	string srcRCCPath = getRCCImg(srcPath);
 //   Mat srcRcc=  imread(RCCPath.c_str(), CV_LOAD_IMAGE_COLOR);    
	cout << "SRC RCC: " << srcRCCPath << endl;

	
    
	double bestMatchRatio = -1;
	string bestMatchPath = "";

	// shape matching
	vector<string> files;  
	getFiles(styleFolder, files);  
	int size = files.size(); 
	for (int i = 0; i < size; ++i) 
	{
		cout << "processing: " << files[i].c_str() << endl; 
	    // rename first
	    string oldName = files[i];
	    string newName = files[i];
		replace(newName.begin(), newName.end(), ' ', '_'); // replace all 'x' to 'y' 
		replace(newName.begin(), newName.end(), '&', '_'); // replace all 'x' to 'y' 
	    rename(oldName.c_str(), newName.c_str()); 
	    files[i] = newName;
	    
	    // skip _RCC & _5c image
	    int rccIndex = files[i].rfind('_');
	    if (rccIndex != string::npos && (files[i].substr(rccIndex) == "_RCC.png"
										|| files[i].substr(rccIndex) == "_5c.png"))
	    	continue;
		// skip txt
	    int pointIndex = files[i].rfind('.');
	    if (rccIndex != string::npos && files[i].substr(pointIndex) == ".txt")
	    	continue;
		
		// get RCC of database image
		string dbRCCPath = getRCCImg(files[i]);
		
		double RCCMatchRatio = getRCCMatchRatio(srcRCCPath, dbRCCPath);
		if (RCCMatchRatio > bestMatchRatio)
		{
			cout << "BEST MATCH UPDATED: " << endl 
				 << RCCMatchRatio << ", " << files[i] << endl; 
			bestMatchRatio = RCCMatchRatio;
			bestMatchPath = files[i];
		}
	}

	cout << "\n\n-------------------------\n"
		 << "BEST MATCH: " << bestMatchPath << endl
		 << "-------------------------\n";

	cout << srcRCCPath << ", " << bestMatchPath << endl;
	showBestMatch(srcRCCPath, bestMatchPath);  
  
    return 0;  
}