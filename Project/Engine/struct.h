#pragma once

struct Vtx
{
	Vec3	vPos;
	Vec4	vColor;
	Vec2	vUV;
};

struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;
};

struct tMtrlConst
{
	int		iArr[4];
	float	fArr[4];
	Vec2	v2Arr[4];
	Vec4	v4Arr[4];
	Matrix	matArr[4];
	int		btex[TEX_PARAM::END + 2];
};

extern tTransform g_Trans;