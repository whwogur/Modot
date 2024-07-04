#pragma once
#include "AssetUI.h"
class MeshDataUI :
    public AssetUI
{
public:
    MeshDataUI();
    ~MeshDataUI();

public:
    virtual void Update() override;
};