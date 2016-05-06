/*
*
*		2016/4/6		qiu_hao
*/
#ifndef __2D_H__
#define __2D_H__

#include "CQMath.h"

struct Vertex2D
{
	CQ::float3 pos;
	float u, v;			
	CQ::float4 color; //x,y,z,w : r,g,b,a
};

struct Quad
{
	Vertex2D v1;	// lb
	Vertex2D v2;	// rb
	Vertex2D v3;	// rt
	Vertex2D v4;	// lt
};

#endif