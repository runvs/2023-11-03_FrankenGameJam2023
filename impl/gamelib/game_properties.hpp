#ifndef GAME_GAMEPROPERTIES_HPP
#define GAME_GAMEPROPERTIES_HPP

#include <color/color.hpp>
#include <color/palette.hpp>
#include <vector.hpp>
#include <string>

class GP {
public:
    GP() = delete;

    static std::string GameName() { return "   FruitLovin\nMonkeyPirates"; }

    static std::string AuthorName() { return "Adkiem, BloodyOrange,\nJacudibu, Laguna"; }

    static std::string JamName() { return "FGJ2023"; }

    static std::string JamDate() { return "2023-11"; }

    static std::string ExplanationText()
    {
        return "Take and give Fruits\n[W,A,S,D] to move \n[Shift] to boost\n[M/U] to mute/unmute "
               "audio";
    }

    static jt::Vector2f GetWindowSize() { return jt::Vector2f { 1280, 960 }; }

    static float GetZoom() { return 4.0f; }

    static jt::Vector2f GetScreenSize() { return GetWindowSize() * (1.0f / GetZoom()); }

    static jt::Color PaletteBackground() { return GP::getPalette().getColor(4); }

    static jt::Color PaletteFontFront() { return jt::Color { 235, 211, 32, 255 }; }

    static jt::Color PalleteFrontHighlight() { return jt::Color { 187, 16, 48, 255 }; }

    static jt::Color PaletteFontShadow() { return GP::getPalette().getColor(2); }

    static jt::Color PaletteFontCredits() { return GP::getPalette().getColor(1); }

    static jt::Palette getPalette();

    static int PhysicVelocityIterations();
    static int PhysicPositionIterations();
    static jt::Vector2f PlayerSize();
    static int TileSizeInPixel();
    static float ScreenSizeScrollBound();

    static inline constexpr auto playerForwardStrength = 100.0f;
    static inline constexpr auto playerRotationStrengthAtMinSpeed = 175.0f;
    static inline constexpr auto playerRotationStrengthAtMaxSpeed = 100.0f;
    static inline constexpr auto playerRotationStrengthAtBoost = 75.0f;
    static inline constexpr auto playerBreakMultiplier = 0.99f;
    static inline constexpr auto playerBoostFactor = 1.5f;
    static inline constexpr auto monkeyForwardStrength = 80.0f;
    static inline constexpr auto monkeyChaseDistance = 125.0f;
    static inline constexpr auto monkeyRotationSpeed = 100.0f;
    static inline constexpr auto cameraDragDistance = 64.0f;
    static inline constexpr auto cameraSmoothFactor = 0.025f;

    static inline constexpr auto playerMaxVelocity = 65.0f;
    static inline constexpr auto playerMaxVelocityWhenBoosting = 140.0f;

    static inline constexpr auto ZLayerBoat = 10;
    static inline constexpr auto ZLayerClouds = 15;
    static inline constexpr auto ZLayerHUD = 20;
};

#endif
