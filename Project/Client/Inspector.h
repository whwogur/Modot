#pragma once
#include "EditorUI.h"
#include "Engine/Ptr.h"
#include "Engine/assets.h"
class CGameObject;
class ComponentUI;
class AssetUI;

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
    Ptr<CAsset> GetTargetAsset() { return m_TargetAsset; }
public:
    virtual void Init() override;
    virtual void Update() override;

private:
    void CreateComponentUI();
    void CreateAssetUI();
    void CreateScriptUI(UINT _Count);
    void Purge();

private:
    CGameObject*                m_TargetObject;
    ComponentUI*                m_arrComUI[(UINT)COMPONENT_TYPE::END];
    std::vector<class ScriptUI*>     m_vecScriptUI;
    Ptr<CAsset>                 m_TargetAsset;
    Ptr<CTexture>               m_IconTexture;
    AssetUI*                    m_arrAssetUI[(UINT)ASSET_TYPE::END];
    char                        m_Namebuffer[255] = {};
};