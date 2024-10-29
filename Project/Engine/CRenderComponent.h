#pragma once
#include "CComponent.h"


class CRenderComponent :
    public CComponent
{
public:
    CRenderComponent(COMPONENT_TYPE _Type);
    CRenderComponent(const CRenderComponent& _Other);
    virtual ~CRenderComponent() = default;
public:
    void SetMesh(Ptr<CMesh> _Mesh) { m_Mesh = _Mesh; }
    void SetMaterial(Ptr<CMaterial> _Mtrl);

    Ptr<CMesh> GetMesh() { return m_Mesh; }
    Ptr<CMaterial> GetMaterial() { return m_Mtrl; }
    Ptr<CMaterial> GetSharedMtrl();

    Ptr<CMaterial> GetDynamicMaterial();

    // ����ü üũ�� ���� ������
    void SetFrustumCheck(bool _Check) { m_FrustumCheck = _Check; }
    // ����ü üũ �޴��� Ȯ��
    bool ChecksFrustum() const { return m_FrustumCheck; }
public:
    virtual void FinalTick() = 0;
    virtual void Render() = 0;

protected:
    void SaveDataToFile(FILE* _File);
    void LoadDataFromFile(FILE* _File);
    
private:
    Ptr<CMesh>          m_Mesh;

    Ptr<CMaterial>      m_Mtrl;         // ���� ������� ����
    Ptr<CMaterial>      m_SharedMtrl;   // ���� ����(������)
    Ptr<CMaterial>      m_DynamicMtrl;  // �ӽ� ����
    bool                m_FrustumCheck; // ����ü üũ�� ���������� ��������
};