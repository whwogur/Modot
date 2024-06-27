#pragma once

struct Vtx
{
	Vec3	vPos;
	Vec4	vColor;
};

struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;
};

extern tTransform g_Trans;