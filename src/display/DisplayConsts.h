#ifndef DISPLAYCONSTS_H
#define DISPLAYCONSTS_H

// Main Window/Rendering
// Defined Constants
inline constexpr int DISPLAY_MAX_X = 1920;
inline constexpr int DISPLAY_MAX_Y = 1080;
inline constexpr int MAX_FRAMERATE = 60;

// Tower Selection "Menu"
// Defined Constants
inline constexpr int ICON_LEFT_PADDING = 180;
inline constexpr int ICON_TOP_PADDING = 100;
inline constexpr int ICON_GAP = 20;

// Placement Constants
inline constexpr float ICON_START_X = DISPLAY_MAX_X - ICON_LEFT_PADDING;
inline constexpr float ICON_START_Y = 0 + ICON_TOP_PADDING;
inline constexpr float SEL_MENU_START_X = ICON_START_X - 50.f * 1.8 - ICON_GAP;

// Text
// Defined Constants
inline constexpr int TEXT_TOP_PADDING           = 0;
inline constexpr float FPS_PERCENT_RIGHT        = 0;
inline constexpr float LIFE_PERCENT_RIGHT       = 1.f / 15.f;
inline constexpr float PRESSURE_PERCENT_RIGHT   = 4.f / 10;
inline constexpr float BALANCE_PERCENT_RIGHT    = 2.f / 10.f;
inline constexpr float WAVE_PERCENT_RIGHT       = 7.8 / 10.f;

// Placement Constants
inline constexpr float FPS_START_RIGHT      = DISPLAY_MAX_X * FPS_PERCENT_RIGHT;
inline constexpr float LIFE_START_RIGHT     = DISPLAY_MAX_X * LIFE_PERCENT_RIGHT;
inline constexpr float PRESSURE_START_RIGHT = DISPLAY_MAX_X * PRESSURE_PERCENT_RIGHT;
inline constexpr float BALANCE_START_RIGHT  = DISPLAY_MAX_X * BALANCE_PERCENT_RIGHT;
inline constexpr float WAVE_START_RIGHT     = DISPLAY_MAX_X * WAVE_PERCENT_RIGHT;
inline constexpr float TEXT_START_Y = 0 + TEXT_TOP_PADDING;
inline constexpr float PRESSURE_GAP_Y = 25;

#endif //DISPLAYCONSTS_H
