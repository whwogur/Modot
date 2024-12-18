#pragma once
#include "CRenderComponent.h"
#include "CRaycastCS.h"

#include "CHeightmapCS.h"
#include "CWeightMapCS.h"

struct tRaycastOut
{
    Vec4    WorldPos;
    Vec2    LocationUV;
    UINT    Distance;
    int     Success;
};

enum LANDSCAPE_MODE
{
    NONE,
    HEIGHTMAP,
    SPLAT,
    PINPOINT,
};

struct tWeight8
{
    float arrWeight[8];
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
    void AddBrushTexture(Ptr<CTexture> _BrushTex) { m_vecBrush.push_back(_BrushTex); }
    void SetHeightMap(Ptr<CTexture> _HeightMap) { m_Heightmap = _HeightMap;  m_IsHeightMapCreated = false; }
    void CreateHeightMap(UINT _Width, UINT _Height);

    int GetFaceX() const { return m_FaceX; }
    int GetFaceZ() const { return m_FaceZ; }

    Ptr<CTexture> GetHeightMap() { return m_Heightmap; }
    
    bool GetWireframeEnabled() const { return m_WireFrame; }
    void SetWireframeEnabled(bool _b);

    LANDSCAPE_MODE GetMode() const { return m_Mode; }
    void SetMode(LANDSCAPE_MODE _Mode) { m_Mode = _Mode; }

    UINT GetBrushIdx() const { return m_BrushIdx; }
    void SetBrushIdx(UINT _Idx) { m_BrushIdx = _Idx; }

    int GetWeightIdx() const { return m_WeightIdx; }
    void SetWeightIdx(int _Idx) { m_WeightIdx = _Idx; }

    bool& GetEditEnableRef() { return m_EditEnable; }
    void SetEdit(bool _b) { m_EditEnable = _b; }
    bool GetEditEnable() const { return m_EditEnable; }

    Vec2& GetBrushScaleRef() { return m_BrushScale;  }
    void SetBrushScale(float _X, float _Y) { m_BrushScale.x = _X, m_BrushScale.y = _Y; }

    float& GetTessMaxLvRef() { return m_MaxLevel; }
    float& GetTessMinLvRef() { return m_MinLevel; }
    float& GetTessMaxThresholdRef() { return m_MaxThreshold; }
    float& GetTessMinThresholdRef() { return m_MinThreshold; }

    const tRaycastOut& GetRaycastInfo() const { return m_Out; }
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

    // Tessellation
    float                                           m_MinLevel = 0;
    float                                           m_MaxLevel = 4;
    float                                           m_MinThreshold = 1500.f;
    float                                           m_MaxThreshold = 500.f;

    bool                                            m_WireFrame = false;
    bool                                            m_EditEnable = false;

    // Weightmap
    Ptr<CTexture>                                   m_ColorTex = nullptr;
    Ptr<CTexture>                                   m_NormalTex = nullptr;
    Ptr<CWeightMapCS>                               m_WeightmapCS = nullptr;

    std::shared_ptr<CStructuredBuffer>              m_Weightmap = nullptr;
    UINT                                            m_WeightWidth = 0;
    UINT                                            m_WeightHeight = 0;
    int                                             m_WeightIdx;

    LANDSCAPE_MODE                                  m_Mode;

    // Heightmap
    Ptr<CTexture>                                   m_Heightmap;
    Ptr<CHeightmapCS>                               m_HeightmapCS;
    bool                                            m_IsHeightMapCreated = false;

    // Brush
    Vec2                                            m_BrushScale = { 0.2f, 0.2f };
    std::vector<Ptr<CTexture>>                      m_vecBrush;
    UINT                                            m_BrushIdx = 0;

    // Raycasting
    Ptr<CRaycastCS>                                 m_RaycastCS;
    std::shared_ptr<CStructuredBuffer>              m_RaycastInfoBuffer;
    tRaycastOut                                     m_Out;
};

