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
};

extern tTransform g_Trans;