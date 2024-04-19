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
    void upButton();
    void downButton();
    void aButton();
    void bButton();
    void touchPoint(int16_t x, int16_t y);
    void touchFlick(int x, int y);

private:
    int32_t text_height;
    int32_t text_width;

    int32_t settings_height;
    int32_t settings_width;

    int8_t selection;
    uint8_t op_mode;

    M5Canvas main_canvas;
    unsigned long last_time;
};

void SettingsScreen::begin()
{
    main_canvas.createSprite(M5.Display.width(), M5.Display.height());
    main_canvas.clear();
    main_canvas.fillScreen(TFT_BLACK);

    text_height = main_canvas.fontHeight(&AsciiFont8x16);
    text_width = main_canvas.fontWidth(&AsciiFont8x16);

    settings_height = main_canvas.fontHeight(&DejaVu24);
    settings_width = main_canvas.fontWidth(&DejaVu24);
}

void SettingsScreen::update()
{
    main_canvas.clear();

    main_canvas.setTextFont(&AsciiFont8x16);
    main_canvas.drawString("M5Synth for CoreS3", 0, 0);
    char power_str[4];
    sprintf(power_str, "%03d", M5.Power.getBatteryLevel());
    main_canvas.drawString(power_str, M5.Display.width() - 3 * text_width, 0);
    main_canvas.fillRectAlpha(0, 0, M5.Display.width(), text_height, 128, TFT_BLUE);

    main_canvas.setTextFont(&DejaVu24);
    char volume_str[4];
    sprintf(volume_str, "%03d", synthData.master_volume);
    auto h1 = map(0, 0, 4, text_height + 20, M5.Display.height() - 20) + text_height / 2;
    main_canvas.drawString("Volume:", 20, h1);
    main_canvas.drawString(volume_str, M5.Display.width() / 2, h1);

    char bpm_str[4];
    sprintf(bpm_str, "%03d", synthData.bpm);
    auto h2 = map(1, 0, 4, text_height + 20, M5.Display.height() - 20) + text_height / 2;
    main_canvas.drawString("BPM:", 20, h2);
    main_canvas.drawString(bpm_str, M5.Display.width() / 2, h2);

    char len_str[4];
    sprintf(len_str, "%03d", synthData.length);
    auto h3 = map(2, 0, 4, text_height + 20, M5.Display.height() - 20) + text_height / 2;
    main_canvas.drawString("Length:", 20, h3);
    main_canvas.drawString(len_str, M5.Display.width() / 2, h3);

    auto selection_height = map(selection,
                                0,
                                4,
                                text_height + 20,
                                M5.Display.height() - 20) +
                            text_height / 2;
    main_canvas.fillRectAlpha(0,
                              selection_height - 5,
                              M5.Display.width(),
                              settings_height + 5,
                              128, op_mode == 1 ? TFT_LIGHTGRAY : TFT_DARKGRAY);

    main_canvas.fillRectAlpha(0,
                              text_height,
                              M5.Display.width(),
                              M5.Display.height() - text_height,
                              128, TFT_DARKGRAY);

    auto current_time = millis();
    if (current_time > last_time + 1000 / SCREEN_HZ)
    {
        M5.Display.startWrite();
        main_canvas.pushSprite(&M5.Display, 0, 0);
        M5.Display.endWrite();
        last_time = current_time;
    }
}

void SettingsScreen::touchPoint(int16_t x, int16_t y)
{
    auto h1 = map(0, 0, 4, text_height + 20, M5.Display.height() - 20) + text_height / 2;
    auto h2 = map(1, 0, 4, text_height + 20, M5.Display.height() - 20) + text_height / 2;
    auto h3 = map(2, 0, 4, text_height + 20, M5.Display.height() - 20) + text_height / 2;
    if (y > h1 && y < h1 + settings_height)
    {
        op_mode = 1;
        selection = 0;
    }
    else if (y > h2 && y < h2 + settings_height)
    {
        op_mode = 1;
        selection = 1;
    }
    else if (y > h3 && y < h3 + settings_height)
    {
        op_mode = 1;
        selection = 2;
    }
    else
    {
        op_mode = 0;
    }
}

void SettingsScreen::upButton()
{
    switch (op_mode)
    {
    case 0:
    {
        selection++;
        if (selection > 2)
            selection = 0;
        break;
    }
    case 1:
    {
        switch (selection)
        {
        case 0:
            synthData.master_volume++;
            break;
        case 1:
            synthData.bpm++;
            break;
        case 2:
            synthData.length++;
            break;
        }
        break;
    }
    }
}

void SettingsScreen::downButton()
{
    switch (op_mode)
    {
    case 0:
    {
        selection--;
        if (selection < 0)
            selection = 2;
        break;
    }
    case 1:
    {
        switch (selection)
        {
        case 0:
            synthData.master_volume--;
            break;
        case 1:
            synthData.bpm--;
            break;
        case 2:
            synthData.length--;
            break;
        }
        break;
    }
    }
}

void SettingsScreen::touchFlick(int x, int y)
{
    switch (op_mode)
    {
    case 0:
        break;
    case 1:
    {
        switch (selection)
        {
        case 0:
            synthData.master_volume -= y;
            break;
        case 1:
            synthData.bpm -= y;
            break;
        case 2:
            synthData.length -= y;
            break;
        }
    }
    break;
    }
}

void SettingsScreen::aButton()
{
    switch (op_mode)
    {
    case 0:
        op_mode = 1;
        break;
    case 1:
        op_mode = 0;
        break;
    }
}

void SettingsScreen::bButton()
{
    screen_num = 0;
}