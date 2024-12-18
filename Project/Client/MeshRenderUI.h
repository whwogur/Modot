#pragma once
#include "ComponentUI.h"
class CMeshRender;
class MeshRenderUI :
    public ComponentUI
{
public:
    MeshRenderUI();
    ~MeshRenderUI() = default;

public:
    void SelectMesh(DWORD_PTR _AssetName);

private:
    virtual void Update() override;
    void Mesh(CMeshRender*);
    void Material(CMeshRender*);
    void MaterialSetInfo(struct tMtrlSet*);
    void MaterialInfo(Ptr<CMaterial>);
};