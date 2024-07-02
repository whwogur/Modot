#pragma once
#include "EditorUI.h"

class Outliner :
    public EditorUI
{
public:
    Outliner();
    ~Outliner();
public:
    virtual void Update() override;

};
