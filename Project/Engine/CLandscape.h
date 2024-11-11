#pragma once
#include "CRenderComponent.h"
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
    void SetHeightMap(Ptr<CTexture> _HeightMap) { m_HeightMap = _HeightMap; }

    int GetFaceX() const { return m_FaceX; }
    int GetFaceZ() const { return m_FaceZ; }
    std::pair<int, int> GetFaceXZ() { return std::make_pair(m_FaceX, m_FaceZ); }

    float GetTessLevel() const { return m_TessLevel; }

    Ptr<CTexture> GetHeightMap() { return m_HeightMap; }
public:
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void CreateMesh();

private:
    int             m_FaceX;
    int             m_FaceZ;
    Ptr<CTexture>   m_HeightMap = nullptr;
    float           m_TessLevel = 8.f;
};

