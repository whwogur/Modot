#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"
// 모험일지 , 메모, 옵션
enum class MENU_TYPE
{
    ADLOG,
    MEMO,
    OPTIONS,
    END,
};

enum class MEMO
{
    STATDESC,
    END,
};

enum class OPTIONS
{
    NONE,
    CAMERASHAKE,
    GENERALVOLUME,
    EFFECTVOLUME,
    BGMVOLUME,
    SAVECHANGES,
    MAIN,
    END,
};

class CMenuScript :
    public CScript
{
public:
    CMenuScript();
    ~CMenuScript() = default;
    CLONE(CMenuScript)
public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    void Activate();
    void Deactivate();
    void Reset();
private:
    Ptr<CSound>         m_TickSound = nullptr;
    Ptr<CSound>         m_CancelSound = nullptr;

    Ptr<CTexture>       m_AdlogTex;
    Ptr<CTexture>       m_MemoTex;
    Ptr<CTexture>       m_OptionsTex;

    MENU_TYPE           m_CurType = MENU_TYPE::ADLOG;
    
    CGameObject*        m_MenuKnob;
    Vec3                m_MenuKnobPos[(UINT)MENU_TYPE::END] = {Vec3(-589.5f, 492.f, 2.f), Vec3(-78.f, 492.f, 2.f), Vec3(550.5f, 492.f, 2.f)};

    CGameObject*        m_ItemKnob;
    int                 m_ItemIdx = 0;
    bool                m_ItemSelected = false;
    bool                m_Active = false;
};

