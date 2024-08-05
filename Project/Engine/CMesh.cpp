#include "pch.h"
#include "CMesh.h"

#include "CDevice.h"

CMesh::CMesh()
	: CAsset(ASSET_TYPE::MESH)
	, m_VtxCount(0)
	, m_IdxCount(0)
	, m_VBDesc{}
	, m_IBDesc{}
	, m_VtxSysMem(nullptr)
	, m_IdxSysMem(nullptr)
{
}

CMesh::~CMesh()
{
	if (nullptr != m_VtxSysMem)
		delete m_VtxSysMem;

	if (nullptr != m_IdxSysMem)
		delete m_IdxSysMem;
}

int CMesh::Create(Vtx* _VtxSysMem, UINT _VtxCount, UINT* _IdxSysMem, UINT _IdxCount)
{
	m_VtxCount = _VtxCount;
	m_IdxCount = _IdxCount;
		
	m_VtxSysMem = new Vtx[m_VtxCount];
	memcpy(m_VtxSysMem, _VtxSysMem, sizeof(Vtx) * m_VtxCount);

	m_IdxSysMem = new UINT[m_IdxCount];
	memcpy(m_IdxSysMem, _IdxSysMem, sizeof(UINT) * m_IdxCount);


	m_VBDesc.ByteWidth = sizeof(Vtx) * m_VtxCount;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// Vertex ���۰� ������ ���Ŀ��� ������ ���Ⱑ �����ϵ��� ����
	m_VBDesc.Usage = D3D11_USAGE_DEFAULT;
	m_VBDesc.CPUAccessFlags = 0;

	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem = _VtxSysMem;

	MD_ENGINE_ASSERT(SUCCEEDED(DEVICE->CreateBuffer(&m_VBDesc, &tSub, m_VB.GetAddressOf())), L"Mesh ���� ���� - VertexBuffer ���� ����");

	m_IBDesc.ByteWidth = sizeof(UINT) * m_IdxCount;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// Index ���۰� ������ ���Ŀ� �����Ͱ� ����� ���� ����.
	m_IBDesc.Usage = D3D11_USAGE_DEFAULT;
	m_IBDesc.CPUAccessFlags = 0;

	m_IBDesc.MiscFlags = 0;
	m_IBDesc.StructureByteStride = 0;

	tSub.pSysMem = _IdxSysMem;

	MD_ENGINE_ASSERT(SUCCEEDED(DEVICE->CreateBuffer(&m_IBDesc, &tSub, m_IB.GetAddressOf())), L"Mesh ���� ���� - IndexBuffer ���� ����");

	return S_OK;
}

void CMesh::Bind()
{
	UINT stride = sizeof(Vtx);
	UINT offset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &stride, &offset);
	CONTEXT->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CMesh::Render()
{
	Bind();

	CONTEXT->DrawIndexed(m_IdxCount, 0, 0);
}


void CMesh::Render_Particle(UINT _Count)
{
	Bind();

	CONTEXT->DrawIndexedInstanced(m_IdxCount, _Count, 0, 0, 0);
}
