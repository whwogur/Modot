#pragma once
#include "CAsset.h"

class CTexture :
    public CAsset
{
public:
    CTexture();
    ~CTexture();
public:
    virtual int Load(const wstring& _FilePath);
    virtual int Save(const wstring& _FilePath);
   
    int Create(UINT _Width, UINT _Height, DXGI_FORMAT _PixelFormat, UINT _Flags, D3D11_USAGE _Usage = D3D11_USAGE_DEFAULT);
    int Create(WRL::ComPtr<ID3D11Texture2D> _Tex2D);

    void Bind(UINT _RegisterNum);
    static void Clear(UINT _RegisterNum);
    
    void Bind_CS_SRV(UINT _RegisterNum);
    void Clear_CS_SRV();

    void Bind_CS_UAV(UINT _RegisterNum);
    void Clear_CS_UAV();
public:
    WRL::ComPtr<ID3D11Texture2D>             GetTex2D() { return m_Tex2D; }
    WRL::ComPtr<ID3D11RenderTargetView>      GetRTV() { return m_RTV; }
    WRL::ComPtr<ID3D11DepthStencilView>      GetDSV() { return m_DSV; }
    WRL::ComPtr<ID3D11ShaderResourceView>    GetSRV() { return m_SRV; }
    WRL::ComPtr<ID3D11UnorderedAccessView>   GetUAV() { return m_UAV; }
    const D3D11_TEXTURE2D_DESC& GetDesc() { return m_Desc; }
public:
    UINT Width() const { return m_Desc.Width; }
    UINT Height() const { return m_Desc.Height; }
    bool IsCubeMap() const { return m_Desc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE; }

private:
    WRL::ComPtr<ID3D11RenderTargetView>         m_RTV;
    WRL::ComPtr<ID3D11DepthStencilView>         m_DSV;
    WRL::ComPtr<ID3D11ShaderResourceView>       m_SRV;
    WRL::ComPtr<ID3D11UnorderedAccessView>      m_UAV;

    D3D11_TEXTURE2D_DESC                        m_Desc;
    UINT                                        m_LastBoundRegNum;

    ScratchImage                                m_Image;
    WRL::ComPtr<ID3D11Texture2D>			    m_Tex2D;
};
