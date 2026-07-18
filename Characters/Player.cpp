#include "Player.hpp"
#include <iostream>
#include <algorithm>
#include "../Skills/CommonSkills/CommonAttack.hpp"
#include "../Skills/ClassSkills/Warrior/Warrior.hpp"
#include "../Skills/ClassSkills/Mage/Mage.hpp"
#include "../Skills/ClassSkills/Priest/Priest.hpp"
#include "../Skills/ClassSkills/Archer/Archer.hpp"
#include "../Skills/ClassSkills/Merchant/Merchant.hpp"

Player::Player(const std::string& name, CharacterClass characterClass, CharacterRace race)
    : Character(name, ClassDatabase::Get(characterClass).startingStats + RaceDatabase::Get(race).statModifier),
      characterClass(characterClass), race(race),
      inventory()
{
    skills.AddSkill(std::make_shared<CommonAttack>());
    InitializeClassSkills();
    RebuildLoadout();

    jobSystem.AddJob(JobType::Mining);
    jobSystem.AddJob(JobType::Lumberjacking);
    jobSystem.AddJob(JobType::Fishing);
    jobSystem.AddJob(JobType::Smithing);
}

void Player::LevelUp()
{
    Character::LevelUp();

    ClassData classData = ClassDatabase::Get(characterClass);
    stats += classData.levelUpGain;

    currentHealth = GetMaxHealth();
    currentMana = GetMaxMana();

    CheckNewSkills();
}

