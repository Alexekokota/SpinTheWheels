// DifficultyLevels.cpp
#include "DifficultyLevels.h"

DifficultySettings::DifficultySettings() {
    // Define values for each difficulty level
    settings[EASY] = {{"X", 1}, {"Y", 2}, {"Z", 3}};
    settings[MEDIUM] = {{"X", 2}, {"Y", 4}, {"Z", 5}};
    settings[HARD] = {{"X", 4}, {"Y", 7}, {"Z", 9}};
}

int DifficultySettings::GetValue(DifficultyLevel level, const std::string& variable) const {
    return settings.at(level).at(variable);
}

DifficultyLevel DifficultySettings::GetNextDifficulty(DifficultyLevel current) const {
    switch (current) {
        case EASY:
            return MEDIUM;
        case MEDIUM:
            return HARD;
        case HARD:
            return EASY;
        default:
            return EASY; // Default to EASY if invalid current level
    }
}
