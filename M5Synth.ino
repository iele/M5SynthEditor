#include <M5Unified.h>

#include "ui/main_screen.hpp"
#include "config.hpp"

int32_t old_inc;

void setup()
{
    M5.begin();
    synth.begin(&Serial2, UNIT_SYNTH_BAUD, 18, 17);
    hmi_module.begin(&Wire1, HMI_ADDR, 11, 12);
    synthData.begin();
    player.begin();
    mainScreen.begin();
    
    old_inc = hmi_module.getEncoderValue();
}

void loop()
{
    M5.update();
    M5.BtnA.setRawState(millis(), !hmi_module.getButton1());
    M5.BtnB.setRawState(millis(), !hmi_module.getButton2());
    M5.BtnC.setRawState(millis(), !hmi_module.getButtonS());

    mainScreen.update();

    if (M5.BtnA.wasPressed())
    {
        mainScreen.aButton();
        delay(10);
    }
    if (M5.BtnB.wasPressed())
    {
        mainScreen.bButton();
        delay(10);
    }
    if (M5.BtnB.wasHold())
    {
        mainScreen.bHold();
        delay(10);
    }
    if (M5.BtnC.wasPressed())
    {
        mainScreen.enterButton();
        delay(10);
    }

    if (M5.BtnC.wasHold())
    {
        mainScreen.enterHold();
        delay(10);
    }

    auto inc = hmi_module.getEncoderValue();
    if (inc > old_inc + 4)
    {
        mainScreen.downButton();
        old_inc = inc;
        delay(10);
    }

    if (inc < old_inc - 4)
    {
        mainScreen.upButton();
        old_inc = inc;
        delay(10);
    }
}
