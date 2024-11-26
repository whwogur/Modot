#include "pch.h"
#include "CMesh.h"

#include "CDevice.h"
#include "CPathMgr.h"

CMesh::CMesh()
	: CAsset(ASSET_TYPE::MESH)
	, m_VtxCount(0)
	, m_VtxSysMem(nullptr)
{
}

CMesh::~CMesh()
{
	if (nullptr != m_VtxSysMem)
		delete m_VtxSysMem;

	for (size_t i = 0; i < m_vecIdxInfo.size(); ++i)
	{
		if (nullptr != m_vecIdxInfo[i].pIdxSysMem)
			delete m_vecIdxInfo[i].pIdxSysMem;
	}
}

CMesh* CMesh::CreateFromContainer(CFBXLoader& _loader)
{
	const tContainer* container = &_loader.GetContainer(0);
	UINT iVtxCount = (UINT)container->vecPos.size();
	D3D11_BUFFER_DESC tVtxDesc = {};
	tVtxDesc.ByteWidth = sizeof(Vtx) * iVtxCount;
	tVtxDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tVtxDesc.Usage = D3D11_USAGE_DEFAULT;
	if (D3D11_USAGE_DYNAMIC == tVtxDesc.Usage)
		tVtxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem = malloc(tVtxDesc.ByteWidth);
	Vtx* pSys = (Vtx*)tSub.pSysMem;
	for (UINT i = 0; i < iVtxCount; ++i)
	{
		pSys[i].vPos = container->vecPos[i];
		pSys[i].vUV = container->vecUV[i];
		pSys[i].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
		pSys[i].vNormal = container->vecNormal[i];
		pSys[i].vTangent = container->vecTangent[i];
		pSys[i].vBinormal = container->vecBinormal[i];
		//pSys[i].vWeights = container->vecWeights[i];
		//pSys[i].vIndices = container->vecIndices[i];
	}
	WRL::ComPtr<ID3D11Buffer> pVB = NULL;
	if (FAILED(DEVICE->CreateBuffer(&tVtxDesc, &tSub, pVB.GetAddressOf())))
	{
		return NULL;
	}
	CMesh* pMesh = new CMesh;
	pMesh->m_VB = pVB;
	pMesh->m_VBDesc = tVtxDesc;
	pMesh->m_VtxSysMem = pSys;
	// 인덱스 정보
	UINT iIdxBufferCount = (UINT)container->vecIdx.size();
	D3D11_BUFFER_DESC tIdxDesc = {};
	for (UINT i = 0; i < iIdxBufferCount; ++i)
	{
		tIdxDesc.ByteWidth = (UINT)container->vecIdx[i].size() * sizeof(UINT); // Index Format 이 R32_UINT 이기 때문
		tIdxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		tIdxDesc.Usage = D3D11_USAGE_DEFAULT;
		if (D3D11_USAGE_DYNAMIC == tIdxDesc.Usage)
			tIdxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		void* pSysMem = malloc(tIdxDesc.ByteWidth);
		memcpy(pSysMem, container->vecIdx[i].data(), tIdxDesc.ByteWidth);
		tSub.pSysMem = pSysMem;
		WRL::ComPtr<ID3D11Buffer> pIB = nullptr;
		if (FAILED(DEVICE->CreateBuffer(&tIdxDesc, &tSub, pIB.GetAddressOf())))
		{
			return NULL;
		}
		tIndexInfo info = {};
		info.tIBDesc = tIdxDesc;
		info.iIdxCount = (UINT)container->vecIdx[i].size();
		info.pIdxSysMem = pSysMem;
		info.pIB = pIB;
		pMesh->m_vecIdxInfo.push_back(info);
	}
	return pMesh;
}

int CMesh::Create(Vtx* _VtxSysMem, UINT _VtxCount, UINT* _IdxSysMem, UINT _IdxCount)
{
	// 버텍스 개수
	m_VtxCount = (UINT)_VtxCount;

	// 버텍스 시스템 메모리
	m_VtxSysMem = new Vtx[m_VtxCount];
	memcpy(m_VtxSysMem, _VtxSysMem, sizeof(Vtx) * m_VtxCount);

	// 버텍스 버퍼 생성
	m_VBDesc.ByteWidth = sizeof(Vtx) * m_VtxCount;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// Vertex 버퍼가 생성된 이후에도 데이터 쓰기가 가능하도록 설정
	m_VBDesc.Usage = D3D11_USAGE_DEFAULT;
	m_VBDesc.CPUAccessFlags = 0;

	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem = _VtxSysMem;

	if (FAILED(DEVICE->CreateBuffer(&m_VBDesc, &tSub, m_VB.GetAddressOf())))
	{
		MessageBox(nullptr, L"VertexBuffer 생성 실패", L"Mesh 생성 실패", MB_OK);
		return E_FAIL;
	}

	// 인덱스 버퍼 생성
	tIndexInfo IndexInfo = {};
	IndexInfo.iIdxCount = _IdxCount;

	IndexInfo.tIBDesc.ByteWidth = sizeof(UINT) * _IdxCount;

	// 버퍼 생성 이후에도, 버퍼의 내용을 수정 할 수 있는 옵션
	IndexInfo.tIBDesc.CPUAccessFlags = 0;
	IndexInfo.tIBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

	// 정점을 저장하는 목적의 버퍼 임을 알림
	IndexInfo.tIBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	IndexInfo.tIBDesc.MiscFlags = 0;
	IndexInfo.tIBDesc.StructureByteStride = 0;

	// 초기 데이터를 넘겨주기 위한 정보 구조체	
	tSub.pSysMem = _IdxSysMem;

	if (FAILED(DEVICE->CreateBuffer(&IndexInfo.tIBDesc, &tSub, IndexInfo.pIB.GetAddressOf())))
	{
		assert(nullptr);
	}

	// 시스템 메모리에 저장
	IndexInfo.pIdxSysMem = new UINT[IndexInfo.iIdxCount];
	memcpy(IndexInfo.pIdxSysMem, _IdxSysMem, sizeof(UINT) * IndexInfo.iIdxCount);

	m_vecIdxInfo.push_back(IndexInfo);

	return S_OK;
}

