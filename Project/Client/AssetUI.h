#pragma once
#include "EditorUI.h"

class AssetUI :
    public EditorUI
{
public:
    AssetUI(ASSET_TYPE _Type);
    ~AssetUI();
public:
    void SetAsset(Ptr<CAsset> _Asset);
    Ptr<CAsset> GetAsset() { return m_Asset; }

protected:
    void Title();
    void OutputAssetName();
private:
    Ptr<CAsset>         m_Asset;
    const ASSET_TYPE    m_Type;
};