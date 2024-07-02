#include "pch.h"


tTransform g_Trans = {};

const char* ASSET_TYPE_STRING[] =
{
	"MESH",
	"MESH_DATA",
	"MATERIAL",
	"PREFAB",
	"TEXTURE",
	"SOUND",
	"GRAPHIC_SHADER",
	"COMPUTE_SHADER",
	"SPRITE",
	"ANIMATION",
};

const char* ToString(ASSET_TYPE _Type) { return ASSET_TYPE_STRING[(UINT)_Type]; }


const char* COMPONENT_TYPE_STRING[] =
{
	"TRANSFORM",
	"COLLIDER2D",
	"COLLIDER3D",
	"ANIMATOR2D",
	"ANIMATOR3D",
	"STATE_MACHINE",
	"RIGIDBODY",
	"CAMERA",
	"MESHRENDER",
	"TILEMAP",
	"PARTICLE_SYSTEM",
	"DECAl",
	"SKYBOX",
	"LANDSCAPE"
};

const char* ToString(COMPONENT_TYPE _Type) { return COMPONENT_TYPE_STRING[(UINT)_Type]; }