#pragma once
#include "CComponent.h"


class CRenderComponent :
    public CComponent
{
public:
    CRenderComponent(COMPONENT_TYPE _Type);
    ~CRenderComponent();
public:
    void SetMesh(CMesh* _Mesh) { m_Mesh = _Mesh; }
    void SetShader(CGraphicShader* _Shader) { m_Shader = _Shader; }

    CMesh* GetMesh() { return m_Mesh; }
    CGraphicShader* GetShader() { return m_Shader; }


public:
    virtual void FinalTick() = 0;
    virtual void Render() = 0;
    virtual CRenderComponent* Clone() = 0;


private:
    CMesh* m_Mesh;
    CGraphicShader* m_Shader;
};