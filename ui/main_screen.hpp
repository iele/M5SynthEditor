#pragma once

#include <M5Unified.h>

#include "base_screen.hpp"

#include "../config.hpp"
#include "../data.hpp"
#include "../player.hpp"

int channel_colors[8] = {TFT_BROWN, TFT_CYAN, TFT_GOLD, TFT_MAGENTA,
                         TFT_ORANGE, TFT_YELLOW, TFT_BLUE, TFT_RED};

class MainScreen : public BaseScreen
{
public:
    void begin();
    void update();
    void switch_screen();

    void upButton();
    void downButton();
    void enterButton();
    void enterHold();
    void aButton();
    void bButton();
    void aHold();
    void bHold();
    void touchPoint(int16_t x, int16_t y);
    void touchFlick(int x, int y);

private:
    M5Canvas main_canvas;
    unsigned long last_time;

    int32_t text_height;
    int32_t text_width;
    int32_t channel_start_pos;
    int32_t note_start_pos;

    uint8_t op_mode;
    uint8_t last_op_mode;
    uint8_t pos_y;
    uint8_t pos_x;
    int32_t base;
    int32_t offset;
    uint8_t last_note;

    unsigned long last_play_time;
};

void MainScreen::begin()
{
    main_canvas.createSprite(M5.Display.width(), M5.Display.height());
    main_canvas.clear();
    main_canvas.fillScreen(TFT_BLACK);
    main_canvas.setTextFont(&AsciiFont8x16);

    op_mode = 0;
    pos_x = 0;
    pos_y = 0;
    base = 0;
    offset = 0;

    text_height = main_canvas.fontHeight(&AsciiFont8x16);
    text_width = main_canvas.fontWidth(&AsciiFont8x16);
    channel_start_pos = M5.Display.height() - (text_height + 3) * (DATA_LENGTH + 1);
    note_start_pos = text_width * 6;
}

