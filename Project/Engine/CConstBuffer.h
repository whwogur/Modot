#pragma once
#include "CEntity.h"

class CConstBuffer :
    public CEntity
{
public:
    CConstBuffer();
    ~CConstBuffer();

public:
    int Create(CB_TYPE _type, UINT _BufferSize);

    void SetData(void* _pData);
    void Binding();

    virtual CConstBuffer* Clone() { return nullptr; }

private:
    WRL::ComPtr<ID3D11Buffer>               m_CB;
    D3D11_BUFFER_DESC                       m_Desc;
    CB_TYPE                                 m_CBType;
};

