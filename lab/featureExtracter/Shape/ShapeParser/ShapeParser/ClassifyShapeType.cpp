#include "ClassifyShapeType.h"


/*
小类：
1	其他直线几何形	
2	直线几何形-正方形、正方体	2.png
3	直线几何形-长方形、长方体	4.png和5.png
4	直线几何形-长方形、长方体-竖用	4.png
5	直线几何形-长方形、长方体-横放	5.png
6	直线几何形-三角形、锥体-正三角形/等腰三角形	6.png
7	直线几何形-三角形、锥体-顶尖朝上	7.png
8	直线几何形-三角形、锥体-顶尖朝下/倒三角形	8.png
9	直线几何形-三角形、锥体-顶尖朝水平方向	9_1.png和9_2.png
10	直线几何形-其他角状-五角星	10.jpg
11	直线几何形-菱形	11.png
12	直线几何形-梯形	12.png
13	其他曲线几何形	
14	曲线几何形-圆形、球体、椭圆、椭球	14.png
15	自由直线形	15.png
16	自由曲线形	16.png


大类：
1	方形	2、3、4、5
2	三角形	6、7、8、9
3	直线几何形	10、11、12
4	圆	14
5	自由直线形	15
6	自由曲线形	16
*/
// 要处理的特殊情况：小类中的第3类对应4.png, 5.png;
//                小类中没有1和13
#define BASE_NUM 14
std::string base[]={"2.png","4.png","5.png","6.png","7.png","8.png","9_1.png",
	"9_2.png","10.jpg","11.png","12.png","14.png","15.png","16.png"};
int index_s[]={2,4,5,6,7,8,9,9,10,11,12,14,15,16}; // base图对应的小类
int index_b[]={1,1,1,2,2,2,2,2,3,3,3,4,5,6};      // base图对应大类
int tag_b[]={0,0,1,1,1,1,2,2,2,2,3,3,3,0,4,5,6}; // 标注的对应的大类


// 返回和每个base匹配的matchCost
void ShapeParser::ClassifyShape(string salfilename, vector<double>& matchCost, int& smallclass, int& bigclass)
{
	string basepath="base/";
	matchCost.resize(BASE_NUM);
	vector<pair<double,int>> allCost(BASE_NUM);
	for (int i=0;i<BASE_NUM;i++)
	{
		double matchCost1, affCost;
		CmShapeContext csc;
		csc.shapeMatch(salfilename.c_str(),(basepath+base[i]).c_str(),matchCost1,affCost);
		allCost[i]=make_pair(matchCost1,i);
		matchCost[i]=matchCost1;
	}
	sort(allCost.begin(),allCost.end());

	smallclass=index_s[allCost[0].second];
	bigclass=index_b[allCost[0].second];
}


// e.g.   "test.jpg", "D:/SummerPractice/ArtAttribute/img/"
void ShapeParser::CalParam(string imgName, string imgPath)
{
//	ifstream fin("D:/SummerPractice/ArtAttribute/img/filename.txt");
	string path=imgPath;
	string pathsal=imgPath;
	string basepath="..\\ShapeParser\\base\\";

	string outPath = imgPath + imgName.substr(0, imgName.rfind('.')) + "_shape.txt";
	ofstream fout(outPath.c_str());
		//"D:/SummerPractice/ArtAttribute/img/testImages_artphoto_out.txt");

	string tmp = imgName;
	int sum=0, cnt_s=0, cnt_b=0;
//	while (fin>>tmp)
//	{
		// find the saliency image path and file
		string tt = tmp.substr(tmp.find('\\')+1,tmp.length());
		tt = tt.substr(0,tt.rfind('.'))+"_RCC.png";
		string salname=pathsal+tt;
		
		printf("%s\n", salname.c_str());

		// if exit saliency image
		IplImage* salImg = cvLoadImage(salname.c_str(),0);
		if (salImg==NULL) // not find image or sal image
			return; //continue;
		cvReleaseImage(&salImg);

		// calculate the mincost shape
		sum++;
		fout<<tmp<<" ";
		cout<<sum<<". ";

		vector<pair<double,int>> allCost(BASE_NUM);
		for (int i=0;i<BASE_NUM;i++)
		{
			double matchCost, affCost;
			CmShapeContext csc;
			csc.shapeMatch(salname.c_str(),(basepath+base[i]).c_str(),matchCost,affCost);
			allCost[i]=make_pair(matchCost,i);

			fout<<matchCost<<" ";
			cout<<i<<" ";
		}
		sort(allCost.begin(),allCost.end());

		//write out--小类和大类
		fout<<index_s[allCost[0].second]<<" "<<index_b[allCost[0].second]<<endl;
		cout<<endl;
//	}
}