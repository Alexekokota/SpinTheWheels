#include <raylib.h>
#include "wheel.h"
#include <iostream>
#include "DifficultyLevels.h"
#include "utils.h"

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 1024;

    InitWindow(screenWidth, screenHeight, "Spin the Wheel");
    InitAudioDevice();
    SetTargetFPS(60);

    // Load font once
    Font font = LoadFontEx("arial.ttf", 32, 0, 250); // Load with a reasonable size and glyph count
    if (font.texture.id == 0) {
        std::cerr << "Failed to load font!" << std::endl;
    } else {
        std::cout << "Font loaded successfully!" << std::endl;
    }

    Sound spinSound = LoadSound("Spin.wav");
    Sound spinEnd = LoadSound("SpinEnd.wav");

    // Initialize difficulty settings
    DifficultySettings difficultySettings;
    DifficultyLevel currentDifficulty = HARD; // Set initial difficulty level
    Wheel wheel;
    std::string currentChallenge; // Store the current challenge
	
    ToggleFullscreen();
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_DOWN)) {
            currentChallenge.clear(); // Clear the current challenge at the start of the spin
            wheel.StartSpinning();
        }

        if (IsKeyPressed(KEY_UP)) {
            currentDifficulty = difficultySettings.GetNextDifficulty(currentDifficulty);
        }

        wheel.Update();

        BeginDrawing();
        ClearBackground(BLACK);
        
        // Draw the wheel first
        wheel.Draw(difficultySettings, currentDifficulty, font);

        // Draw the current challenge if it has been set
        if (!currentChallenge.empty()) {
            std::unordered_map<std::string, int> values = {
                {"X", difficultySettings.GetValue(currentDifficulty, "X")},
                {"Y", difficultySettings.GetValue(currentDifficulty, "Y")},
                {"Z", difficultySettings.GetValue(currentDifficulty, "Z")}
            };
            std::string challengeWithValues = ReplacePlaceholders(currentChallenge, values);
            float textWidth = MeasureTextEx(font, challengeWithValues.c_str(), 40, 1).x;
            float textHeight = MeasureTextEx(font, challengeWithValues.c_str(), 40, 1).y;

            // Check if text width exceeds available space, adjust font size if needed
            int fontSize = 40;
            while (textWidth > screenHeight - 100) {
                fontSize--; // Decrease font size until text fits within bounds
                textWidth = MeasureTextEx(font, challengeWithValues.c_str(), fontSize, 1).x;
                textHeight = MeasureTextEx(font, challengeWithValues.c_str(), fontSize, 1).y;
            }

            // Draw at the top center upside down
            Vector2 topCenterPos = {static_cast<float>(screenWidth / 2), 50};
            Vector2 originTopCenter = {textWidth / 2, textHeight / 2};

            DrawTextPro(font, challengeWithValues.c_str(), topCenterPos, originTopCenter, 180.0f, static_cast<float>(fontSize), 1.0f, WHITE); // 180 degrees (upside down)
            
            // Draw at the bottom centered
            DrawTextEx(font, challengeWithValues.c_str(), Vector2{static_cast<float>(screenWidth / 2 - textWidth / 2), static_cast<float>(screenHeight - 50 - textHeight)}, fontSize, 1, WHITE);

            // Calculate position for left text
            Vector2 leftPos = {50, static_cast<float>(screenHeight / 2)};

            // Calculate position for right text (aligned to right edge of the screen)
            Vector2 rightPos = {static_cast<float>(screenWidth - 50), static_cast<float>(screenHeight / 2 - textHeight / 2)};

            // Calculate origin for rotation (center of text)
            Vector2 origin = {textWidth / 2, textHeight / 2};

            // Draw at the left rotated
            DrawTextPro(font, challengeWithValues.c_str(), leftPos, origin, 90.0f, static_cast<float>(fontSize), 1.0f, WHITE); // -90 degrees (sideways)
            
            // Draw at the right rotated
            DrawTextPro(font, challengeWithValues.c_str(), rightPos, origin, -90.0f, static_cast<float>(fontSize), 1.0f, WHITE); // 90 degrees (sideways)
        }

        EndDrawing();

        if (!wheel.IsSpinning() && wheel.HasStopped()) {
            currentChallenge = wheel.GetCurrentChallenge(); // Update the current challenge
            wheel.ResetStopped();
        }
    }

    // Unload the font
    UnloadFont(font);
    UnloadSound(spinSound);
    UnloadSound(spinEnd);

    CloseWindow();
    return 0;
}
