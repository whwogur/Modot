#pragma once
#include "CComponent.h"
enum class OBJECT_DIR
{
    LEFT,
    RIGHT,
};

class CTransform :
    public CComponent
{
public:
    CTransform();
    ~CTransform() = default;
    CLONE(CTransform);
public:
    virtual void FinalTick() override;
    void Bind();

public:
    void SetRelativePos(Vec3 _Pos) { m_RelativePos = _Pos; }
    void SetRelativeScale(Vec3 _Scale) { m_RelativeScale = _Scale; }
    void SetRelativeRotation(Vec3 _Rot) { m_RelativeRotation = _Rot; }
    void SetWorldMatrix(const Matrix& matWorld) { m_matWorld = matWorld; }
    void SetIndependentScale(bool _Set) { m_IndependentScale = _Set; }
    bool IsIndependentScale() { return m_IndependentScale; }

    void SetRelativePos(float x, float y, float z) { m_RelativePos = Vec3(x, y, z); }
    void SetRelativeScale(float x, float y, float z) { m_RelativeScale = Vec3(x, y, z); }
    void SetRelativeRotation(float x, float y, float z) { m_RelativeRotation = Vec3(x, y, z); }
    void SetDir(OBJECT_DIR _Dir);
    OBJECT_DIR GetObjectDir() const { return m_RelativeScale.x > 0 ? OBJECT_DIR::RIGHT : OBJECT_DIR::LEFT; }

    Matrix& GetWorldMat() { return m_matWorld; }
    Matrix& GetTransformationMat() { return m_matTransformation; }

    Vec3 GetRelativePos() { return m_RelativePos; }
    Vec3& GetRelativePosRef() { return m_RelativePos; }
    Vec3 GetWorldPos() { return m_matWorld.Translation(); }
    Vec3 GetRelativeScale() { return m_RelativeScale; }
    Vec3& GetRelativeScaleRef() { return m_RelativeScale; }
    Vec3 GetWorldScale();
    Vec3 GetRelativeRotation() { return m_RelativeRotation; }
    Vec3& GetRelativeRotationRef() { return m_RelativeRotation; }
    Vec3 GetRelativeDir(DIR _Type) { return m_RelativeDir[_Type]; }
    Vec3 GetWorldDir(DIR _Type) { return m_WorldDir[_Type]; }

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
private:
    Vec3    m_RelativePos;
    Vec3    m_RelativeScale;
    Vec3    m_RelativeRotation;
    Vec3    m_RelativeDir[3];
    Vec3    m_WorldDir[3];

    Matrix  m_matWorld;
    Matrix m_matTransformation = {};
    bool    m_IndependentScale;
};