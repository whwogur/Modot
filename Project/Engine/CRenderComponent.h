#pragma once
#include "CComponent.h"

struct tMtrlSet
{
    Ptr<CMaterial>  pSharedMtrl;    // 공유 메테리얼
    Ptr<CMaterial>  pDynamicMtrl;   // 공유 메테리얼의 복사본    
    Ptr<CMaterial>  pCurMtrl;       // 현재 사용 할 메테리얼
};

class CRenderComponent :
    public CComponent
{
public:
    CRenderComponent(COMPONENT_TYPE _Type);
    CRenderComponent(const CRenderComponent& _Other);
    virtual ~CRenderComponent() = default;
public:
    void SetMesh(Ptr<CMesh> _Mesh);
    Ptr<CMesh> GetMesh() { return m_Mesh; }

    void SetMaterial(Ptr<CMaterial> _Mtrl, UINT _idx);
    Ptr<CMaterial> GetMaterial(UINT _idx);
    Ptr<CMaterial> GetSharedMaterial(UINT _idx);
    Ptr<CMaterial> GetDynamicMaterial(UINT _idx);
    UINT GetMaterialCount() { return (UINT)m_vecMtrls.size(); }
    // 절두체 체크를 받을 것인지
    void SetFrustumCheck(bool _Check) { m_FrustumCheck = _Check; }
    // 절두체 체크 받는지 확인
    bool ChecksFrustum() const { return m_FrustumCheck; }
public:
    virtual void FinalTick() = 0;
    virtual void Render() = 0;
    virtual void RenderShadow();
protected:
    void SaveDataToFile(FILE* _File);
    void LoadDataFromFile(FILE* _File);
    
private:
    Ptr<CMesh>              m_Mesh;
    std::vector<tMtrlSet>   m_vecMtrls;
    bool                    m_FrustumCheck; // 절두체 체크를 받을것인지 말것인지
};