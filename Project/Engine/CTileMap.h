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
    int                     m_Row;              // TileMap 의 행 숫자
    int                     m_Col;              // TileMap 의 열 숫자
    Vec2                    m_TileSize;         // Tile 1개의 크기

    Ptr<CTexture>           m_TileAtlas;        // Tile 개별 이미지들으 보유하고 있는 아틀라스 텍스쳐
    Vec2                    m_AtlasResolution;  // Atlas 텍스쳐 해상도
    Vec2                    m_AtlasTileSize;    // Atlas 텍스쳐 내에서 타일 1개의 크기
    Vec2                    m_AtlasTileSliceUV;

    int                     m_AtlasMaxRow;      // Atlas 텍스쳐가 보유하고 있는 타일의 최대 행 숫자
    int                     m_AtlasMaxCol;      // Atlas 텍스쳐가 보유하고 있는 타일의 최대 열 숫자

    vector<tTileInfo>       m_vecTileInfo;      // 각각의 타일의 정보

    std::shared_ptr<CStructuredBuffer>      m_Buffer;
};