void Player::InitializeClassSkills()
{
    switch (characterClass)
    {
        case CharacterClass::Warrior:
            {
                auto ps = std::make_shared<PowerStrike>(); ps->characterClass = CharacterClass::Warrior; skills.AddSkill(ps);
                auto ww = std::make_shared<Whirlwind>(); ww->characterClass = CharacterClass::Warrior; skills.AddSkill(ww);
                auto ds = std::make_shared<DefensiveStance>(); ds->characterClass = CharacterClass::Warrior; skills.AddSkillIfLevelMet(ds, level);
                auto wc = std::make_shared<WarCry>(); wc->characterClass = CharacterClass::Warrior; skills.AddSkillIfLevelMet(wc, level);
                auto sb = std::make_shared<ShieldBash>(); sb->characterClass = CharacterClass::Warrior; skills.AddSkillIfLevelMet(sb, level);
                auto bc = std::make_shared<BattleCry>(); bc->characterClass = CharacterClass::Warrior; skills.AddSkillIfLevelMet(bc, level);
                auto ex = std::make_shared<Execute>(); ex->characterClass = CharacterClass::Warrior; skills.AddSkillIfLevelMet(ex, level);
                auto ws = std::make_shared<WarStomp>(); ws->characterClass = CharacterClass::Warrior; skills.AddSkillIfLevelMet(ws, level);
            }
            break;

        case CharacterClass::Mage:
            {
                auto fb = std::make_shared<Fireball>(); fb->characterClass = CharacterClass::Mage; skills.AddSkill(fb);
                auto ib = std::make_shared<IceBolt>(); ib->characterClass = CharacterClass::Mage; skills.AddSkill(ib);
                auto ab = std::make_shared<ArcaneBolt>(); ab->characterClass = CharacterClass::Mage; skills.AddSkillIfLevelMet(ab, level);
                auto mt = std::make_shared<Meteor>(); mt->characterClass = CharacterClass::Mage; skills.AddSkillIfLevelMet(mt, level);
                auto fw = std::make_shared<FrostWard>(); fw->characterClass = CharacterClass::Mage; skills.AddSkillIfLevelMet(fw, level);
                auto as = std::make_shared<ArcaneSurge>(); as->characterClass = CharacterClass::Mage; skills.AddSkillIfLevelMet(as, level);
                auto ms = std::make_shared<ManaSiphon>(); ms->characterClass = CharacterClass::Mage; skills.AddSkillIfLevelMet(ms, level);
                auto cl = std::make_shared<ChainLightning>(); cl->characterClass = CharacterClass::Mage; skills.AddSkillIfLevelMet(cl, level);
                auto lb = std::make_shared<LightningBolt>(); lb->characterClass = CharacterClass::Mage; skills.AddSkillIfLevelMet(lb, level);
                auto am = std::make_shared<ArcaneMissiles>(); am->characterClass = CharacterClass::Mage; skills.AddSkillIfLevelMet(am, level);
                auto fs = std::make_shared<FrostShield>(); fs->characterClass = CharacterClass::Mage; skills.AddSkillIfLevelMet(fs, level);
                auto mb = std::make_shared<ManaBomb>(); mb->characterClass = CharacterClass::Mage; skills.AddSkillIfLevelMet(mb, level);
                auto bo = std::make_shared<BlazingOrb>(); bo->characterClass = CharacterClass::Mage; skills.AddSkillIfLevelMet(bo, level);
                auto fn = std::make_shared<FrostNova>(); fn->characterClass = CharacterClass::Mage; skills.AddSkillIfLevelMet(fn, level);
                auto abr = std::make_shared<ArcaneBarrage>(); abr->characterClass = CharacterClass::Mage; skills.AddSkillIfLevelMet(abr, level);
                auto tw = std::make_shared<TimeWarp>(); tw->characterClass = CharacterClass::Mage; skills.AddSkillIfLevelMet(tw, level);
                auto gs = std::make_shared<GlacialSpike>(); gs->characterClass = CharacterClass::Mage; skills.AddSkillIfLevelMet(gs, level);
                auto ms2 = std::make_shared<ManaShield>(); ms2->characterClass = CharacterClass::Mage; skills.AddSkillIfLevelMet(ms2, level);
                auto pb = std::make_shared<Pyroblast>(); pb->characterClass = CharacterClass::Mage; skills.AddSkillIfLevelMet(pb, level);
                auto bl = std::make_shared<Blizzard>(); bl->characterClass = CharacterClass::Mage; skills.AddSkillIfLevelMet(bl, level);
                auto ap = std::make_shared<ArcanePower>(); ap->characterClass = CharacterClass::Mage; skills.AddSkillIfLevelMet(ap, level);
                auto il = std::make_shared<IceLance>(); il->characterClass = CharacterClass::Mage; skills.AddSkillIfLevelMet(il, level);
                auto inf = std::make_shared<Inferno>(); inf->characterClass = CharacterClass::Mage; skills.AddSkillIfLevelMet(inf, level);
                auto ef = std::make_shared<ElementalFury>(); ef->characterClass = CharacterClass::Mage; skills.AddSkillIfLevelMet(ef, level);
                auto ac = std::make_shared<ArchmageCataclysm>(); ac->characterClass = CharacterClass::Mage; skills.AddSkillIfLevelMet(ac, level);
            }
            break;

        case CharacterClass::Priest:
            {
                auto hs = std::make_shared<HolySmite>(); hs->characterClass = CharacterClass::Priest; skills.AddSkill(hs);
                auto hl = std::make_shared<Heal>(); hl->characterClass = CharacterClass::Priest; skills.AddSkill(hl);
                auto mh = std::make_shared<MassHeal>(); mh->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(mh, level);
                auto ds = std::make_shared<DivineShield>(); ds->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(ds, level);
                auto sm = std::make_shared<Smite>(); sm->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(sm, level);
                auto rn = std::make_shared<Renew>(); rn->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(rn, level);
                auto hn = std::make_shared<HolyNova>(); hn->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(hn, level);
                auto dw = std::make_shared<DivineWrath>(); dw->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(dw, level);
            }
            break;

        case CharacterClass::Archer:
            {
                auto ps = std::make_shared<PiercingShot>(); ps->characterClass = CharacterClass::Archer; skills.AddSkill(ps);
                auto ms = std::make_shared<MultiShot>(); ms->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(ms, level);
                auto ra = std::make_shared<RainOfArrows>(); ra->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(ra, level);
                auto qs = std::make_shared<QuickShot>(); qs->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(qs, level);
                auto sn = std::make_shared<Snipe>(); sn->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(sn, level);
                auto ast = std::make_shared<ArrowStorm>(); ast->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(ast, level);
                auto dm = std::make_shared<DeathMark>(); dm->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(dm, level);
            }
            break;

        case CharacterClass::Merchant:
            {
                auto tc = std::make_shared<ThrowCoin>(); tc->characterClass = CharacterClass::Merchant; skills.AddSkill(tc);
                auto ap = std::make_shared<Appraise>(); ap->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(ap, level);
                auto hg = std::make_shared<Haggle>(); hg->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(hg, level);
                auto br = std::make_shared<Bribery>(); br->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(br, level);
                auto iv = std::make_shared<Invest>(); iv->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(iv, level);
                auto cs = std::make_shared<CoinStorm>(); cs->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(cs, level);
                auto gh = std::make_shared<GoldenHurricane>(); gh->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(gh, level);
            }
            break;

        default:
            break;
    }
}

