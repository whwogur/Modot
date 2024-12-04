#pragma once
#include "EditorUI.h"

class TexturePreview
	: public EditorUI
{
public:
	TexturePreview() = default;
	~TexturePreview() = default;

public:
	virtual void Update() override;
	void ChangeTexture();
	
private:
	Ptr<CTexture> m_Tex1 = nullptr;
	Ptr<CTexture> m_Tex2 = nullptr;
	Ptr<CTexture> m_Tex3 = nullptr;
	Ptr<CTexture> m_Tex4 = nullptr;

	Ptr<CTexture>* m_SelectedContext = nullptr;
	float			m_TexSizeX = 250.f;
	float			m_TexSizeY = 170.f;
};

