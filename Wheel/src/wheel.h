#pragma once
#include <vector>
#include <string>
#include <raylib.h>
#include "DifficultyLevels.h"

struct Section {
    std::string name;
    std::string challenge; // Add challenge to section
    float startAngle;
    float endAngle;
    Color color;
};

class Wheel
{
public:
    Wheel();
    void Update();
    void Draw(const DifficultySettings& difficultySettings, DifficultyLevel currentDifficulty, Font font); // Pass font
    void StartSpinning();
    bool IsSpinning() const;
    std::string GetCurrentSection() const;
    std::string GetCurrentChallenge() const; // Add method to get current challenge
    bool HasStopped() const;
    void ResetStopped();

private:
    float x;
    float y;
    float rotation;
    float rotationSpeed;
    float deceleration;
    int radius;
    float spinDuration;
    bool spinning;
    bool stopped;
    Sound spinSound;
    Sound spinEnd;
    std::vector<Section> sections;
    std::vector<std::string> challenges;
};
