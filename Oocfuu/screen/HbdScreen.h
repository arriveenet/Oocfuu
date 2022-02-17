#pragma once
#include "Screen.h"
#include "../Course.h"

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