void CMesh::Bind(UINT _Subset)
{
	UINT stride = sizeof(Vtx);
	UINT offset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &stride, &offset);
	CONTEXT->IASetIndexBuffer(m_vecIdxInfo[_Subset].pIB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CMesh::Render(UINT _Subset)
{
	Bind(_Subset);

	CONTEXT->DrawIndexed(m_vecIdxInfo[_Subset].iIdxCount, 0, 0);
}


void CMesh::Render_Particle(UINT _Count)
{
	Bind(0);

	CONTEXT->DrawIndexedInstanced(m_vecIdxInfo[0].iIdxCount, _Count, 0, 0, 0);
}

int CMesh::Save(const wstring& _RelativePath)
{
	SetRelativePath(_RelativePath);
	wstring fullPath = CPathMgr::GetInst()->GetContentPath() + _RelativePath;
	// 파일 쓰기모드로 열기
	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, fullPath.c_str(), L"wb");
	assert(pFile);

	// 키값, 상대 경로	
	SaveWString(GetName(), pFile);
	SaveWString(GetKey(), pFile);
	SaveWString(GetRelativePath(), pFile);

	// 정점 데이터 저장				
	int iByteSize = m_VBDesc.ByteWidth;
	fwrite(&iByteSize, sizeof(int), 1, pFile);
	fwrite(m_VtxSysMem, iByteSize, 1, pFile);

	// 인덱스 정보
	UINT iMtrlCount = (UINT)m_vecIdxInfo.size();
	fwrite(&iMtrlCount, sizeof(int), 1, pFile);
	UINT iIdxBuffSize = 0;
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		fwrite(&m_vecIdxInfo[i], sizeof(tIndexInfo), 1, pFile);
		fwrite(m_vecIdxInfo[i].pIdxSysMem
			, m_vecIdxInfo[i].iIdxCount * sizeof(UINT)
			, 1, pFile);
	}
	fclose(pFile);
	return S_OK;
}

int CMesh::Load(const wstring& _RelativePath)
{
	SetRelativePath(_RelativePath);
	wstring fullPath = CPathMgr::GetInst()->GetContentPath() + _RelativePath;

	// 읽기모드로 파일열기
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, fullPath.c_str(), L"rb");
	// 키값, 상대경로
	wstring strName, strKey, strRelativePath = _RelativePath;
	LoadWString(strName, pFile);
	LoadWString(strKey, pFile);
	LoadWString(strRelativePath, pFile);
	SetName(strName);
	SetKey(strKey);
	SetRelativePath(_RelativePath);
	// 정점데이터
	UINT iByteSize = 0;
	fread(&iByteSize, sizeof(int), 1, pFile);
	m_VtxSysMem = (Vtx*)malloc(iByteSize);
	fread(m_VtxSysMem, 1, iByteSize, pFile);
	D3D11_BUFFER_DESC tDesc = {};
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.ByteWidth = iByteSize;
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	D3D11_SUBRESOURCE_DATA tSubData = {};
	tSubData.pSysMem = m_VtxSysMem;
	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tSubData, m_VB.GetAddressOf())))
	{
		assert(nullptr);
	}
	// 인덱스 정보
	UINT iMtrlCount = 0;
	fread(&iMtrlCount, sizeof(int), 1, pFile);
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		tIndexInfo info = {};
		fread(&info, sizeof(tIndexInfo), 1, pFile);
		UINT iByteWidth = info.iIdxCount * sizeof(UINT);
		void* pSysMem = malloc(iByteWidth);
		info.pIdxSysMem = pSysMem;
		fread(info.pIdxSysMem, iByteWidth, 1, pFile);
		tSubData.pSysMem = info.pIdxSysMem;
		if (FAILED(DEVICE->CreateBuffer(&info.tIBDesc, &tSubData, info.pIB.GetAddressOf())))
		{
			assert(nullptr);
		}
		m_vecIdxInfo.push_back(info);
	}
	fclose(pFile);
	return S_OK;
}