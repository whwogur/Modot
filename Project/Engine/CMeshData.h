#pragma once
#include "CAsset.h"
#include "CMesh.h"
#include "CMaterial.h"
class CMeshData :
	public CAsset
{
public:
	CLONE_DISABLE(CMeshData);
	CMeshData(bool _Engine = false);
	~CMeshData() = default;

public:
	static CMeshData* LoadFromFBX(const wstring& _RelativePath);
	CGameObject* Instantiate();

	virtual int Save(const wstring& _RelativePath) override;
	virtual int Load(const wstring& _RelativePath) override;

private:
	Ptr<CMesh>					m_pMesh;
	std::vector<Ptr<CMaterial>>	m_vecMtrl;
};