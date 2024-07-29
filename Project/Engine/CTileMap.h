#pragma once
#include "CRenderComponent.h"


class CStructuredBuffer;

struct tTileInfo
{
    int ImgIdx = 5;
    int padding[3];
};

class CTileMap :
    public CRenderComponent
{

public:
    CTileMap();
    ~CTileMap() = default;
    CTileMap(const CTileMap& _Other);
    CLONE(CTileMap);
public:
    void SetRowCol(UINT _Row, UINT _Col);
    void SetTileSize(Vec2 _Size);
    void SetAtlasTexture(Ptr<CTexture> _Atlas);
    void SetAtlasTileSize(Vec2 _TileSize);
    void SetTileInfo(const vector<tTileInfo>& _vecTileInfo) { m_vecTileInfo.assign(_vecTileInfo.begin(), _vecTileInfo.end()); }

    Vec2 GetTileSize() const { return m_TileSize; }
    Vec2 GetAtlasTileSize() const { return m_AtlasTileSize; }
    Vec2 GetRowCol() { return Vec2(m_Row, m_Col); }
    Vec2 GetTileSliceUV() const { return m_AtlasTileSliceUV; }
    Ptr<CTexture> GetAtlasTexture() { return m_TileAtlas; }
    Vec2& GetAtlasTileSizeRef() { return m_AtlasTileSize; }
    Vec2& GetTileSizeRef() { return m_TileSize; }
    int& GetRowRef() { return m_Row; }
    int& GetColRef() { return m_Col; }
    vector<tTileInfo>& GetTileInfoRef() { return m_vecTileInfo; }
    int GetMaxAtlasRow() const { return m_AtlasMaxRow; }
    int GetMaxAtlasCol() const { return m_AtlasMaxCol; }
public:
    virtual void Init() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

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