void MainScreen::update()
{
    main_canvas.clear();

    main_canvas.drawString("M5Synth for CoreS3", 0, 0);
    char power_str[4];
    sprintf(power_str, "%03d", M5.Power.getBatteryLevel());
    main_canvas.drawString(power_str, M5.Display.width() - 3 * text_width, 0);
    main_canvas.fillRectAlpha(0, 0, M5.Display.width(), text_height, 128, TFT_BLUE);

    char volume_str[4];
    sprintf(volume_str, "%03d", synthData.master_volume);
    main_canvas.drawString("Master Volume:", 0, text_height);
    main_canvas.drawString(volume_str, text_width * 15, text_height);

    auto str_x = 0;
    char len_str[4];
    sprintf(len_str, "%03d", synthData.length);
    str_x += main_canvas.drawString("Length:", 0, text_height * 2);
    str_x += main_canvas.drawString(len_str, str_x, text_height * 2);
    char bpm_str[4];
    sprintf(bpm_str, "%03d", synthData.bpm);
    str_x += main_canvas.drawString(" BPM:", str_x, text_height * 2);
    str_x += main_canvas.drawString(bpm_str, str_x, text_height * 2);
    char tempo_str[4];
    sprintf(tempo_str, "%1d/%1d", 4, 4);
    str_x += main_canvas.drawString(" Tempo:", str_x, text_height * 2);
    str_x += main_canvas.drawString(tempo_str, str_x, text_height * 2);

    main_canvas.fillRectAlpha(0,
                            text_height,
                            M5.Display.width(),
                            channel_start_pos - text_height,
                            128, TFT_DARKGRAY);

    for (int i = 0; i < DATA_LENGTH + 1; i++)
    {
        main_canvas.writeFastHLine(0,
                                 channel_start_pos + (text_height + 3) * i,
                                 M5.Display.width(),
                                 TFT_WHITE);
    }

    main_canvas.drawString("Note:",
                         0,
                         M5.Display.height() - text_height);
    main_canvas.writeFastVLine(note_start_pos,
                             channel_start_pos,
                             M5.Display.height() - channel_start_pos,
                             TFT_GOLD);
    for (int i = 0; i < M5.Display.width() / text_width; i++)
    {
        main_canvas.writeFastVLine(note_start_pos + 3 * (text_width + 3) * i + offset,
                                 channel_start_pos,
                                 M5.Display.height() - channel_start_pos,
                                 (i + base) % 4 == 0 ? TFT_LIGHTGREY : TFT_DARKGRAY);
        main_canvas.drawNumber(i + base,
                             note_start_pos + 3 * (text_width + 3) * i + 2,
                             M5.Display.height() - text_height);
    }
    main_canvas.fillRectAlpha(0,
                            M5.Display.height() - text_height - 1,
                            M5.Display.width(),
                            text_height + 1,
                            128, TFT_DARKGRAY);

    for (int i = 0; i < DATA_LENGTH; i++)
    {
        auto instrument = synthData.getInstrument(i);
        char channel_name[7];
        i == 0 ? sprintf(channel_name, "Drum%02X", instrument)
               : sprintf(channel_name, "CH%d.%02X", i, instrument);
        main_canvas.drawString(channel_name, 0, channel_start_pos + (text_height + 3) * i + 2);

        for (int j = 0; j < M5.Display.width() / text_width; j++)
        {
            char note_name[4];
            sprintf(note_name, "%s", tone_name(synthData.getNote(i, j + base)));
            main_canvas.drawString(note_name,
                                 note_start_pos + 3 * (text_width + 3) * j + 3 + offset,
                                 channel_start_pos + (text_height + 3) * i + 2);
        }

        main_canvas.fillRectAlpha(0,
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
        main_canvas.fillRectAlpha(note_start_pos + 3 * (text_width + 3) * pos_x + 1 + offset,
                                channel_start_pos + (text_height + 3) * pos_y + 1,
                                4 * text_width + 1,
                                1 * text_height + 2,
                                0x88, TFT_DARKGRAY);
        break;
    }
    case 2:
    {
        main_canvas.fillRectAlpha(note_start_pos + 3 * (text_width + 3) * pos_x + 1 + offset,
                                channel_start_pos + (text_height + 3) * pos_y + 1,
                                4 * text_width + 1,
                                1 * text_height + 2,
                                0x88, TFT_WHITE);
        break;
    }
    case 3:
    {
        main_canvas.fillRectAlpha(0,
                                channel_start_pos + (text_height + 3) * pos_y + 2,
                                6 * text_width,
                                1 * text_height, 0x88, TFT_DARKGRAY);
        break;
    }
    case 4:
    {
        main_canvas.fillRectAlpha(0,
                                channel_start_pos + (text_height + 3) * pos_y + 2,
                                6 * text_width,
                                1 * text_height, 0x88, TFT_WHITE);
        char instument[30];
        sprintf(instument, "%s", instrument_name(synthData.getInstrument(pos_y)));
        main_canvas.drawString(instument, 0, channel_start_pos - (text_height + 3));
        break;
    }
    case 0XFF:
    {
        auto current = player.current;
        base = current - current % 8;
        for (int i = 0; i < DATA_LENGTH; i++)
        {
            main_canvas.fillRectAlpha(note_start_pos + 3 * (text_width + 3) * (current % 8) + 1,
                                    channel_start_pos + (text_height + 3) * i + 1,
                                    4 * text_width + 1,
                                    1 * text_height + 2,
                                    0x88, TFT_DARKGREEN);
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
        if (pos_x < 7)
            pos_x++;
        else
        {
            if (base + pos_x < synthData.length)
                base++;
        }
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
        auto note = synthData.getNote(pos_y, pos_x + base);
        if (note == 0)
        {
            note = last_note;
        }
        synthData.setNote(pos_y, pos_x + base, ++note, 127);
        last_note = synthData.getNote(pos_y, pos_x + base);
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
        if (pos_x > 0)
            pos_x--;
        else
        {
            if (base > 0)
                base--;
        }
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
        auto note = synthData.getNote(pos_y, pos_x + base);
        if (note == 0)
        {
            note = last_note;
        }
        synthData.setNote(pos_y, pos_x + base, --note, 127);
        last_note = synthData.getNote(pos_y, pos_x + base);
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
        base = 0;
        offset = 0;
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
void MainScreen::aHold()
{
    switch (op_mode)
    {
    case 0:
    case 1:
    case 3:
    {
        synthData.setNote(pos_y, pos_x + base, 0, 127);
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
void MainScreen::touchPoint(int16_t x, int16_t y)
{
    switch (op_mode)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    {
        if (y < channel_start_pos) {
            screen_num = 1;
        }
        if (y > channel_start_pos && y < M5.Display.height() - text_height)
        {
            pos_y = map(y,
                        channel_start_pos, M5.Display.height() - text_height,
                        0, 8);
            if (x < note_start_pos)
            {
                op_mode = 4;
            }
            if (x > note_start_pos)
            {
                pos_x = map(x,
                            note_start_pos, note_start_pos + 3 * (text_width + 3) * 8,
                            0, 8);
                op_mode = 2;
            }
        }
        if (y > M5.Display.height() - text_height)
        {
            op_mode = last_op_mode;
        }
        break;
    }
    }
}
void MainScreen::touchFlick(int x, int y)
{
    switch (op_mode)
    {
    case 0:
    case 1:
    {
        // offset = (x) % (3 * (text_width + 3));
        base -= x / (3 * (text_width + 3));
        if (base < 0)
            base = 0;
        if (base + 8 > synthData.length)
            base = (synthData.length - 8);
        break;
    }
    case 2:
    {
        auto note = synthData.getNote(pos_y, pos_x + base);
        if (note == 0)
        {
            note = last_note;
        }
        synthData.setNote(pos_y, pos_x + base, note - y, 127);
        last_note = synthData.getNote(pos_y, pos_x + base);
        break;
    }
    case 4:
    {
        auto instrument = synthData.getInstrument(pos_y);
        synthData.setInstrument(pos_y, instrument - y);
        break;
    }
    }
}
