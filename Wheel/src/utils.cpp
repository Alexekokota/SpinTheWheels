#include "utils.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <locale>
#include <codecvt>

std::vector<std::string> ReadChallengesFromFile(const std::string& filename)
{
    std::wifstream file(filename);
    file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t>));
    std::vector<std::string> challenges;
    std::wstring wchallenge;

    if (file.is_open())
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        while (std::getline(file, wchallenge))
        {
            std::string challenge = converter.to_bytes(wchallenge);
            std::string replaced_challenge;
            for (char c : challenge) {
                if (c == '\'') {
                    replaced_challenge += "{O}"; // Replace apostrophe with {O}
                } else {
                    replaced_challenge += c; // Keep the character as it is
                }
            }
            challenges.push_back(replaced_challenge);

            // Debug: Print character codes of the challenge
            PrintCharacterCodes(replaced_challenge);
        }
        file.close();
    }
    else
    {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
    }

    return challenges;
}


void PrintCharacterCodes(const std::string& text) {
    std::cout << "Original string: " << text << std::endl;
    for (char c : text) {
        std::cout << "Character: " << c << ", Code: " << static_cast<int>(c) << std::endl;
    }
}

std::string ReplacePlaceholders(const std::string& challenge, const std::unordered_map<std::string, int>& values) {
    std::string result = challenge;
    const std::string toReplace = "{APOSTROPHE}";
    const std::string replacement = "'";

    size_t pos = result.find(toReplace);
    while (pos != std::string::npos) {
        result.replace(pos, toReplace.length(), replacement);
        pos = result.find(toReplace, pos + replacement.length());
    }

    // Perform other replacements based on values map
    for (const auto& pair : values) {
        std::string placeholder = "{" + pair.first + "}";
        pos = result.find(placeholder);
        while (pos != std::string::npos) {
            result.replace(pos, placeholder.length(), std::to_string(pair.second));
            pos = result.find(placeholder, pos + std::to_string(pair.second).length());
        }
    }

    return result;
}







