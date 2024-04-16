#pragma once

#include <M5Unified.h>

#include "data.hpp"
#include "config.hpp"

class Player
{
public:
    void begin();
    void play();
    void start();
    void stop();

    uint8_t current;
    bool is_playing;
};

void Player::begin()
{
}

void Player::play()
{
    if (!is_playing)
        return;
    for (int i = 0; i < DATA_LENGTH; i++)
    {
        auto note = synthData.getNote(i, current);
        synth.setAllNotesOff(i == 0 ? 9 : i);
        synth.setNoteOn(i == 0 ? 9 : i, note, 127);
    }
    current++;
    if (current >= synthData.length)
        current = 0;
}

void Player::start()
{
    is_playing = true;
    current = 0;
    synth.setMasterVolume(127);
    for (int i = 0; i < DATA_LENGTH; i++)
    {
        synth.setVolume(i == 0 ? 9 : i, synthData.getVolume(i));
    }
    for (int i = 0; i < DATA_LENGTH; i++)
    {
        synth.setInstrument(0, i == 0 ? 9 : i, synthData.getInstrument(i));
    }
}

void Player::stop()
{
    is_playing = false;
    for (int i = 0; i < DATA_LENGTH; i++)
    {
        synth.setAllNotesOff(i == 0 ? 9 : i);
    }
}

Player player;
