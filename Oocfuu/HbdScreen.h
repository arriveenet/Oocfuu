#pragma once
#include "Screen.h"
class HbdScreen :
    public Screen
{
public:
    virtual void init();
    virtual void reset();
    virtual void update();
    virtual void draw();
};

