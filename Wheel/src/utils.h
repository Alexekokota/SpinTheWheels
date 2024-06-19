#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <unordered_map>

std::vector<std::string> ReadChallengesFromFile(const std::string& filename);
std::string ReplacePlaceholders(const std::string& challenge, const std::unordered_map<std::string, int>& values);
void PrintCharacterCodes(const std::string& text); // Add this line

#endif // UTILS_H
