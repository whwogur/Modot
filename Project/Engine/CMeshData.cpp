#include "pch.h"
#include "CMeshData.h"
#include "CPathMgr.h"
#include "CAssetMgr.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CFBXLoader.h"
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
	for (UINT i = 0; i < m_vecMtrl.size(); ++i)
	{
		pNewObj->MeshRender()->SetMaterial(m_vecMtrl[i], i);
	}
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
		wstring strMeshKey = L"mesh\\";
		strMeshKey += path(strFullPath).stem();
		strMeshKey += L".mesh";
		CAssetMgr::GetInst()->AddAsset<CMesh>(strMeshKey, pMesh);
		// �޽ø� ���� ���Ϸ� ����
		pMesh->Save(strMeshKey);
	}
	std::vector<Ptr<CMaterial>> vecMtrl;
	// ���׸��� ��������
	for (UINT i = 0; i < loader.GetContainer(0).vecMtrl.size(); ++i)
	{
		// ����ó�� (material �̸��� �Է� �ȵǾ����� ���� �ִ�.)
		Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(loader.GetContainer(0).vecMtrl[i].strMtrlName);
		assert(pMtrl.Get());
		vecMtrl.push_back(pMtrl);
	}
	CMeshData* pMeshData = new CMeshData(true);
	pMeshData->m_pMesh = pMesh;
	pMeshData->m_vecMtrl = vecMtrl;
	return pMeshData;
}
int CMeshData::Save(const wstring& _FilePath)
{
	return 0;
}
int CMeshData::Load(const wstring& _FilePath)
{
	return 0;
}