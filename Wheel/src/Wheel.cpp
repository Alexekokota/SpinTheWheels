#include "wheel.h"
#include <raylib.h>
#include <raymath.h>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include "utils.h"
#include <unordered_map>
#include "DifficultyLevels.h"
#include <rlgl.h>

Wheel::Wheel()
{
    const int screenHeight = 1024;
    x = 640.0f;
    y = static_cast<float>(screenHeight) / 2.0f;
    radius = 400;
    rotation = 0.0f;
    rotationSpeed = 0.0f;
    spinDuration = 0.0f;
    deceleration = 500.0f;
    spinning = false;
    stopped = false;
    challenges = ReadChallengesFromFile("challenges.txt");

    // Ensure that we have at least six challenges
    while (challenges.size() < 6)
    {
        challenges.insert(challenges.end(), challenges.begin(), challenges.end());
    }

    // Shuffle the list of challenges
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(challenges.begin(), challenges.end(), g);

    // Define sections with random challenges
    float sectionAngle = 360.0f / 6; // Divide the wheel into six equal sections
    Color purple1 = {127, 0, 225, 255}; // Darker purple
    Color purple2 = {195, 177, 255, 255}; // Lighter purple
    for (int i = 0; i < 6; ++i)
    {
        // Alternate between two shades of purple
        Color sectionColor = (i % 2 == 0) ? purple1 : purple2;

        // Assign the next challenge to the current section
        sections.push_back({std::to_string(i + 1), challenges[i], i * sectionAngle, (i + 1) * sectionAngle, sectionColor});
    }

    spinSound = LoadSound("Spin.wav");
    spinEnd = LoadSound("SpinEnd.wav");
}

void Wheel::Update()
{
    if (spinning)
    {
        rotation += rotationSpeed * GetFrameTime();
        if (rotation >= 360.0f) rotation -= 360.0f;

        spinDuration -= GetFrameTime();
        if (spinDuration <= 0.0f)
        {
            spinDuration = 0.0f;
            rotationSpeed -= deceleration * GetFrameTime();
            if (rotationSpeed <= 0.0f)
            {
                rotationSpeed = 0.0f;
                spinning = false;
                stopped = true;
                PlaySound(spinEnd);
            }
        }
    }
}

std::string WrapText(const std::string& text, int maxWidth)
{

    const int fontSize = 20;
    std::istringstream words(text);
    std::ostringstream wrapped;
    std::string word;
    float spaceWidth = MeasureText(" ", fontSize);

    float lineWidth = 0;
    while (words >> word)
    {
        float wordWidth = MeasureText(word.c_str(), fontSize);
        if (lineWidth + wordWidth >= maxWidth)
        {
            wrapped << '\n';
            lineWidth = 0;
        }
        if (lineWidth > 0)
        {
            wrapped << ' ';
            lineWidth += spaceWidth;
        }
        wrapped << word;
        lineWidth += wordWidth;
    }
    return wrapped.str();
}

