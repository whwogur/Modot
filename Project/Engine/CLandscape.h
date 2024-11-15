#pragma once
#include "CRenderComponent.h"
#include "CHeightmapCS.h"
#include "CRaycastCS.h"

struct tRaycastOut
{
    Vec2    Location;
    UINT    Distance;
    int     Success;
};

class CLandscape :
    public CRenderComponent
{
public:
    CLONE(CLandscape);
    CLandscape();
    ~CLandscape() = default;
public:
    void SetFace(int _X, int _Z);
    void SetTessLevel(float _TessLv) { m_TessLevel = _TessLv; }
    void AddBrushTexture(Ptr<CTexture> _BrushTex) { m_vecBrush.push_back(_BrushTex); }
    void SetHeightMap(Ptr<CTexture> _HeightMap) { m_Heightmap = _HeightMap;  m_IsHeightMapCreated = false; }
    void CreateHeightMap(UINT _Width, UINT _Height);

    int GetFaceX() const { return m_FaceX; }
    int GetFaceZ() const { return m_FaceZ; }

    float GetTessLevel() const { return m_TessLevel; }
    Ptr<CTexture> GetHeightMap() { return m_Heightmap; }

public:
    virtual void Init() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    int RayCast();
private:
    void CreateMesh();
    void CreateComputeShader();
    void CreateTextureAndStructuredBuffer();

private:
    int                                             m_FaceX = 1;
    int                                             m_FaceZ = 1;

    float                                           m_TessLevel = 8.f;

    // Heightmap
    Ptr<CTexture>                                   m_Heightmap;
    Ptr<CHeightmapCS>                               m_HeightmapCS;
    bool                                            m_IsHeightMapCreated = false;

    // Brush
    Vec2                                            m_BrushScale = { 0.2f, 0.2f };
    std::vector<Ptr<CTexture>>                      m_vecBrush;
    UINT                                            m_BrushIdx = -1;

    // Raycasting
    Ptr<CRaycastCS>                                 m_RaycastCS;
    std::shared_ptr<CStructuredBuffer>              m_RaycastOut;
    tRaycastOut                                     m_Out;
};

