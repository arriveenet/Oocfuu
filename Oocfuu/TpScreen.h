#pragma once
#include "Screen.h"

class TpScreen :
    public Screen
{
    unsigned int m_count = 0;
public:
    virtual void init();
    virtual void reset();
    virtual void update();
    virtual void draw();
};

