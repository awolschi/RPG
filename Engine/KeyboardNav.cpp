#include "KeyboardNav.hpp"
#include "../Graphics/Colors.hpp"
#include <algorithm>
#include <cmath>

KeyboardNav::KeyboardNav()
    : focusIndex(0), focusCount(0), lastMoveTime(0.0)
{
}

void KeyboardNav::Reset()
{
    focusIndex = 0;
    focusCount = 0;
    lastMoveTime = 0.0;
}

void KeyboardNav::SetFocusCount(int count)
{
    focusCount = count;
    if (focusIndex >= focusCount)
        focusIndex = std::max(0, focusCount - 1);
}

int KeyboardNav::GetFocus() const
{
    return focusIndex;
}

void KeyboardNav::SetFocus(int index)
{
    focusIndex = index;
}

bool KeyboardNav::IsFocused(int index) const
{
    return focusIndex == index;
}

int KeyboardNav::Update()
{
    if (focusCount <= 0)
        return -1;

    double now = ::GetTime();
    bool canMove = (now - lastMoveTime) >= COOLDOWN;

    // Confirm
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
    {
        return focusIndex;
    }

    // Back
    if (IsKeyPressed(KEY_ESCAPE))
    {
        return -3;
    }

    // Navigation
    if (canMove)
    {
        bool moved = false;

        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_RIGHT))
        {
            focusIndex++;
            if (focusIndex >= focusCount)
                focusIndex = 0;
            moved = true;
        }
        else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_LEFT))
        {
            focusIndex--;
            if (focusIndex < 0)
                focusIndex = focusCount - 1;
            moved = true;
        }
        else if (IsKeyPressed(KEY_TAB))
        {
            if (IsKeyDown(KEY_LEFT_SHIFT))
            {
                focusIndex--;
                if (focusIndex < 0)
                    focusIndex = focusCount - 1;
            }
            else
            {
                focusIndex++;
                if (focusIndex >= focusCount)
                    focusIndex = 0;
            }
            moved = true;
        }

        if (moved)
            lastMoveTime = now;
    }

    return -1;
}

void KeyboardNav::DrawFocusRect(int x, int y, int w, int h) const
{
    DrawFocusRect(x, y, w, h, CQColors::TextGold);
}

void KeyboardNav::DrawFocusRect(int x, int y, int w, int h, Color tint) const
{
    // Draw a pulsing golden border to indicate focus
    unsigned char alpha = static_cast<unsigned char>(
        180 + 75.0 * std::sin(::GetTime() * 4.0));
    Color focusColor = tint;
    focusColor.a = alpha;

    for (int i = 0; i < 2; i++)
    {
        DrawRectangleLines(x - i, y - i, w + i * 2, h + i * 2, focusColor);
    }
}
