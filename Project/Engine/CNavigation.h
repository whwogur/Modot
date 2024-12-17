#pragma once
#include "CComponent.h"
class CCell;
class CNavigation :
    public CComponent
{
public:
    CNavigation();
    ~CNavigation() = default;
    CLONE(CNavigation);
    typedef struct tagNavigationDesc
    {
        int			iCurrentCellIndex = -1;
    }NAVIDESC;
public:
    float Compute_Height(const Vec3& _Position, float _Offset);
    void Compute_CurrentIdx_viaDistance(const Vec3& _Position);
    bool Compute_CurrentIdx_viaHeight(const Vec3& _Position);

public:
    bool CanMove(const Vec3& _Position);
    int	Get_CurrentCellIndex() { return m_NaviDesc.iCurrentCellIndex; }
    UINT Get_CurrentCelltype();
    const Vec3& Get_CurrentCellCenter();
    const Vec3& Get_LastNormal() { return m_vLastNormal; }

public: // ������Ʈ �������̽�
    virtual void FinalTick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    HRESULT InitializeNeighbors();

private:
    NAVIDESC				        m_NaviDesc;
    std::vector<CCell*>	            m_Cells;
    Vec3					        m_vLastNormal = {0, 0, 0};
    bool					        m_bIs2D = false;

    /* x z�� ���� ���̰� �ٸ� ���� ������ ���� �־ �ٸ� �����̳ʿ� ��Ƶΰ�*/
    /* �ش� ���͸� ���Ƽ� ���� �񱳷� ���� ���� Ȯ�� ��ų ��*/
    std::vector<CCell*>	            m_CellsForComputation;
};

