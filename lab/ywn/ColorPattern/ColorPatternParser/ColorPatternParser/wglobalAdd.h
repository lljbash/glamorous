#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <math.h>
#include <vector>
using namespace std;


#define NUM_VEC3 3


class CVector3f  
{
public: 
	//无参构造函数 
	CVector3f(float x, float y, float z)
	{
		m_fea[0] = x; 
		m_fea[1] = y;
		m_fea[2] = z;
	}

	CVector3f() 
	{ 
		for (int i = 0; i < NUM_VEC3; i++)
		{
			m_fea[i] = .0;
		}
	}; 
	//有参构造函数 
	CVector3f(float *fpFea) 
	{ 
		//分别将参数赋值给三个成员变量 
		for (int i = 0; i < NUM_VEC3; i++)
		{
			m_fea[i] = fpFea[i];
		}

		if (fpFea)
			delete[] fpFea;
	}; 
	//析构函数 
	~CVector3f(){}; 

	//重载=操作符,实现两向量变量的赋值 
	CVector3f& operator=(const CVector3f& inVet) 
	{ 
		for (int i = 0; i < NUM_VEC3; i++)
		{
			m_fea[i] = inVet.m_fea[i];
		}

		return *this; 
	}; 

	//重载+操作符,实现两向量变量的相加 
	CVector3f operator+(CVector3f vVector) 
	{ 
		//返回相加的结果 
		float *tmp = new float[NUM_VEC3];

		for (int i = 0; i < NUM_VEC3; i++)
		{
			tmp[i] = m_fea[i] + vVector.m_fea[i]; // revised by xiaohui
		}

		return CVector3f(tmp);		
	}

	double wDistance(CVector3f vVector)
	{
		double sum=0;
		for (int i=0; i< 3; i++)
		{
			sum += (m_fea[i] - vVector.m_fea[i])*(m_fea[i] - vVector.m_fea[i]);
		}
		return sqrt(sum);
	}

public:  
	float m_fea[3];
};

// warning H color space in HSV has been regularized to 0 ~ 1  
// new RGB 2 HSV
void RGB2HSV(float R, float G, float B, float& H, float& S, float&V);
void HSV2RGB(float H, float S, float V, float &R, float &G, float &B);
void myRGB2HSV(const CVector3f& ColorRGB, CVector3f& ColorHSV);
void myHSV2RGB(const CVector3f& ColorHSV, CVector3f& ColorRGB);

#endif