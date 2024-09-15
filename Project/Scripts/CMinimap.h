#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"
enum class MapType
{
    KOHOVILL,
    KOHOSHRINE,
    DEMON,
    LINETH,

    END,
};

class CMinimap :
    public CScript
{
public:
    CMinimap();
    ~CMinimap() = default;
    CLONE(CMinimap);
public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    Ptr<CSound>     m_BGM;
    Ptr<CTexture>   m_MapTileTex;

    CGameObject*    m_Player = nullptr;

    CGameObject*    m_PlayerIcon = nullptr;

    float           m_Acc = 0.f;
    float           m_Timer = 2.f;

    MapType         m_Type = MapType::END;
};

