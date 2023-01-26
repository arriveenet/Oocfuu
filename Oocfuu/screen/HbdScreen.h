#pragma once
#include "Screen.h"
#include "world/Course.h"

class HbdScreen :
    public Screen
{
public:

    HbdScreen();
    virtual void init();
    virtual void reset();
    virtual void update();
    virtual void draw();
};

