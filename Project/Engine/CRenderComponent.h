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

    // 절두체 체크를 받을 것인지
    void SetFrustumCheck(bool _Check) { m_FrustumCheck = _Check; }
    // 절두체 체크 받는지 확인
    bool ChecksFrustum() const { return m_FrustumCheck; }
public:
    virtual void FinalTick() = 0;
    virtual void Render() = 0;

protected:
    void SaveDataToFile(FILE* _File);
    void LoadDataFromFile(FILE* _File);
    
private:
    Ptr<CMesh>          m_Mesh;

    Ptr<CMaterial>      m_Mtrl;         // 현재 사용중인 재질
    Ptr<CMaterial>      m_SharedMtrl;   // 공유 재질(마스터)
    Ptr<CMaterial>      m_DynamicMtrl;  // 임시 재질
    bool                m_FrustumCheck; // 절두체 체크를 받을것인지 말것인지
};