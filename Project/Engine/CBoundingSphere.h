#pragma once
class CBoundingSphere
{
public:
    CBoundingSphere() = default;
    ~CBoundingSphere() = default;
    CBoundingSphere(const CBoundingSphere& _Other);

public:
    float GetRadius() const { return m_Radius; }
    void SetRadius(float _Radius) { m_Radius = _Radius; }

    void RenderBounds(const Vec3& _Pos);

private:
    float m_Radius = 0.f;
};