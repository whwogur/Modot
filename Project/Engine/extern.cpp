#include "pch.h"


tTransform g_Trans = {};
tGlobalData g_GlobalData = {};

const char* ASSET_TYPE_STRING[] =
{
	"Mesh",
	"Mesh Data",
	"Material",
	"Prefab",
	"Texture",
	"Sound",
	"Graphic Shader",
	"Compute Shader",
	"Sprite",
	"Animation",
};

const char* ToString(ASSET_TYPE _Type) { return ASSET_TYPE_STRING[(UINT)_Type]; }


const char* COMPONENT_TYPE_STRING[] =
{
	"Transform",
	"Collider2D",
	"Collider3D",
	"Light2D",
	"Light3D",
	"Animator2D",
	"Animator3D",
	"Text",
	"RigidBody",
	"Camera",
	"Mesh Render",
	"Tilemap",
	"Particle System",
	"Decal",
	"Skybox",
	"Landscape"
};

const char* ToString(COMPONENT_TYPE _Type) { return COMPONENT_TYPE_STRING[(UINT)_Type]; }