#pragma once
#include "ComponentUI.h"
class TileMapUI :
    public ComponentUI
{
public:
    TileMapUI();
    ~TileMapUI();
private:
    virtual void Update() override;

};