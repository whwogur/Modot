#pragma once


enum class CB_TYPE
{
	TRANSFORM,

	MATERIAL,
	SPRITE,
	GLOBAL,

	END,
};

enum DIR
{
	RIGHT,
	UP,
	FRONT,
};

enum class ASSET_TYPE
{
	MESH,
	MESH_DATA,
	MATERIAL,
	PREFAB,
	TEXTURE,
	SOUND,
	GRAPHIC_SHADER,
	COMPUTE_SHADER,
	SPRITE,
	ANIMATION,
	END,
};
extern const char* ASSET_TYPE_STRING[(UINT)ASSET_TYPE::END];
const char* ToString(ASSET_TYPE _Type);

enum class COMPONENT_TYPE
{
	TRANSFORM,
	COLLIDER2D,
	COLLIDER3D,
	LIGHT2D,
	LIGHT3D,
	ANIMATOR2D,
	ANIMATOR3D,
	TEXTRENDER,
	RIGIDBODY,
	CAMERA,

	MESHRENDER,
	TILEMAP,
	PARTICLESYSTEM,
	DECAL,
	SKYBOX,
	LANDSCAPE,

	END,

	SCRIPT,
};
extern const char* COMPONENT_TYPE_STRING[(UINT)COMPONENT_TYPE::END];
const char* ToString(COMPONENT_TYPE _Type);

enum class RS_TYPE
{
	CULL_BACK,
	CULL_FRONT,
	CULL_NONE,
	WIRE_FRAME,
	END,
};

enum class DS_TYPE
{
	LESS,
	LESS_EQUAL,

	GREATER,

	NO_TEST,
	NO_WRITE,
	NO_TEST_NO_WRITE,

	END,
};

enum class BS_TYPE
{
	DEFAULT,
	ALPHABLEND_COVERAGE,
	ALPHABLEND,
	ONE_ONE,
	DECAL,
	END,
};

enum SCALAR_PARAM
{
	INT_0, INT_1, INT_2, INT_3,
	FLOAT_0, FLOAT_1, FLOAT_2, FLOAT_3,
	VEC2_0, VEC2_1, VEC2_2, VEC2_3,
	VEC4_0, VEC4_1, VEC4_2, VEC4_3,
	MAT_0, MAT_1, MAT_2, MAT_3,
};

enum TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,

	TEXCUBE_0,
	TEXCUBE_1,

	TEXARR_0,
	TEXARR_1,

	END,
};


enum SHADER_DOMAIN
{
	DOMAIN_DEFERRED,
	DOMAIN_LIGHT,
	DOMAIN_DECAL,
	DOMAIN_SHADOWMAP,

	DOMAIN_OPAQUE,
	DOMAIN_MASKED,
	DOMAIN_TRANSPARENT,
	DOMAIN_PARTICLE,
	DOMAIN_EFFECT,
	DOMAIN_POSTPROCESS,
	DOMAIN_UI,
	DOMAIN_DEBUG,
	DOMAIN_NONE,
};

enum class DEBUG_SHAPE
{
	RECT,
	CIRCLE,
	LINE,
	CUBE,
	SPHERE,
	CONE,
};

enum TASK_TYPE
{
	// Param_0 : Layer, Param_1 : Object Address
	CREATE_OBJECT,

	// Param_0 : Delete Object Address
	DELETE_OBJECT,
	CHANGE_LEVELSTATE,
	CHANGE_LEVEL,
	ASSET_SETDIRTY,
	LEVEL_SETDIRTY,
	ADD_CHILD,
	COMPONENT_ACTIVATE,
	COMPONENT_DEACTIVATE,
	DEL_ASSET,
};

enum LEVEL_STATE
{
	PLAY,
	PAUSE,
	STOP,
};

enum class LIGHT_TYPE
{
	DIRECTIONAL,
	POINT,
	SPOT,
};

enum class PARTICLE_MODULE
{
	SPAWN,
	SPAWN_BURST,
	ADD_VELOCITY,
	SCALE,
	DRAG,
	NOISE_FORCE,
	RENDER,
	ORBIT,

	END,
};

enum class MRT_TYPE
{
	SWAPCHAIN,
	EFFECT,
	EFFECT_BLUR,
	DEFERRED,
	LIGHT,
	DECAL,

	END,
};

enum class DT_OPTIONS
{
	SCALE,
	NONESCALE,
};