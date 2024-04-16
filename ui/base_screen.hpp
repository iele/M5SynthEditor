#pragma once

int screen_num = 0;

class BaseScreen
{
public:
    virtual void begin() = 0;
    virtual void update() = 0;

    virtual void upButton(){};
    virtual void downButton(){};
    virtual void enterButton(){};
    virtual void enterHold(){};
    virtual void aButton(){};
    virtual void bButton(){};
    virtual void aHold(){};
    virtual void bHold(){};
    virtual void touchPoint(int16_t x, int16_t y){};
    virtual void touchFlick(int x, int y){};
};