#include "pch.h"
#include "CMeshData.h"
#include "CPathMgr.h"
#include "CAssetMgr.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CFBXLoader.h"
#include "CAnimator3D.h"

CMeshData::CMeshData(bool _Engine)
	: CAsset(ASSET_TYPE::MESH_DATA)
{
	if (_Engine)
		SetEngineAsset();
}

CGameObject* CMeshData::Instantiate()
{
	CGameObject* pNewObj = new CGameObject;
	pNewObj->AddComponent(new CTransform);
	pNewObj->AddComponent(new CMeshRender);
	pNewObj->MeshRender()->SetMesh(m_pMesh);

	MD_ENGINE_WARN(L"�޽� ������ {0} �ν��Ͻ�ȭ", GetKey());
	for (UINT i = 0; i < m_vecMtrl.size(); ++i)
	{
		MD_ENGINE_TRACE(L"=== #{0}: {1} ===", std::to_wstring(i), m_vecMtrl[i]->GetKey());
		MD_ENGINE_TRACE(L"���̴� {0}", m_vecMtrl[i]->GetShader()->GetKey());
		
		pNewObj->MeshRender()->SetMaterial(m_vecMtrl[i], i);
	}

	// Animation
	if (!m_pMesh->IsAnimMesh())
		return pNewObj;

	CAnimator3D* pAnimator = new CAnimator3D;
	pNewObj->AddComponent(pAnimator);

	pAnimator->SetBones(m_pMesh->GetBones());
	pAnimator->SetAnimClip(m_pMesh->GetAnimClip());

	return pNewObj;
}

CMeshData* CMeshData::LoadFromFBX(const wstring& _RelativePath)
{
	wstring strFullPath = CPathMgr::GetInst()->GetContentPath();
	strFullPath += _RelativePath;

	CFBXLoader loader;
	loader.init();
	loader.LoadFbx(strFullPath);

	// �޽� ��������
	CMesh* pMesh = nullptr;
	pMesh = CMesh::CreateFromContainer(loader);
	// AssetMgr �� �޽� ���
	if (nullptr != pMesh)
	{
		wstring strMeshKey = path(strFullPath).stem();
		strMeshKey += L"Mesh";
		pMesh->SetRelativePath(L"mesh\\" + strMeshKey + L".mesh");
		pMesh->SetKey(strMeshKey);

		if (!CAssetMgr::GetInst()->FindAsset<CMesh>(strMeshKey))
		{
			// �޽ø� ���� ���Ϸ� ����
			CAssetMgr::GetInst()->AddAsset<CMesh>(strMeshKey, pMesh);
			pMesh->Save(pMesh->GetRelativePath());
		}
	}
	std::vector<Ptr<CMaterial>> vecMtrl;
	// ���׸��� ��������
	for (UINT i = 0; i < loader.GetContainer(0).vecMtrl.size(); ++i)
	{
		// ����ó�� (material �̸��� �Է� �ȵǾ����� ���� �ִ�.)
		Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(loader.GetContainer(0).vecMtrl[i].strMtrlName);
		assert(pMtrl.Get());
		vecMtrl.emplace_back(pMtrl);
	}

	CMeshData* pMeshData = new CMeshData(true);
	pMeshData->m_pMesh = pMesh;
	pMeshData->m_vecMtrl = vecMtrl;

	return pMeshData;
}

int CMeshData::Save(const wstring& _RelativePath)
{
	SetRelativePath(_RelativePath);
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + _RelativePath;
	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	assert(pFile);

	MD_ENGINE_INFO(L"�޽� ������ ����");
	// Mesh Key ����	
	// Mesh Data ����
	SaveAssetRef(m_pMesh, pFile);
	// material ���� ����
	UINT iMtrlCount = (UINT)m_vecMtrl.size();
	fwrite(&iMtrlCount, sizeof(UINT), 1, pFile);
	UINT i = 0;
	for (; i < iMtrlCount; ++i)
	{
		if (nullptr == m_vecMtrl[i])
			continue;
		// Material �ε���, Key, Path ����
		fwrite(&i, sizeof(UINT), 1, pFile);
		SaveAssetRef(m_vecMtrl[i], pFile);
		MD_ENGINE_TRACE(m_vecMtrl[i]->GetKey());
		if (m_vecMtrl[i]->GetTexParam(TEX_PARAM::TEX_0) != nullptr)
			MD_ENGINE_TRACE(L"Tex0 - ", m_vecMtrl[i]->GetTexParam(TEX_PARAM::TEX_0)->GetKey());
		if (m_vecMtrl[i]->GetTexParam(TEX_PARAM::TEX_1) != nullptr)
			MD_ENGINE_TRACE(L"Tex1 - ", m_vecMtrl[i]->GetTexParam(TEX_PARAM::TEX_1)->GetKey());
		if (m_vecMtrl[i]->GetTexParam(TEX_PARAM::TEX_2) != nullptr)
			MD_ENGINE_TRACE(L"Tex2 - ", m_vecMtrl[i]->GetTexParam(TEX_PARAM::TEX_2)->GetKey());
		if (m_vecMtrl[i]->GetTexParam(TEX_PARAM::TEX_3) != nullptr)
			MD_ENGINE_TRACE(L"Tex3 - ", m_vecMtrl[i]->GetTexParam(TEX_PARAM::TEX_3)->GetKey());
	}
	i = -1; // ���� ��
	fwrite(&i, sizeof(UINT), 1, pFile);
	fclose(pFile);
	return S_OK;
}

int CMeshData::Load(const wstring& _RelativePath)
{
	wstring fullPath = CPathMgr::GetInst()->GetContentPath() + _RelativePath;
	FILE* pFile = NULL;

	_wfopen_s(&pFile, fullPath.c_str(), L"rb");
	assert(pFile);

	// Mesh Load
	LoadAssetRef(m_pMesh, pFile);
	assert(m_pMesh.Get());

	// material ���� �б�
	UINT iMtrlCount = 0;
	fread(&iMtrlCount, sizeof(UINT), 1, pFile);
	m_vecMtrl.resize(iMtrlCount);
	MD_ENGINE_INFO(L"�޽� ������ �ε�");
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		UINT idx = -1;
		fread(&idx, sizeof(UINT), 1, pFile);
		if (idx == -1)
			break;

		Ptr<CMaterial> pMtrl;
		LoadAssetRef(pMtrl, pFile);
		MD_ENGINE_TRACE(pMtrl->GetKey());
		if (pMtrl->GetTexParam(TEX_PARAM::TEX_0) != nullptr)
			MD_ENGINE_TRACE(L"Tex0 - {0}",pMtrl->GetTexParam(TEX_PARAM::TEX_0)->GetKey());
		if (pMtrl->GetTexParam(TEX_PARAM::TEX_1) != nullptr)
			MD_ENGINE_TRACE(L"Tex1 - {0}", pMtrl->GetTexParam(TEX_PARAM::TEX_1)->GetKey());
		if (pMtrl->GetTexParam(TEX_PARAM::TEX_2) != nullptr)
			MD_ENGINE_TRACE(L"Tex2 - {0}", pMtrl->GetTexParam(TEX_PARAM::TEX_2)->GetKey());
		if (pMtrl->GetTexParam(TEX_PARAM::TEX_3) != nullptr)
			MD_ENGINE_TRACE(L"Tex3 - {0}", pMtrl->GetTexParam(TEX_PARAM::TEX_3)->GetKey());

		m_vecMtrl[i] = pMtrl;
	}
	fclose(pFile);
	return S_OK;
}