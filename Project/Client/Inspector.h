#pragma once
#include "EditorUI.h"
#include "Engine/Ptr.h"
#include "Engine/assets.h"
class Inspector :
    public EditorUI
{
public:
    Inspector();
    ~Inspector();

public:
    void SetTargetObject(CGameObject* _Object);
    void SetTargetAsset(Ptr<CAsset> _Asset);

    CGameObject* GetTargetObject() { return m_TargetObject; }
public:
    virtual void Init() override;
    virtual void Update() override;

private:
    void CreateComponentUI();
    void CreateAssetUI();

private:
    class CGameObject* m_TargetObject;
    class ComponentUI* m_arrComUI[(UINT)COMPONENT_TYPE::END];

    Ptr<CAsset>         m_TargetAsset;
    Ptr<CTexture>       m_IconTexture;
    class AssetUI* m_arrAssetUI[(UINT)ASSET_TYPE::END];
};