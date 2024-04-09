#pragma once

#include <M5Unified.h>

#include "../config.hpp"
#include "../data.hpp"
#include "../player.hpp"

int channel_colors[8] = {TFT_BROWN, TFT_CYAN, TFT_GOLD, TFT_MAGENTA,
                         TFT_ORANGE, TFT_YELLOW, TFT_BLUE, TFT_RED};

class MainScreen
{
public:
    void begin();
    void update();

    void upButton();
    void downButton();
    void enterButton();
    void enterHold();
    void aButton();
    void bButton();
    void bHold();

private:
    M5Canvas main_canvas;
    M5Canvas bg_canvas;
    unsigned long last_time;

    int32_t text_height;
    int32_t text_width;
    int32_t channel_start_pos;
    int32_t note_start_pos;

    uint8_t op_mode;
    uint8_t last_op_mode;
    uint8_t pos_x;
    uint8_t pos_y;
    uint8_t last_note;

    unsigned long last_play_time;
};

void MainScreen::begin()
{
    main_canvas.createSprite(M5.Display.width(), M5.Display.height());
    main_canvas.clear();

    bg_canvas.createSprite(M5.Display.width(), M5.Display.height());
    bg_canvas.clear();
    bg_canvas.fillScreen(TFT_BLACK);
    bg_canvas.setTextFont(&AsciiFont8x16);

    op_mode = 0;
    pos_x = 0;
    pos_y = 0;

    text_height = bg_canvas.fontHeight(&AsciiFont8x16);
    text_width = bg_canvas.fontWidth(&AsciiFont8x16);
    channel_start_pos = M5.Display.height() - (text_height + 3) * DATA_LENGTH;
    note_start_pos = text_width * 6;
}

void MainScreen::update()
{
    main_canvas.clear();
    bg_canvas.clear();

    bg_canvas.drawString("M5Synth for CoreS3", 0, 0);
    char power_str[4];
    sprintf(power_str, "%03d", M5.Power.getBatteryLevel());
    bg_canvas.drawString(power_str, M5.Display.width() - 3 * text_width, 0);
    bg_canvas.fillRectAlpha(0, 0, M5.Display.width(), text_height, 128, TFT_BLUE);

    char volume_str[4];
    sprintf(volume_str, "%03d", synthData.master_volume);
    bg_canvas.drawString("Master Volume:", 0, text_height);
    bg_canvas.drawString(volume_str, text_width * 15, text_height);

    char bpm_str[4];
    sprintf(bpm_str, "%03d", synthData.bpm);
    bg_canvas.drawString("BPM:", 0, text_height * 2);
    bg_canvas.drawString(bpm_str, text_width * 5, text_height * 2);
    char tempo_str[4];
    sprintf(tempo_str, "%1d/%1d", 4, 4);
    bg_canvas.drawString("Tempo:", text_width * 9, text_height * 2);
    bg_canvas.drawString(tempo_str, text_width * 16, text_height * 2);

    bg_canvas.fillRectAlpha(0, text_height,
                            M5.Display.width(), channel_start_pos - text_height, 128, TFT_DARKGRAY);

    for (int i = 0; i < DATA_LENGTH; i++)
    {
        bg_canvas.writeFastHLine(0,
                                 channel_start_pos + (text_height + 3) * i,
                                 M5.Display.width(),
                                 TFT_WHITE);
    }
    for (int i = 0; i < M5.Display.width() / text_width; i++)
    {
        bg_canvas.writeFastVLine(note_start_pos + 3 * (text_width + 3) * i,
                                 channel_start_pos,
                                 M5.Display.width(),
                                 i % 4 == 0 ? TFT_LIGHTGREY : TFT_DARKGRAY);
    }
    for (int i = 0; i < DATA_LENGTH; i++)
    {
        auto instrument = synthData.getInstrument(i);
        char channel_name[7];
        i == 0 ? sprintf(channel_name, "Drum%02X", instrument)
               : sprintf(channel_name, "CH%d.%02X", i, instrument);
        bg_canvas.drawString(channel_name, 0, channel_start_pos + (text_height + 3) * i + 2);

        for (int j = 0; j < M5.Display.width() / text_width; j++)
        {
            char note_name[4];
            sprintf(note_name, "%s", tone_name(synthData.getNote(i, j)));
            bg_canvas.drawString(note_name,
                                 note_start_pos + 3 * (text_width + 3) * j + 3,
                                 channel_start_pos + (text_height + 3) * i + 2);
        }

        bg_canvas.fillRectAlpha(0,
                                channel_start_pos + (text_height + 3) * i + 1,
                                M5.Display.width(),
                                text_height + 1,
                                64,
                                channel_colors[i]);
    }

    switch (op_mode)
    {
    case 0:
    case 1:
    {
        bg_canvas.fillRectAlpha(note_start_pos + 3 * (text_width + 3) * pos_x + 1,
                                channel_start_pos + (text_height + 3) * pos_y + 2,
                                4 * text_width,
                                1 * text_height, 0x88, TFT_DARKGRAY);
        break;
    }
    case 2:
    {
        bg_canvas.fillRectAlpha(note_start_pos + 3 * (text_width + 3) * pos_x + 1,
                                channel_start_pos + (text_height + 3) * pos_y + 2,
                                4 * text_width,
                                1 * text_height, 0x88, TFT_WHITE);
        break;
    }
    case 3:
    {
        bg_canvas.fillRectAlpha(0,
                                channel_start_pos + (text_height + 3) * pos_y + 2,
                                6 * text_width,
                                1 * text_height, 0x88, TFT_DARKGRAY);
        break;
    }
    case 4:
    {
        bg_canvas.fillRectAlpha(0,
                                channel_start_pos + (text_height + 3) * pos_y + 2,
                                6 * text_width,
                                1 * text_height, 0x88, TFT_WHITE);
        char instument[30];
        sprintf(instument, "%s", instrument_name(synthData.getInstrument(pos_y)));
        bg_canvas.drawString(instument, 0, channel_start_pos - (text_height + 3));
        break;
    }
    case 0XFF:
    {
        for (int i = 0; i < DATA_LENGTH; i++)
        {

            bg_canvas.fillRectAlpha(note_start_pos + 3 * (text_width + 3) * player.current + 1,
                                    channel_start_pos + (text_height + 3) * i + 2,
                                    4 * text_width,
                                    1 * text_height, 0x88, TFT_DARKGREEN);
        }
        break;
    }
    }

    auto current_play_time = millis();
    if (current_play_time > last_play_time + 60000 / synthData.bpm)
    {
        player.play();
        last_play_time = current_play_time;
    }

    auto current_time = millis();
    if (current_time > last_time + 1000 / SCREEN_HZ)
    {
        M5.Display.startWrite();
        bg_canvas.pushSprite(&main_canvas, 0, 0);
        main_canvas.pushSprite(&M5.Display, 0, 0);
        M5.Display.endWrite();
        last_time = current_time;
    }
}