bool Player::CanEquip(const std::shared_ptr<Item>& item) const
{
    if (!item) return false;

    ClassData classData = ClassDatabase::Get(characterClass);

    if (auto weapon = std::dynamic_pointer_cast<Weapon>(item))
    {
        for (auto wt : classData.allowedWeaponTypes)
        {
            if (wt == weapon->weaponType) return true;
        }
        return false;
    }
    else if (auto armor = std::dynamic_pointer_cast<Armor>(item))
    {
        for (auto at : classData.allowedArmorTypes)
        {
            if (at == armor->armorType) return true;
        }
        return false;
    }
    else if (auto oh = std::dynamic_pointer_cast<Offhand>(item))
    {
        switch (oh->offhandType)
        {
            case OffhandType::Shield:
                return characterClass == CharacterClass::Warrior || characterClass == CharacterClass::Archer;
            case OffhandType::Orb:
            case OffhandType::Book:
                return characterClass == CharacterClass::Mage || characterClass == CharacterClass::Priest;
            case OffhandType::Bag:
                return true;
        }
        return true;
    }
    // Accessories and consumables can be used by anyone
    return true;
}

bool Player::EquipItem(std::shared_ptr<Item> item)
{
    if (!item) return false;

    int idx = inventory.GetItemIndex(item);
    if (idx < 0) return false;

    if (!CanEquip(item)) return false;

    if (auto oh = std::dynamic_pointer_cast<Offhand>(item))
    {
        int oldMaxHP = GetMaxHealth();
        int oldMaxMP = GetMaxMana();

        if (equipment.offhand)
            inventory.AddItem(equipment.offhand);
        equipment.offhand = oh->Clone();
        inventory.RemoveOneItem(idx);

        int hpDiff = GetMaxHealth() - oldMaxHP;
        int mpDiff = GetMaxMana() - oldMaxMP;
        if (hpDiff > 0) currentHealth += hpDiff;
        if (currentHealth > GetMaxHealth()) currentHealth = GetMaxHealth();
        if (mpDiff > 0) currentMana += mpDiff;
        if (currentMana > GetMaxMana()) currentMana = GetMaxMana();

        return true;
    }
    else if (auto weapon = std::dynamic_pointer_cast<Weapon>(item))
    {
        if (!equipment.weapon)
        {
            equipment.weapon = std::dynamic_pointer_cast<Weapon>(item->Clone());
        }
        else if (!equipment.offhand)
        {
            equipment.offhand = std::dynamic_pointer_cast<Weapon>(item->Clone());
        }
        else
        {
            if (equipment.offhand)
                inventory.AddItem(equipment.offhand);
            equipment.offhand = std::dynamic_pointer_cast<Weapon>(item->Clone());
        }
        inventory.RemoveOneItem(idx);
        return true;
    }
    else if (auto armor = std::dynamic_pointer_cast<Armor>(item))
    {
        std::shared_ptr<Armor>* slot = nullptr;
        switch (armor->piece)
        {
            case ArmorPiece::Helmet: slot = &equipment.helmet; break;
            case ArmorPiece::Chest:  slot = &equipment.chest;  break;
            case ArmorPiece::Gloves: slot = &equipment.gloves; break;
            case ArmorPiece::Pants:  slot = &equipment.pants;  break;
            case ArmorPiece::Boots:  slot = &equipment.boots;  break;
        }
        if (slot)
        {
            if (*slot) inventory.AddItem(*slot);
            *slot = std::dynamic_pointer_cast<Armor>(item->Clone());
            inventory.RemoveOneItem(idx);
            return true;
        }
    }
    else if (auto accessory = std::dynamic_pointer_cast<Accessory>(item))
    {
        int oldMaxHP = GetMaxHealth();
        int oldMaxMP = GetMaxMana();

        if (!equipment.ring1)
            equipment.ring1 = std::dynamic_pointer_cast<Accessory>(item->Clone());
        else if (!equipment.ring2)
            equipment.ring2 = std::dynamic_pointer_cast<Accessory>(item->Clone());
        else
        {
            if (equipment.amulet)
                inventory.AddItem(equipment.amulet);
            equipment.amulet = std::dynamic_pointer_cast<Accessory>(item->Clone());
        }
        inventory.RemoveOneItem(idx);

        // Apply HP/MP bonus from new accessory
        int hpDiff = GetMaxHealth() - oldMaxHP;
        int mpDiff = GetMaxMana() - oldMaxMP;
        if (hpDiff > 0) currentHealth += hpDiff;
        if (currentHealth > GetMaxHealth()) currentHealth = GetMaxHealth();
        if (mpDiff > 0) currentMana += mpDiff;
        if (currentMana > GetMaxMana()) currentMana = GetMaxMana();

        return true;
    }
    return false;
}

