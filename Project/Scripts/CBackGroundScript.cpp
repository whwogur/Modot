#include "spch.h"
#include "CBackGroundScript.h"

CBackGroundScript::CBackGroundScript()
	: CScript(SCRIPT_TYPE::BACKGROUNDSCRIPT)
{
	AddScriptParam(SCRIPT_PARAM::TEXTURE, u8"텍스처", &m_Texture);
	AddScriptParam(SCRIPT_PARAM::BOOLEAN_TOGGLE, u8"움직임", &m_Mobile);
}

void CBackGroundScript::Begin()
{
	MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_Texture);
}

void CBackGroundScript::Tick()
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

void CBackGroundScript::SaveToFile(FILE* _File)
{
	SaveAssetRef(m_Texture, _File);
	fwrite(&m_Mobile, sizeof(bool), 1, _File);
}

void CBackGroundScript::LoadFromFile(FILE* _File)
{
	LoadAssetRef(m_Texture, _File);
	fread(&m_Mobile, sizeof(bool), 1, _File);
}