void Wheel::Draw(const DifficultySettings& difficultySettings, DifficultyLevel currentDifficulty, Font font)
{
    // Draw sections
    for (const Section& section : sections)
    {
        DrawCircleSector(Vector2{x, y}, radius, section.startAngle, section.endAngle, 0, section.color);

        // Calculate the center angle of the section
        float centerAngle = (section.startAngle + section.endAngle) / 2;

        // Convert the center angle to radians for calculations
        float centerAngleRad = centerAngle * DEG2RAD;

        // Calculate the position of the text closer to the outer edge
        float textX = x + (radius * 0.69f) * cos(centerAngleRad); // Adjusted radius multiplier
        float textY = y + (radius * 0.69f) * sin(centerAngleRad); // Adjusted radius multiplier

        // Calculate the maximum width for the challenge text to fit inside the section
        float sectionWidth = radius * sin((section.endAngle - section.startAngle) * DEG2RAD);
        int maxTextWidth = static_cast<int>(sectionWidth * 0.8f); // Adjust the factor as needed

        // Calculate font size based on section width (adjust as needed)
        int fontSize = static_cast<int>(sectionWidth / 18.0f); // Adjust divisor for desired font size

        // Get the challenge text for this section
        std::string challengeText = section.challenge;

        // Replace "{X}" with actual values based on difficulty level
        size_t xPos = challengeText.find("{X}");
        if (xPos != std::string::npos) {
            int valueX = difficultySettings.GetValue(currentDifficulty, "X");
            challengeText.replace(xPos, 3, std::to_string(valueX));
        }

        // Replace "{Y}" with actual value based on difficulty level
        size_t yPos = challengeText.find("{Y}");
        if (yPos != std::string::npos) {
            int valueY = difficultySettings.GetValue(currentDifficulty, "Y");
            challengeText.replace(yPos, 3, std::to_string(valueY));
        }

        // Replace "{Z}" with actual value based on difficulty level
        size_t zPos = challengeText.find("{Z}");
        if (zPos != std::string::npos) {
            int valueZ = difficultySettings.GetValue(currentDifficulty, "Z");
            challengeText.replace(zPos, 3, std::to_string(valueZ));
        }

        // Replace all occurrences of "{APOSTROPHE}" with "'"
        const std::string toReplace = "{APOSTROPHE}";
        const std::string replacement = "'";
        size_t apoPos = challengeText.find(toReplace);
        while (apoPos != std::string::npos) {
            challengeText.replace(apoPos, toReplace.length(), replacement);
            apoPos = challengeText.find(toReplace, apoPos + replacement.length());
        }

        // Wrap text
        std::string wrappedText = WrapText(challengeText, maxTextWidth);

        // Split wrapped text into lines
        std::vector<std::string> lines;
        std::istringstream wrappedStream(wrappedText);
        std::string line;
        while (std::getline(wrappedStream, line))
        {
            lines.push_back(line);
        }

        // Calculate the total height of the text block after wrapping
        float totalTextHeight = lines.size() * (fontSize + 2);

        // Determine if the section is in the bottom half of the wheel
        bool isBottomSection = (centerAngle > 180.0f && centerAngle < 360.0f);

        if (isBottomSection)
        {
            // Save current transform matrix
            rlPushMatrix();

            // Translate to text position and rotate 180 degrees around the center of the text block
            rlTranslatef(textX, textY, 0);
            rlRotatef(180, 0, 0, 1);
            rlTranslatef(-textX + 10, -textY, 0);

            // Adjust textY to center vertically
            textY += totalTextHeight / 2 - 50;
        }
        else
        {
            // Adjust textY to center vertically
            textY -= totalTextHeight / 2;
        }

        // Draw each line of text
        for (size_t i = 0; i < lines.size(); ++i)
        {
            // Calculate the position to center the text horizontally
            float textWidth = MeasureText(lines[i].c_str(), fontSize);
            float lineX = textX - textWidth / 2; // Center horizontally
            float lineY = textY + i * (fontSize + 2);

            // Draw the text line
            DrawTextEx(font, lines[i].c_str(), Vector2{lineX, lineY}, fontSize, 1, BLACK);
        }

        if (isBottomSection)
        {
            // Restore previous transform matrix
            rlPopMatrix();
        }
    }

    // Calculate the endpoint of the line indicating rotation direction
    float endX = x + radius * cos(rotation * DEG2RAD);
    float endY = y + radius * sin(rotation * DEG2RAD);

    // Draw the line indicating rotation direction as part of the wheel
    DrawLine(x, y, endX, endY, BLACK);

    // Draw the wheel border
    DrawCircleSectorLines(Vector2{x, y}, radius, 0, 360, 360, BLACK);

    // Draw center
    DrawCircle(x, y, 4, RED);
}

void Wheel::StartSpinning()
{
    if (!spinning)
    {
        PlaySound(spinSound);

        spinning = true;
        stopped = false;
        rotationSpeed = 1000.0f;
        spinDuration = static_cast<float>(GetRandomValue(2, 6));

        // Find the current section based on the rotation angle
        std::string currentSection = GetCurrentSection();
        int currentSectionIndex = -1;

        for (size_t i = 0; i < sections.size(); ++i)
        {
            if (sections[i].name == currentSection)
            {
                currentSectionIndex = static_cast<int>(i);
                break;
            }
        }

        // Ensure we found a valid section index
        if (currentSectionIndex != -1)
        {
            // Seed the random number generator
            srand(static_cast<unsigned int>(time(nullptr)));

            // Get a new random challenge for the chosen section
            size_t j = rand() % challenges.size();
            sections[currentSectionIndex].challenge = challenges[j];
        }
    }
}



bool Wheel::IsSpinning() const
{
    return spinning;
}

std::string Wheel::GetCurrentSection() const
{
    std::cout << "Rotation: " << rotation << std::endl;
    for (const Section& section : sections)
    {
        std::cout << "Section: " << section.name << ", Start: " << section.startAngle << ", End: " << section.endAngle << std::endl;
        if (rotation >= section.startAngle && rotation < section.endAngle)
        {
            return section.name;
        }
    }
    return "Unknown";
}

std::string Wheel::GetCurrentChallenge() const
{
    for (const Section& section : sections)
    {
        if (rotation >= section.startAngle && rotation < section.endAngle)
        {
            return section.challenge;
        }
    }
    return "Unknown";
}

bool Wheel::HasStopped() const
{
    StopSound(spinSound);
    return stopped;
}

void Wheel::ResetStopped()
{
    stopped = false;
}