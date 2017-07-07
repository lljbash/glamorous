//#include "stdafx.h"
#include "wglobalAdd.h"

void RGB2HSV(float R, float G, float B, float& H, float& S, float&V)
{
	// r,g,b values are from 0 to 1
	// h = [0,360], s = [0,1], v = [0,1]
	// if s == 0, then h = -1 (undefined)

	float _min, _max, delta,tmp;
	tmp = min(R, G);
	_min = min( tmp, B );
	tmp = max( R, G);
	_max = max(tmp, B );
	V = _max; // v

	delta = _max - _min;

	if( _max != 0 )
		S = delta / _max; // s
	else
	{
		// r = g = b = 0 // s = 0, v is undefined
		S = 0;
		H = -1;
		return;
	}

	if( R == _max )
		H = ( G - B ) / delta; // between yellow & magenta
	else if( G == _max )
		H = 2 + ( B - R ) / delta; // between cyan & yellow
	else
		H = 4 + ( R - G ) / delta; // between magenta & cyan

	H *= 60; // degrees
	if( H < 0 )
		H += 360;
}


void HSV2RGB(float H, float S, float V, float &R, float &G, float &B)
{
	int i;
	float f, p, q, t;

	if( S == 0 ) 
	{
		// achromatic (grey)
		R = G = B = V;
		return;
	}

	H /= 60; // sector 0 to 5
	i = floor( H );
	f = H - i; // factorial part of h
	p = V * ( 1 - S );
	q = V * ( 1 - S * f );
	t = V * ( 1 - S * ( 1 - f ) );

	switch( i ) 
	{
	case 0: 
		R = V;
		G = t;
		B = p;
		break;
	case 1:
		R = q;
		G = V;
		B = p;
		break;
	case 2:
		R = p;
		G = V;
		B = t;
		break;
	case 3:
		R = p;
		G = q;
		B = V;
		break;
	case 4:
		R = t;
		G = p;
		B = V;
		break;
	default: // case 5:
		R = V;
		G = p;
		B = q;
		break;
	}
}

void myRGB2HSV(const CVector3f& ColorRGB, CVector3f& ColorHSV)
{
	RGB2HSV(ColorRGB.m_fea[0], ColorRGB.m_fea[1], ColorRGB.m_fea[2],
		    ColorHSV.m_fea[0], ColorHSV.m_fea[1], ColorHSV.m_fea[2]);
}

void myHSV2RGB(const CVector3f& ColorHSV, CVector3f& ColorRGB)
{
	HSV2RGB(ColorHSV.m_fea[0], ColorHSV.m_fea[1], ColorHSV.m_fea[2],
        	ColorRGB.m_fea[0], ColorRGB.m_fea[1], ColorRGB.m_fea[2]);
}