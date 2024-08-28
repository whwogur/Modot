#include "spch.h"
#include "CTintedBGScript.h"

CTintedBGScript::CTintedBGScript()
	: CScript(SCRIPT_TYPE::TINTEDBGSCRIPT)
{
	AddScriptParam(SCRIPT_PARAM::TEXTURE, u8"텍스처", &m_Texture);
	AddScriptParam(SCRIPT_PARAM::COLOR, u8"틴트", &m_Tint);
	AddScriptParam(SCRIPT_PARAM::BOOLEAN_TOGGLE, u8"움직임", &m_Mobile);
}

void CTintedBGScript::Begin()
{
	MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_Texture);
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, m_Tint);
}

void CTintedBGScript::Tick()
{
	if (m_Mobile)
	{
		if (KEY_PRESSED(KEY::LEFT))
		{
			const Vec3& relPos = Transform()->GetRelativePosRef();
			Transform()->SetRelativePos(Vec3(relPos.x + relPos.z * 3 * DT, relPos.y, relPos.z));
		}

		if (KEY_PRESSED(KEY::RIGHT))
		{
			const Vec3& relPos = Transform()->GetRelativePosRef();
			Transform()->SetRelativePos(Vec3(relPos.x - relPos.z * 3 * DT, relPos.y, relPos.z));
		}
	}
}

void CTintedBGScript::SaveToFile(FILE* _File)
{
	SaveAssetRef(m_Texture, _File);
	fwrite(&m_Tint, sizeof(Vec4), 1, _File);
	fwrite(&m_Mobile, sizeof(bool), 1, _File);
}

void CTintedBGScript::LoadFromFile(FILE* _File)
{
	LoadAssetRef(m_Texture, _File);
	fread(&m_Tint, sizeof(Vec4), 1, _File);
	fread(&m_Mobile, sizeof(bool), 1, _File);
}