bool Player::UnequipItem(const std::string& slotName)
{
    std::shared_ptr<Item> old = nullptr;
    if (slotName == "weapon") { old = equipment.weapon; equipment.weapon = nullptr; }
    else if (slotName == "offhand") { old = equipment.offhand; equipment.offhand = nullptr; }
    else if (slotName == "helmet") { old = equipment.helmet; equipment.helmet = nullptr; }
    else if (slotName == "chest") { old = equipment.chest; equipment.chest = nullptr; }
    else if (slotName == "gloves") { old = equipment.gloves; equipment.gloves = nullptr; }
    else if (slotName == "pants") { old = equipment.pants; equipment.pants = nullptr; }
    else if (slotName == "boots") { old = equipment.boots; equipment.boots = nullptr; }
    else if (slotName == "ring1") { old = equipment.ring1; equipment.ring1 = nullptr; }
    else if (slotName == "ring2") { old = equipment.ring2; equipment.ring2 = nullptr; }
    else if (slotName == "amulet") { old = equipment.amulet; equipment.amulet = nullptr; }

    if (old)
    {
        if (inventory.AddItem(old))
        {
            currentHealth = std::min(currentHealth, GetMaxHealth());
            currentMana = std::min(currentMana, GetMaxMana());
            return true;
        }
    }
    return false;
}

void Player::ListEquipment() const
{
    std::cout << "\n=== EQUIPMENT ===\n";
    auto print = [](const std::string& slot, const auto& item)
    {
        std::cout << slot << ": ";
        if (item)
        {
            std::cout << item->name;
            if (auto oh = std::dynamic_pointer_cast<Offhand>(item))
                std::cout << " (DEF:" << oh->defense << " Mana:" << oh->manaBonus << ")";
            else if (auto w = std::dynamic_pointer_cast<Weapon>(item))
                std::cout << " (DMG:" << w->damage << " Mana:" << w->manaCost << ")";
            else if (auto a = std::dynamic_pointer_cast<Armor>(item))
                std::cout << " (DEF:" << a->defense << ")";
            else if (auto ac = std::dynamic_pointer_cast<Accessory>(item))
                std::cout << " (HP:" << ac->bonusHealth << " Mana:" << ac->bonusMana << ")";
        }
        else
            std::cout << "(empty)";
        std::cout << "\n";
    };
    print("Weapon", equipment.weapon);
    print("Offhand", equipment.offhand);
    print("Helmet", equipment.helmet);
    print("Chest", equipment.chest);
    print("Gloves", equipment.gloves);
    print("Pants", equipment.pants);
    print("Boots", equipment.boots);
    print("Ring 1", equipment.ring1);
    print("Ring 2", equipment.ring2);
    print("Amulet", equipment.amulet);
    std::cout << "Total Defense Bonus: " << equipment.GetTotalDefense() << "\n";

    int totalHP = 0, totalMana = 0;
    auto addStats = [&](const auto& item) {
        if (auto ac = std::dynamic_pointer_cast<Accessory>(item))
        {
            totalHP += ac->bonusHealth;
            totalMana += ac->bonusMana;
        }
    };
    addStats(equipment.ring1);
    addStats(equipment.ring2);
    addStats(equipment.amulet);
    if (totalHP > 0 || totalMana > 0)
        std::cout << "Accessory Bonuses: +" << totalHP << " HP, +" << totalMana << " Mana\n";
}

