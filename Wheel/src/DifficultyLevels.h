#pragma once

#include <unordered_map>
#include <string>

enum DifficultyLevel {
    EASY,
    MEDIUM,
    HARD
};

class DifficultySettings {
public:
    DifficultySettings();
    int GetValue(DifficultyLevel level, const std::string& variable) const;
    DifficultyLevel GetNextDifficulty(DifficultyLevel current) const;

private:
    std::unordered_map<DifficultyLevel, std::unordered_map<std::string, int>> settings;
};
