#pragma once
#include "CRenderComponent.h"


class CStructuredBuffer;

struct tTileInfo
{
    int ImgIdx = 2;
    int padding[3];
};

class CTileMap :
    public CRenderComponent
{

public:
    CTileMap();
    ~CTileMap();
public:
    void SetRowCol(UINT _Row, UINT _Col);
    void SetTileSize(Vec2 _Size);
    void SetAtlasTexture(Ptr<CTexture> _Atlas);
    void SetAtlasTileSize(Vec2 _TileSize);
    Ptr<CTexture> GetAtlasTexture() { return m_TileAtlas; }
public:
    virtual void FinalTick() override;
    virtual void Render() override;

private:
    void ChangeTileMapSize();
private:
    int                     m_Row;              // TileMap �� �� ����
    int                     m_Col;              // TileMap �� �� ����
    Vec2                    m_TileSize;         // Tile 1���� ũ��

    Ptr<CTexture>           m_TileAtlas;        // Tile ���� �̹������� �����ϰ� �ִ� ��Ʋ�� �ؽ���
    Vec2                    m_AtlasResolution;  // Atlas �ؽ��� �ػ�
    Vec2                    m_AtlasTileSize;    // Atlas �ؽ��� ������ Ÿ�� 1���� ũ��
    Vec2                    m_AtlasTileSliceUV;

    int                     m_AtlasMaxRow;      // Atlas �ؽ��İ� �����ϰ� �ִ� Ÿ���� �ִ� �� ����
    int                     m_AtlasMaxCol;      // Atlas �ؽ��İ� �����ϰ� �ִ� Ÿ���� �ִ� �� ����

    vector<tTileInfo>       m_vecTileInfo;      // ������ Ÿ���� ����

    std::shared_ptr<CStructuredBuffer>      m_Buffer;
};