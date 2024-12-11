#include "pch.h"
#include "CCopyBoneCS.h"
#include "CStructuredBuffer.h"

CCopyBoneCS::CCopyBoneCS()
	: CComputeShader(256, 1, 1, L"shader\\copybone.fx", "CS_CopyBoneMatrix")
{
}

int CCopyBoneCS::Bind()
{
	// ����ȭ���� ����
	m_SrcBuffer.lock()->Bind_CS_SRV(18);  // t18
	m_DestBuffer.lock()->Bind_CS_UAV(0);  // u0

	return S_OK;
}

void CCopyBoneCS::CalcGroupNum()
{
	// �׷� �� ���
	int iBoneCount = m_Const.iArr[0];

	m_GroupX = iBoneCount / m_ThreadPerGroupX + 1;
	m_GroupY = 1;
	m_GroupZ = 1;
}

void CCopyBoneCS::Clear()
{
	// ������ ����ȭ���� Ŭ����	
	m_SrcBuffer.lock()->Clear_CS_SRV();
	m_DestBuffer.lock()->Clear_CS_UAV();

	m_SrcBuffer = std::weak_ptr<CStructuredBuffer>();
	m_DestBuffer = std::weak_ptr<CStructuredBuffer>();
}
