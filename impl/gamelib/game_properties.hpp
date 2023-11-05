#ifndef GAME_GAMEPROPERTIES_HPP
#define GAME_GAMEPROPERTIES_HPP

#include <color/color.hpp>
#include <color/palette.hpp>
#include <vector.hpp>
#include <string>

class GP {
public:
    GP() = delete;

    static std::string GameName() { return "MyAwesomeGame"; }

    static std::string AuthorName() { return "TODO"; }

    static std::string JamName() { return "TODO"; }

    static std::string JamDate() { return "TODO"; }

    static std::string ExplanationText()
    {
        return "Win the game\n[W,A,S,D] to move \n[Space] to jump\n[M/U] to mute/unmute audio";
    }

    static jt::Vector2f GetWindowSize() { return jt::Vector2f { 1280, 960 }; }

    static float GetZoom() { return 4.0f; }

    static jt::Vector2f GetScreenSize() { return GetWindowSize() * (1.0f / GetZoom()); }

    static jt::Color PaletteBackground() { return GP::getPalette().getColor(4); }

    static jt::Color PaletteFontFront() { return GP::getPalette().getColor(0); }

    static jt::Color PalleteFrontHighlight() { return GP::getPalette().getColor(1); }

    static jt::Color PaletteFontShadow() { return GP::getPalette().getColor(2); }

    static jt::Color PaletteFontCredits() { return GP::getPalette().getColor(1); }

    static jt::Palette getPalette();

    static int PhysicVelocityIterations();
    static int PhysicPositionIterations();
    static jt::Vector2f PlayerSize();
    static int TileSizeInPixel();
    static float HarborInteractionTimerMax();

    static inline constexpr float playerForwardStrength = 100.0f;
    static inline constexpr float playerRotationStrength = 150.0f;
    static inline constexpr float playerBreakMultiplier = 0.99f;
    static inline constexpr float playerBoostFactor = 1.5f;
    static inline constexpr float monkeyForwardStrength = 80.0f;
    static inline constexpr float monkeyChaseDistance = 125.0f;
    static inline constexpr float monkeyRotationSpeed = 100.0f;
    static inline constexpr float cameraDragDistance = 64.0f;
    static inline constexpr float cameraSmoothFactor = 0.025f;
    static float ScreenSizeScrollBound();
};

#endif
