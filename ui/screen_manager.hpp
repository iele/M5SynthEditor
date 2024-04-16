#pragma once

#include "base_screen.hpp"
#include "setting_screen.hpp"
#include "main_screen.hpp"

BaseScreen *screen;
MainScreen mainScreen;
SettingsScreen settingsScreen;

void screen_begin()
{
    mainScreen.begin();
    settingsScreen.begin();
    screen = &mainScreen;
}

void screen_update()
{
    screen->update();
    switch (screen_num)
    {
    case 0:
        screen = &mainScreen;
        break;
    case 1:
        screen = &settingsScreen;
        break;
    }
}