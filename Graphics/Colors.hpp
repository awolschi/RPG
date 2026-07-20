#ifndef COLORS_HPP
#define COLORS_HPP

#include "raylib.h"
#include "../Characters/Classes/Classes.hpp"

namespace CQColors
{

// Backgrounds
const Color BgDark      = { 26, 15, 10, 255 };
const Color BgPanel     = { 46, 30, 20, 255 };
const Color BgInput     = { 35, 22, 15, 255 };
const Color Parchment   = { 212, 196, 160, 255 };

// Accent / Gold
const Color Gold        = { 201, 168, 76, 255 };
const Color GoldDim     = { 160, 130, 50, 255 };
const Color GoldBright  = { 230, 200, 100, 255 };

// Buttons
const Color BtnBg       = { 80, 55, 35, 255 };
const Color BtnHover    = { 110, 75, 50, 255 };
const Color BtnActive   = { 60, 40, 25, 255 };
const Color BtnText     = { 212, 196, 160, 255 };
const Color BtnBorder   = { 139, 111, 71, 255 };

// Bars
const Color HpFg        = { 192, 57, 43, 255 };
const Color HpBg        = { 80, 30, 25, 255 };
const Color ManaFg      = { 52, 152, 219, 255 };
const Color ManaBg      = { 20, 50, 80, 255 };
const Color XpFg        = { 241, 196, 15, 255 };
const Color XpBg        = { 60, 50, 20, 255 };
const Color RepFg       = { 155, 89, 182, 255 };
const Color RepBg       = { 40, 25, 50, 255 };

// Text
const Color TextLight   = { 230, 220, 200, 255 };
const Color TextDark    = { 42, 26, 10, 255 };
const Color TextDim     = { 160, 140, 120, 255 };
const Color TextRed     = { 220, 80, 60, 255 };
const Color TextGreen   = { 80, 200, 80, 255 };
const Color TextGold    = { 201, 168, 76, 255 };

// Special
const Color BorderLight = { 160, 130, 90, 255 };
const Color LogBg       = { 20, 12, 8, 200 };
const Color CardBg      = { 55, 38, 25, 255 };
const Color FocusBorder = { 230, 200, 100, 255 };

// Class-specific skill card tints
inline Color SkillCardBg(CharacterClass cc)
{
    switch (cc)
    {
        case CharacterClass::Warrior:  return { 80, 30, 25, 255 };
        case CharacterClass::Priest:   return { 80, 70, 40, 255 };
        case CharacterClass::Mage:     return { 25, 30, 70, 255 };
        case CharacterClass::Archer:   return { 25, 50, 30, 255 };
        case CharacterClass::Merchant: return { 70, 60, 25, 255 };
    }
    return CardBg;
}

}

#endif