void Player::SetSkillLoadout(const std::vector<int>& loadout)
{
    skillLoadout = loadout;
    if (skillLoadout.size() > MAX_LOADOUT_SKILLS)
        skillLoadout.resize(MAX_LOADOUT_SKILLS);
}

void Player::RebuildLoadout()
{
    skillLoadout.clear();
    for (size_t i = 0; i < skills.GetSkillCount(); ++i)
    {
        auto sk = skills.GetSkill(i);
        if (!sk) continue;
        if (i == 0 && sk->name == "Attack") continue;
        if (skillLoadout.size() >= MAX_LOADOUT_SKILLS) break;
        skillLoadout.push_back(static_cast<int>(i));
    }
}

void Player::AutoFillLoadout()
{
    for (size_t i = 0; i < skills.GetSkillCount(); ++i)
    {
        auto sk = skills.GetSkill(i);
        if (!sk) continue;
        if (i == 0 && sk->name == "Attack") continue;
        if (IsInLoadout(static_cast<int>(i))) continue;
        if (skillLoadout.size() >= MAX_LOADOUT_SKILLS) break;
        skillLoadout.push_back(static_cast<int>(i));
    }
}

bool Player::IsInLoadout(int skillIndex) const
{
    for (int idx : skillLoadout)
        if (idx == skillIndex) return true;
    return false;
}

