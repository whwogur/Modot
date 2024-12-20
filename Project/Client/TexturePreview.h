#pragma once
#include "CTool.h"

class TexturePreview
	: public CTool
{
public:
	TexturePreview() = default;
	~TexturePreview() = default;

public:
	virtual void Update() override;
	virtual void Init() override {};
	void ChangeTexture(DWORD_PTR _ListUI);
	
private:
	Ptr<CTexture> m_Tex1 = nullptr;
	Ptr<CTexture> m_Tex2 = nullptr;
	Ptr<CTexture> m_Tex3 = nullptr;
	Ptr<CTexture> m_Tex4 = nullptr;

	Ptr<CTexture>* m_SelectedContext = nullptr;
	float			m_TexSizeX = 250.f;
	float			m_TexSizeY = 170.f;
};

