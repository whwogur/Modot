#pragma once
#include "SE_Sub.h"
class SE_Detail :
    public SE_Sub
{
public:
    SE_Detail();
    ~SE_Detail();

public:
    void SetAtlasTex(Ptr<CTexture> _Tex);
public:
    virtual void Init() override;
    virtual void Update() override;

private:
    void Atlas();
    void AtlasInfo();
    void SelectTexture(DWORD_PTR _ListUI);
private:
    Ptr<CTexture>   m_AtlasTex;
};