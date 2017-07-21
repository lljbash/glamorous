#include "ClassifyShapeType.h"


/*
С�ࣺ
1	����ֱ�߼�����	
2	ֱ�߼�����-�����Ρ�������	2.png
3	ֱ�߼�����-�����Ρ�������	4.png��5.png
4	ֱ�߼�����-�����Ρ�������-����	4.png
5	ֱ�߼�����-�����Ρ�������-���	5.png
6	ֱ�߼�����-�����Ρ�׶��-��������/����������	6.png
7	ֱ�߼�����-�����Ρ�׶��-���⳯��	7.png
8	ֱ�߼�����-�����Ρ�׶��-���⳯��/��������	8.png
9	ֱ�߼�����-�����Ρ�׶��-���⳯ˮƽ����	9_1.png��9_2.png
10	ֱ�߼�����-������״-�����	10.jpg
11	ֱ�߼�����-����	11.png
12	ֱ�߼�����-����	12.png
13	�������߼�����	
14	���߼�����-Բ�Ρ����塢��Բ������	14.png
15	����ֱ����	15.png
16	����������	16.png


���ࣺ
1	����	2��3��4��5
2	������	6��7��8��9
3	ֱ�߼�����	10��11��12
4	Բ	14
5	����ֱ����	15
6	����������	16
*/
// Ҫ��������������С���еĵ�3���Ӧ4.png, 5.png;
//                С����û��1��13
#define BASE_NUM 14
std::string base[]={"2.png","4.png","5.png","6.png","7.png","8.png","9_1.png",
	"9_2.png","10.jpg","11.png","12.png","14.png","15.png","16.png"};
int index_s[]={2,4,5,6,7,8,9,9,10,11,12,14,15,16}; // baseͼ��Ӧ��С��
int index_b[]={1,1,1,2,2,2,2,2,3,3,3,4,5,6};      // baseͼ��Ӧ����
int tag_b[]={0,0,1,1,1,1,2,2,2,2,3,3,3,0,4,5,6}; // ��ע�Ķ�Ӧ�Ĵ���


// ���غ�ÿ��baseƥ���matchCost
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

		//write out--С��ʹ���
		fout<<index_s[allCost[0].second]<<" "<<index_b[allCost[0].second]<<endl;
		cout<<endl;
//	}
}