void Player::CheckNewSkills()
{
    auto addIfMissing = [this](const std::string& name, auto factory)
    {
        if (!skills.GetSkillByName(name))
        {
            auto skill = factory();
            skill->characterClass = characterClass;
            if (level >= skill->requiredLevel)
            {
                skills.AddSkill(skill);
                return true;
            }
        }
        return false;
    };

    bool learned = false;

    switch (characterClass)
    {
        case CharacterClass::Warrior:
            learned |= addIfMissing("Defensive Stance", []{ return std::make_shared<DefensiveStance>(); });
            learned |= addIfMissing("War Cry", []{ return std::make_shared<WarCry>(); });
            learned |= addIfMissing("Shield Bash", []{ return std::make_shared<ShieldBash>(); });
            learned |= addIfMissing("Battle Cry", []{ return std::make_shared<BattleCry>(); });
            learned |= addIfMissing("Execute", []{ return std::make_shared<Execute>(); });
            learned |= addIfMissing("War Stomp", []{ return std::make_shared<WarStomp>(); });
            break;

        case CharacterClass::Mage:
            learned |= addIfMissing("Arcane Bolt", []{ return std::make_shared<ArcaneBolt>(); });
            learned |= addIfMissing("Meteor", []{ return std::make_shared<Meteor>(); });
            learned |= addIfMissing("Frost Ward", []{ return std::make_shared<FrostWard>(); });
            learned |= addIfMissing("Arcane Surge", []{ return std::make_shared<ArcaneSurge>(); });
            learned |= addIfMissing("Mana Siphon", []{ return std::make_shared<ManaSiphon>(); });
            learned |= addIfMissing("Chain Lightning", []{ return std::make_shared<ChainLightning>(); });
            learned |= addIfMissing("Lightning Bolt", []{ return std::make_shared<LightningBolt>(); });
            learned |= addIfMissing("Arcane Missiles", []{ return std::make_shared<ArcaneMissiles>(); });
            learned |= addIfMissing("Frost Shield", []{ return std::make_shared<FrostShield>(); });
            learned |= addIfMissing("Mana Bomb", []{ return std::make_shared<ManaBomb>(); });
            learned |= addIfMissing("Blazing Orb", []{ return std::make_shared<BlazingOrb>(); });
            learned |= addIfMissing("Frost Nova", []{ return std::make_shared<FrostNova>(); });
            learned |= addIfMissing("Arcane Barrage", []{ return std::make_shared<ArcaneBarrage>(); });
            learned |= addIfMissing("Time Warp", []{ return std::make_shared<TimeWarp>(); });
            learned |= addIfMissing("Glacial Spike", []{ return std::make_shared<GlacialSpike>(); });
            learned |= addIfMissing("Mana Shield", []{ return std::make_shared<ManaShield>(); });
            learned |= addIfMissing("Pyroblast", []{ return std::make_shared<Pyroblast>(); });
            learned |= addIfMissing("Blizzard", []{ return std::make_shared<Blizzard>(); });
            learned |= addIfMissing("Arcane Power", []{ return std::make_shared<ArcanePower>(); });
            learned |= addIfMissing("Ice Lance", []{ return std::make_shared<IceLance>(); });
            learned |= addIfMissing("Inferno", []{ return std::make_shared<Inferno>(); });
            learned |= addIfMissing("Elemental Fury", []{ return std::make_shared<ElementalFury>(); });
            learned |= addIfMissing("Archmage's Cataclysm", []{ return std::make_shared<ArchmageCataclysm>(); });
            break;

        case CharacterClass::Priest:
            learned |= addIfMissing("Mass Heal", []{ return std::make_shared<MassHeal>(); });
            learned |= addIfMissing("Divine Shield", []{ return std::make_shared<DivineShield>(); });
            learned |= addIfMissing("Smite", []{ return std::make_shared<Smite>(); });
            learned |= addIfMissing("Renew", []{ return std::make_shared<Renew>(); });
            learned |= addIfMissing("Holy Nova", []{ return std::make_shared<HolyNova>(); });
            learned |= addIfMissing("Divine Wrath", []{ return std::make_shared<DivineWrath>(); });
            break;

        case CharacterClass::Archer:
            learned |= addIfMissing("Multi Shot", []{ return std::make_shared<MultiShot>(); });
            learned |= addIfMissing("Rain of Arrows", []{ return std::make_shared<RainOfArrows>(); });
            learned |= addIfMissing("Quick Shot", []{ return std::make_shared<QuickShot>(); });
            learned |= addIfMissing("Snipe", []{ return std::make_shared<Snipe>(); });
            learned |= addIfMissing("Arrow Storm", []{ return std::make_shared<ArrowStorm>(); });
            learned |= addIfMissing("Death Mark", []{ return std::make_shared<DeathMark>(); });
            break;

        case CharacterClass::Merchant:
            learned |= addIfMissing("Appraise", []{ return std::make_shared<Appraise>(); });
            learned |= addIfMissing("Haggle", []{ return std::make_shared<Haggle>(); });
            learned |= addIfMissing("Bribery", []{ return std::make_shared<Bribery>(); });
            learned |= addIfMissing("Invest", []{ return std::make_shared<Invest>(); });
            learned |= addIfMissing("Coin Storm", []{ return std::make_shared<CoinStorm>(); });
            learned |= addIfMissing("Golden Hurricane", []{ return std::make_shared<GoldenHurricane>(); });
            break;

        default: break;
    }

    if (learned)
    {
        std::cout << "\n* You learned new skills! *\n";
        for (size_t i = 0; i < skills.GetSkillCount(); ++i)
        {
            auto s = skills.GetSkill(i);
            if (s && s->requiredLevel == level)
                std::cout << "  - " << s->name << "\n";
        }
        AutoFillLoadout();
    }
}
