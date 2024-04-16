#include <M5Unified.h>

#include "config.hpp"

#include "ui/screen_manager.hpp"

int32_t old_inc;

void setup()
{
    M5.begin();
    synth.begin(&Serial2, UNIT_SYNTH_BAUD, 18, 17);
    hmi_module.begin(&Wire1, HMI_ADDR, 11, 12);
    synthData.begin();
    player.begin();
    screen_begin();

    old_inc = hmi_module.getEncoderValue();
}

void loop()
{
    M5.update();
    M5.BtnA.setRawState(millis(), !hmi_module.getButton1());
    M5.BtnB.setRawState(millis(), !hmi_module.getButton2());
    M5.BtnC.setRawState(millis(), !hmi_module.getButtonS());

    screen_update();

    if (M5.BtnA.wasClicked())
    {
        screen->aButton();
        delay(10);
    }
    if (M5.BtnB.wasClicked())
    {
        screen->bButton();
        delay(10);
    }
    if (M5.BtnA.wasHold())
    {
        screen->aHold();
        delay(10);
    }
    if (M5.BtnB.wasHold())
    {
        screen->bHold();
        delay(10);
    }
    if (M5.BtnC.wasClicked())
    {
        screen->enterButton();
        delay(10);
    }

    if (M5.BtnC.wasHold())
    {
        screen->enterHold();
        delay(10);
    }

    auto inc = hmi_module.getEncoderValue();
    if (inc > old_inc + 4)
    {
        screen->downButton();
        old_inc = inc;
        delay(10);
    }
    else if (inc < old_inc - 4)
    {
        screen->upButton();
        old_inc = inc;
        delay(10);
    }

    auto t = M5.Touch.getDetail();
    if (t.wasClicked())
    {
        screen->touchPoint(t.x, t.y);
        delay(10);
    }
    else
    {
        if (t.isFlicking() || t.wasFlicked())
            screen->touchFlick(t.deltaX(), t.deltaX());
        delay(10);
    }
}