void MainScreen::upButton()
{
    switch (op_mode)
    {
    case 0:
    {
        pos_x = pos_x < 8 - 1 ? pos_x + 1 : 7;
        break;
    }
    case 1:
    case 3:
    {
        pos_y = pos_y < 8 - 1 ? pos_y + 1 : 7;
        break;
    }
    case 2:
    {
        auto note = synthData.getNote(pos_y, pos_x);
        if (note == 0)
        {
            note = last_note;
        }
        synthData.setNote(pos_y, pos_x, ++note, 127);
        last_note = synthData.getNote(pos_y, pos_x);
        break;
    }
    case 4:
    {
        auto instrument = synthData.getInstrument(pos_y);
        synthData.setInstrument(pos_y, ++instrument);
        break;
    }
    }
}

void MainScreen::downButton()
{
    switch (op_mode)
    {
    case 0:
    {
        pos_x = pos_x > 0 ? pos_x - 1 : 0;
        break;
    }
    case 1:
    case 3:
    {
        pos_y = pos_y > 0 ? pos_y - 1 : 0;
        break;
    }
    case 2:
    {
        auto note = synthData.getNote(pos_y, pos_x);
        if (note == 0)
        {
            note = last_note;
        }
        synthData.setNote(pos_y, pos_x, --note, 127);
        last_note = synthData.getNote(pos_y, pos_x);
        break;
    }
    case 4:
    {
        auto instrument = synthData.getInstrument(pos_y);
        synthData.setInstrument(pos_y, --instrument);
        break;
    }
    }
}
void MainScreen::enterButton()
{
}

void MainScreen::enterHold()
{
    switch (op_mode)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    {
        op_mode = 0xFF;
        player.start();
        break;
    }
    case 0xFF:
    {
        op_mode = last_op_mode;
        player.stop();
        break;
    }
    }
}
void MainScreen::aButton()
{
    switch (op_mode)
    {
    case 0:
    case 1:
    {
        op_mode = 2;
        break;
    }
    case 2:
    {
        op_mode = last_op_mode;
        break;
    }
    case 3:
    {
        op_mode = 4;
        break;
    }
    case 4:
    {
        op_mode = 3;
        break;
    }
    }
}
void MainScreen::bButton()
{
    switch (op_mode)
    {
    case 0:
    {
        last_op_mode = op_mode = 1;
        break;
    }
    case 1:
    {
        last_op_mode = op_mode = 0;
        break;
    }
    }
}
void MainScreen::bHold()
{
    switch (op_mode)
    {
    case 0:
    case 1:
    {
        op_mode = 3;
        break;
    }
    case 3:
    {
        op_mode = last_op_mode;
    }
    }
}

MainScreen mainScreen;