#pragma once

#include <M5Unified.h>

#include "base_screen.hpp"

#include "../config.hpp"
#include "../data.hpp"
#include "../player.hpp"

class SettingsScreen : public BaseScreen
{
public:
    void begin();
    void update();
    void touchPoint(int16_t x, int16_t y);

private:
    M5Canvas main_canvas;
    unsigned long last_time;
};

void SettingsScreen::begin()
{
    main_canvas.createSprite(M5.Display.width(), M5.Display.height());
    main_canvas.clear();
    main_canvas.fillScreen(TFT_TRANSPARENT);
    main_canvas.setTextFont(&AsciiFont8x16);
}

void SettingsScreen::update()
{
    auto current_time = millis();
    if (current_time > last_time + 1000 / SCREEN_HZ)
    {
        M5.Display.startWrite();
        main_canvas.pushSprite(&M5.Display, 0, 0);
        M5.Display.endWrite();
        last_time = current_time;
    }
}

void SettingsScreen::touchPoint(int16_t x, int16_t y) {
    screen_num = 0;
}
