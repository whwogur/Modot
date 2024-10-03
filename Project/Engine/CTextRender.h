#pragma once
#include "CRenderComponent.h"
class CTextRender :
    public CRenderComponent
{
public:
    CLONE(CTextRender);
    CTextRender();
    ~CTextRender() = default;

public:
    virtual void FinalTick() override;
    virtual void Render() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    const wstring& GetText() const { return m_Text; }
    void SetText(const wstring& _Text) { m_Text = _Text; }

    float GetTextSize() const { return m_Size; }
    void SetTextSize(float _size) { m_Size = _size; }

    Vec4 GetTextColor() const { return m_Color; }
    void SetTextColor(Vec4 _color) { m_Color = _color; }

private:
    wstring     m_Text = {};
    float       m_Size = 20.f;
    Vec4        m_Color = Vec4(255.f, 255.f, 255.f, 255.f);
};

