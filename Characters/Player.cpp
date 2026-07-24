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

int Player::GetBagBonus() const
{
    auto oh = std::dynamic_pointer_cast<Offhand>(equipment.offhand);
    if (oh && oh->offhandType == OffhandType::Bag)
        return oh->defense * 2; // Each bag defense point adds 2 capacity
    return 0;
}

void Player::LevelUp()
{
    Character::LevelUp();

    ClassData classData = ClassDatabase::Get(characterClass);
    stats += classData.levelUpGain;

    currentHealth = GetMaxHealth();
    currentMana = GetMaxMana();
    RecalcMasteryBonuses();

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
                auto cl = std::make_shared<Cleave>(); cl->characterClass = CharacterClass::Warrior; skills.AddSkillIfLevelMet(cl, level);
                auto sw = std::make_shared<ShieldWall>(); sw->characterClass = CharacterClass::Warrior; skills.AddSkillIfLevelMet(sw, level);
                auto rc = std::make_shared<RallyingCry>(); rc->characterClass = CharacterClass::Warrior; skills.AddSkillIfLevelMet(rc, level);
                auto hs = std::make_shared<HammerSlam>(); hs->characterClass = CharacterClass::Warrior; skills.AddSkillIfLevelMet(hs, level);
                auto vr = std::make_shared<VictoryRush>(); vr->characterClass = CharacterClass::Warrior; skills.AddSkillIfLevelMet(vr, level);
                auto ch = std::make_shared<Charge>(); ch->characterClass = CharacterClass::Warrior; skills.AddSkillIfLevelMet(ch, level);
                auto cs = std::make_shared<CleaveStrike>(); cs->characterClass = CharacterClass::Warrior; skills.AddSkillIfLevelMet(cs, level);
                auto im = std::make_shared<Intimidate>(); im->characterClass = CharacterClass::Warrior; skills.AddSkillIfLevelMet(im, level);
                auto br = std::make_shared<BerserkerRage>(); br->characterClass = CharacterClass::Warrior; skills.AddSkillIfLevelMet(br, level);
                auto ic = std::make_shared<Intercept>(); ic->characterClass = CharacterClass::Warrior; skills.AddSkillIfLevelMet(ic, level);
                auto ft = std::make_shared<Fortress>(); ft->characterClass = CharacterClass::Warrior; skills.AddSkillIfLevelMet(ft, level);
                auto sc = std::make_shared<ShieldCharge>(); sc->characterClass = CharacterClass::Warrior; skills.AddSkillIfLevelMet(sc, level);
                auto dv = std::make_shared<Devastate>(); dv->characterClass = CharacterClass::Warrior; skills.AddSkillIfLevelMet(dv, level);
                auto wb = std::make_shared<Warbanner>(); wb->characterClass = CharacterClass::Warrior; skills.AddSkillIfLevelMet(wb, level);
                auto cs2 = std::make_shared<ColossusStrike>(); cs2->characterClass = CharacterClass::Warrior; skills.AddSkillIfLevelMet(cs2, level);
                auto rm = std::make_shared<Rampage>(); rm->characterClass = CharacterClass::Warrior; skills.AddSkillIfLevelMet(rm, level);
                auto tg = std::make_shared<TitansGrip>(); tg->characterClass = CharacterClass::Warrior; skills.AddSkillIfLevelMet(tg, level);
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
                auto pm = std::make_shared<PrayerOfMending>(); pm->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(pm, level);
                auto cl = std::make_shared<CleansingLight>(); cl->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(cl, level);
                auto hs2 = std::make_shared<HolySmiteII>(); hs2->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(hs2, level);
                auto rd = std::make_shared<Radiance>(); rd->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(rd, level);
                auto su = std::make_shared<SmiteUndead>(); su->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(su, level);
                auto ba = std::make_shared<Barrier>(); ba->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(ba, level);
                auto ph = std::make_shared<PrayerOfHealing>(); ph->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(ph, level);
                auto pu = std::make_shared<Purify>(); pu->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(pu, level);
                auto bn = std::make_shared<Benediction>(); bn->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(bn, level);
                auto hf = std::make_shared<HolyFire>(); hf->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(hf, level);
                auto cg = std::make_shared<ConsecratedGround>(); cg->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(cg, level);
                auto sr = std::make_shared<Serenity>(); sr->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(sr, level);
                auto ab = std::make_shared<ArchangelsBlessing>(); ab->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(ab, level);
                auto jd = std::make_shared<Judgement>(); jd->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(jd, level);
                auto as = std::make_shared<Ascension>(); as->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(as, level);
                auto ex = std::make_shared<Exorcism>(); ex->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(ex, level);
                auto dh = std::make_shared<DivineHurricane>(); dh->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(dh, level);
                auto pn = std::make_shared<Penance>(); pn->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(pn, level);
                auto rp = std::make_shared<Rapture>(); rp->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(rp, level);
                auto hp = std::make_shared<HolyPrism>(); hp->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(hp, level);
                auto gg = std::make_shared<GraceOfGod>(); gg->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(gg, level);
                auto cj = std::make_shared<CelestialJudgment>(); cj->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(cj, level);
                auto ap = std::make_shared<Apotheosis>(); ap->characterClass = CharacterClass::Priest; skills.AddSkillIfLevelMet(ap, level);
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
                auto pa = std::make_shared<PoisonedArrow>(); pa->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(pa, level);
                auto ev = std::make_shared<Evade>(); ev->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(ev, level);
                auto as = std::make_shared<AimedShot>(); as->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(as, level);
                auto vo = std::make_shared<Volley>(); vo->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(vo, level);
                auto ts = std::make_shared<TrapShot>(); ts->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(ts, level);
                auto he = std::make_shared<HawkEye>(); he->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(he, level);
                auto ew = std::make_shared<ExploitWeakness>(); ew->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(ew, level);
                auto cs = std::make_shared<ConcussiveShot>(); cs->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(cs, level);
                auto cm = std::make_shared<Camouflage>(); cm->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(cm, level);
                auto dt = std::make_shared<DoubleTap>(); dt->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(dt, level);
                auto ss = std::make_shared<ScatterShot>(); ss->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(ss, level);
                auto mt = std::make_shared<MarkTarget>(); mt->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(mt, level);
                auto hs = std::make_shared<Headshot>(); hs->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(hs, level);
                auto ba = std::make_shared<Barrage>(); ba->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(ba, level);
                auto sm = std::make_shared<Shadowmeld>(); sm->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(sm, level);
                auto sob = std::make_shared<StormOfBlades>(); sob->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(sob, level);
                auto em = std::make_shared<EvasiveManeuvers>(); em->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(em, level);
                auto rf = std::make_shared<RapidFire>(); rf->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(rf, level);
                auto wr = std::make_shared<Windrunner>(); wr->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(wr, level);
                auto pb = std::make_shared<PiercingBarrage>(); pb->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(pb, level);
                auto ss2 = std::make_shared<ShadowStep>(); ss2->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(ss2, level);
                auto wa = std::make_shared<WindArrow>(); wa->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(wa, level);
                auto ew2 = std::make_shared<EaglesWrath>(); ew2->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(ew2, level);
                auto pv = std::make_shared<PhantomVolley>(); pv->characterClass = CharacterClass::Archer; skills.AddSkillIfLevelMet(pv, level);
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
                auto po = std::make_shared<PurchaseOrder>(); po->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(po, level);
                auto aw = std::make_shared<AssessWeakness>(); aw->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(aw, level);
                auto pc = std::make_shared<PriceCheck>(); pc->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(pc, level);
                auto ms = std::make_shared<MarketSwing>(); ms->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(ms, level);
                auto bd = std::make_shared<BulkDeal>(); bd->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(bd, level);
                auto lq = std::make_shared<Liquidate>(); lq->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(lq, level);
                auto ht = std::make_shared<HostileTakeover>(); ht->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(ht, level);
                auto ip = std::make_shared<InsurancePolicy>(); ip->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(ip, level);
                auto ss = std::make_shared<ShortSell>(); ss->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(ss, level);
                auto pl = std::make_shared<PredatoryLending>(); pl->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(pl, level);
                auto gr = std::make_shared<GoldReserves>(); gr->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(gr, level);
                auto mc = std::make_shared<MarginCall>(); mc->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(mc, level);
                auto mo = std::make_shared<Monopoly>(); mo->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(mo, level);
                auto mk = std::make_shared<MarketCrash>(); mk->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(mk, level);
                auto tx = std::make_shared<Taxation>(); tx->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(tx, level);
                auto pl2 = std::make_shared<Plunder>(); pl2->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(pl2, level);
                auto cm = std::make_shared<CornerMarket>(); cm->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(cm, level);
                auto ff = std::make_shared<FortunesFavor>(); ff->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(ff, level);
                auto hb = std::make_shared<HostileBid>(); hb->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(hb, level);
                auto ec = std::make_shared<EconomicCollapse>(); ec->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(ec, level);
                auto mz = std::make_shared<Monopolize>(); mz->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(mz, level);
                auto hm = std::make_shared<HostileMerger>(); hm->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(hm, level);
                auto gh2 = std::make_shared<GoldenHandshake>(); gh2->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(gh2, level);
                auto lb = std::make_shared<LegendaryBargain>(); lb->characterClass = CharacterClass::Merchant; skills.AddSkillIfLevelMet(lb, level);
            }
            break;

        default:
            break;
    }
}

bool Player::CanEquip(const std::shared_ptr<Item>& item) const
{
    if (!item) return false;
    if (item->requiredLevel > GetLevel()) return false;

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
    if (skillLoadout.size() > static_cast<size_t>(GetMaxLoadoutSkills()))
        skillLoadout.resize(GetMaxLoadoutSkills());
}

void Player::RebuildLoadout()
{
    skillLoadout.clear();
    for (size_t i = 0; i < skills.GetSkillCount(); ++i)
    {
        auto sk = skills.GetSkill(i);
        if (!sk) continue;
        if (i == 0 && sk->name == "Attack") continue;
        if (skillLoadout.size() >= static_cast<size_t>(GetMaxLoadoutSkills())) break;
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
        if (skillLoadout.size() >= static_cast<size_t>(GetMaxLoadoutSkills())) break;
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
            learned |= addIfMissing("Power Strike", []{ return std::make_shared<PowerStrike>(); });
            learned |= addIfMissing("Whirlwind", []{ return std::make_shared<Whirlwind>(); });
            learned |= addIfMissing("Defensive Stance", []{ return std::make_shared<DefensiveStance>(); });
            learned |= addIfMissing("War Cry", []{ return std::make_shared<WarCry>(); });
            learned |= addIfMissing("Shield Bash", []{ return std::make_shared<ShieldBash>(); });
            learned |= addIfMissing("Battle Cry", []{ return std::make_shared<BattleCry>(); });
            learned |= addIfMissing("Execute", []{ return std::make_shared<Execute>(); });
            learned |= addIfMissing("War Stomp", []{ return std::make_shared<WarStomp>(); });
            learned |= addIfMissing("Cleave", []{ return std::make_shared<Cleave>(); });
            learned |= addIfMissing("Shield Wall", []{ return std::make_shared<ShieldWall>(); });
            learned |= addIfMissing("Rallying Cry", []{ return std::make_shared<RallyingCry>(); });
            learned |= addIfMissing("Hammer Slam", []{ return std::make_shared<HammerSlam>(); });
            learned |= addIfMissing("Victory Rush", []{ return std::make_shared<VictoryRush>(); });
            learned |= addIfMissing("Charge", []{ return std::make_shared<Charge>(); });
            learned |= addIfMissing("Cleave Strike", []{ return std::make_shared<CleaveStrike>(); });
            learned |= addIfMissing("Intimidate", []{ return std::make_shared<Intimidate>(); });
            learned |= addIfMissing("Berserker Rage", []{ return std::make_shared<BerserkerRage>(); });
            learned |= addIfMissing("Intercept", []{ return std::make_shared<Intercept>(); });
            learned |= addIfMissing("Fortress", []{ return std::make_shared<Fortress>(); });
            learned |= addIfMissing("Shield Charge", []{ return std::make_shared<ShieldCharge>(); });
            learned |= addIfMissing("Devastate", []{ return std::make_shared<Devastate>(); });
            learned |= addIfMissing("Warbanner", []{ return std::make_shared<Warbanner>(); });
            learned |= addIfMissing("Colossus Strike", []{ return std::make_shared<ColossusStrike>(); });
            learned |= addIfMissing("Rampage", []{ return std::make_shared<Rampage>(); });
            learned |= addIfMissing("Titan's Grip", []{ return std::make_shared<TitansGrip>(); });
            break;

        case CharacterClass::Mage:
            learned |= addIfMissing("Fireball", []{ return std::make_shared<Fireball>(); });
            learned |= addIfMissing("Ice Bolt", []{ return std::make_shared<IceBolt>(); });
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
            learned |= addIfMissing("Holy Smite", []{ return std::make_shared<HolySmite>(); });
            learned |= addIfMissing("Heal", []{ return std::make_shared<Heal>(); });
            learned |= addIfMissing("Mass Heal", []{ return std::make_shared<MassHeal>(); });
            learned |= addIfMissing("Divine Shield", []{ return std::make_shared<DivineShield>(); });
            learned |= addIfMissing("Smite", []{ return std::make_shared<Smite>(); });
            learned |= addIfMissing("Renew", []{ return std::make_shared<Renew>(); });
            learned |= addIfMissing("Holy Nova", []{ return std::make_shared<HolyNova>(); });
            learned |= addIfMissing("Divine Wrath", []{ return std::make_shared<DivineWrath>(); });
            learned |= addIfMissing("Prayer of Mending", []{ return std::make_shared<PrayerOfMending>(); });
            learned |= addIfMissing("Cleansing Light", []{ return std::make_shared<CleansingLight>(); });
            learned |= addIfMissing("Holy Smite II", []{ return std::make_shared<HolySmiteII>(); });
            learned |= addIfMissing("Radiance", []{ return std::make_shared<Radiance>(); });
            learned |= addIfMissing("Smite Undead", []{ return std::make_shared<SmiteUndead>(); });
            learned |= addIfMissing("Barrier", []{ return std::make_shared<Barrier>(); });
            learned |= addIfMissing("Prayer of Healing", []{ return std::make_shared<PrayerOfHealing>(); });
            learned |= addIfMissing("Purify", []{ return std::make_shared<Purify>(); });
            learned |= addIfMissing("Benediction", []{ return std::make_shared<Benediction>(); });
            learned |= addIfMissing("Holy Fire", []{ return std::make_shared<HolyFire>(); });
            learned |= addIfMissing("Consecrated Ground", []{ return std::make_shared<ConsecratedGround>(); });
            learned |= addIfMissing("Serenity", []{ return std::make_shared<Serenity>(); });
            learned |= addIfMissing("Archangel's Blessing", []{ return std::make_shared<ArchangelsBlessing>(); });
            learned |= addIfMissing("Judgement", []{ return std::make_shared<Judgement>(); });
            learned |= addIfMissing("Ascension", []{ return std::make_shared<Ascension>(); });
            learned |= addIfMissing("Exorcism", []{ return std::make_shared<Exorcism>(); });
            learned |= addIfMissing("Divine Hurricane", []{ return std::make_shared<DivineHurricane>(); });
            learned |= addIfMissing("Penance", []{ return std::make_shared<Penance>(); });
            learned |= addIfMissing("Rapture", []{ return std::make_shared<Rapture>(); });
            learned |= addIfMissing("Holy Prism", []{ return std::make_shared<HolyPrism>(); });
            learned |= addIfMissing("Grace of God", []{ return std::make_shared<GraceOfGod>(); });
            learned |= addIfMissing("Celestial Judgment", []{ return std::make_shared<CelestialJudgment>(); });
            learned |= addIfMissing("Apotheosis", []{ return std::make_shared<Apotheosis>(); });
            break;

        case CharacterClass::Archer:
            learned |= addIfMissing("Piercing Shot", []{ return std::make_shared<PiercingShot>(); });
            learned |= addIfMissing("Multi Shot", []{ return std::make_shared<MultiShot>(); });
            learned |= addIfMissing("Rain of Arrows", []{ return std::make_shared<RainOfArrows>(); });
            learned |= addIfMissing("Quick Shot", []{ return std::make_shared<QuickShot>(); });
            learned |= addIfMissing("Snipe", []{ return std::make_shared<Snipe>(); });
            learned |= addIfMissing("Arrow Storm", []{ return std::make_shared<ArrowStorm>(); });
            learned |= addIfMissing("Death Mark", []{ return std::make_shared<DeathMark>(); });
            learned |= addIfMissing("Poisoned Arrow", []{ return std::make_shared<PoisonedArrow>(); });
            learned |= addIfMissing("Evade", []{ return std::make_shared<Evade>(); });
            learned |= addIfMissing("Aimed Shot", []{ return std::make_shared<AimedShot>(); });
            learned |= addIfMissing("Volley", []{ return std::make_shared<Volley>(); });
            learned |= addIfMissing("Trap Shot", []{ return std::make_shared<TrapShot>(); });
            learned |= addIfMissing("Hawk Eye", []{ return std::make_shared<HawkEye>(); });
            learned |= addIfMissing("Exploit Weakness", []{ return std::make_shared<ExploitWeakness>(); });
            learned |= addIfMissing("Concussive Shot", []{ return std::make_shared<ConcussiveShot>(); });
            learned |= addIfMissing("Camouflage", []{ return std::make_shared<Camouflage>(); });
            learned |= addIfMissing("Double Tap", []{ return std::make_shared<DoubleTap>(); });
            learned |= addIfMissing("Scatter Shot", []{ return std::make_shared<ScatterShot>(); });
            learned |= addIfMissing("Mark Target", []{ return std::make_shared<MarkTarget>(); });
            learned |= addIfMissing("Headshot", []{ return std::make_shared<Headshot>(); });
            learned |= addIfMissing("Barrage", []{ return std::make_shared<Barrage>(); });
            learned |= addIfMissing("Shadowmeld", []{ return std::make_shared<Shadowmeld>(); });
            learned |= addIfMissing("Storm of Blades", []{ return std::make_shared<StormOfBlades>(); });
            learned |= addIfMissing("Evasive Maneuvers", []{ return std::make_shared<EvasiveManeuvers>(); });
            learned |= addIfMissing("Rapid Fire", []{ return std::make_shared<RapidFire>(); });
            learned |= addIfMissing("Windrunner", []{ return std::make_shared<Windrunner>(); });
            learned |= addIfMissing("Piercing Barrage", []{ return std::make_shared<PiercingBarrage>(); });
            learned |= addIfMissing("Shadow Step", []{ return std::make_shared<ShadowStep>(); });
            learned |= addIfMissing("Wind Arrow", []{ return std::make_shared<WindArrow>(); });
            learned |= addIfMissing("Eagle's Wrath", []{ return std::make_shared<EaglesWrath>(); });
            learned |= addIfMissing("Phantom Volley", []{ return std::make_shared<PhantomVolley>(); });
            break;

        case CharacterClass::Merchant:
            learned |= addIfMissing("Throw Coin", []{ return std::make_shared<ThrowCoin>(); });
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

void Player::EvolveClass()
{
    if (evolved) return;
    evolved = true;

    // +20% base stats
    Stats s = GetStats();
    s.health    = s.health    * 120 / 100;
    s.mana      = s.mana      * 120 / 100;
    s.strength  = s.strength  * 120 / 100;
    s.vitality  = s.vitality  * 120 / 100;
    s.intelligence = s.intelligence * 120 / 100;
    s.wisdom    = s.wisdom    * 120 / 100;
    s.dexterity = s.dexterity * 120 / 100;
    s.defense   = s.defense   * 120 / 100;
    SetStats(s);

    // Restore HP/MP to new max
    SetCurrentHealth(GetMaxHealth());
    SetCurrentMana(GetMaxMana());
    RecalcMasteryBonuses();
}

std::string Player::GetEvolvedClassName() const
{
    switch (characterClass)
    {
        case CharacterClass::Warrior:  return "Hero";
        case CharacterClass::Priest:   return "Sage";
        case CharacterClass::Mage:     return "Archmage";
        case CharacterClass::Archer:   return "Ranger";
        case CharacterClass::Merchant: return "Tycoon";
        default: return "";
    }
}

float Player::GetEvolvedDamageReduction() const
{
    if (!evolved || characterClass != CharacterClass::Warrior) return 0.0f;
    return 0.10f;
}

float Player::GetEvolvedHealingBonus() const
{
    if (!evolved || characterClass != CharacterClass::Priest) return 0.0f;
    return 0.15f;
}

float Player::GetEvolvedManaCostReduction() const
{
    if (!evolved || characterClass != CharacterClass::Mage) return 0.0f;
    return 0.15f;
}

float Player::GetEvolvedCritBonus() const
{
    if (!evolved || characterClass != CharacterClass::Archer) return 0.0f;
    return 0.10f;
}

float Player::GetEvolvedGoldFind() const
{
    if (!evolved || characterClass != CharacterClass::Merchant) return 0.0f;
    return 0.25f;
}

// ---- Character Mastery ----

void Player::OnOverflowXP(int xp) { GainCharMasteryXP(xp); }

void Player::GainCharMasteryXP(int xp) {
    float xpBonus = 1.0f + GetCharMasteryXPBonus();
    charMasteryXP += static_cast<int>(xp * xpBonus);
    int required = GetCharMasteryXPToLevel();
    while (charMasteryXP >= required) {
        charMasteryXP -= required;
        CharMasteryLevelUp();
        required = GetCharMasteryXPToLevel();
    }
}

void Player::CharMasteryLevelUp() {
    charMasteryLevel++;
    charMasteryPoints++;
    currentHealth = GetMaxHealth();
    currentMana = GetMaxMana();
    RecalcMasteryBonuses();
}

bool Player::CanUnlockCharMasteryNode(int branch, int node) const {
    if (branch < 0 || branch >= CHAR_MASTERY_BRANCHES) return false;
    if (node < 0 || node >= CHAR_MASTERY_NODES_PER_BRANCH) return false;
    if (charMasteryNodes[branch][node]) return false;
    if (charMasteryPoints <= 0) return false;
    for (int n = 0; n < node; ++n)
        if (!charMasteryNodes[branch][n]) return false;
    int requiredLevel = (node + 1) * 4;
    if (charMasteryLevel < requiredLevel) return false;
    return true;
}

bool Player::UnlockCharMasteryNode(int branch, int node) {
    if (!CanUnlockCharMasteryNode(branch, node)) return false;
    charMasteryNodes[branch][node] = true;
    charMasteryPoints--;
    currentHealth = GetMaxHealth();
    currentMana = GetMaxMana();
    RecalcMasteryBonuses();
    return true;
}

void Player::RecalcMasteryBonuses() {
    int hpBonus = (stats.health * GetCharMasteryBonusHP()) / 100;
    int mpBonus = 0;
    SetMasteryBonuses(hpBonus, mpBonus, GetCharMasteryBonusDEF(),
                      GetCharMasteryDamageBonus(), GetCharMasteryDamageReduction(),
                      GetCharMasteryHealingBonus(), GetCharMasteryManaCostReduction(),
                      GetCharMasteryDodgeChance());
}

int Player::GetCharMasteryBonusHP() const {
    int bonus = 0;
    for (int n = 0; n < CHAR_MASTERY_NODES_PER_BRANCH; ++n)
        if (charMasteryNodes[0][n]) bonus += 5;
    // Overflow: +1% HP per level beyond node tree (level 20+)
    int nodesComplete = 0;
    for (int n = 0; n < CHAR_MASTERY_NODES_PER_BRANCH; ++n)
        if (charMasteryNodes[0][n]) nodesComplete++;
    if (nodesComplete == CHAR_MASTERY_NODES_PER_BRANCH && charMasteryLevel > 20)
        bonus += (charMasteryLevel - 20);
    return bonus;
}

int Player::GetCharMasteryBonusDEF() const {
    int count = 0;
    for (int n = 0; n < CHAR_MASTERY_NODES_PER_BRANCH; ++n)
        if (charMasteryNodes[0][n]) count++;
    int def = count * 2;
    // Overflow: +1 DEF per level beyond node tree
    if (count == CHAR_MASTERY_NODES_PER_BRANCH && charMasteryLevel > 20)
        def += (charMasteryLevel - 20);
    return def;
}

float Player::GetCharMasteryDamageBonus() const {
    // Merchant branch 1 is "Opulence" (Gold Find), not damage — skip it
    if (characterClass == CharacterClass::Merchant) return 0.0f;
    int count = 0;
    for (int n = 0; n < CHAR_MASTERY_NODES_PER_BRANCH; ++n)
        if (charMasteryNodes[1][n]) count++;
    float bonus = count * 0.05f;
    // Overflow: +1% damage per level beyond node tree
    if (count == CHAR_MASTERY_NODES_PER_BRANCH && charMasteryLevel > 20)
        bonus += (charMasteryLevel - 20) * 0.01f;
    return bonus;
}

float Player::GetCharMasteryDamageReduction() const {
    if (characterClass != CharacterClass::Warrior) return 0.0f;
    int count = 0;
    for (int n = 0; n < CHAR_MASTERY_NODES_PER_BRANCH; ++n)
        if (charMasteryNodes[2][n]) count++;
    float bonus = count * 0.02f;
    if (count == CHAR_MASTERY_NODES_PER_BRANCH && charMasteryLevel > 20)
        bonus += (charMasteryLevel - 20) * 0.01f;
    return bonus;
}

float Player::GetCharMasteryHealingBonus() const {
    if (characterClass != CharacterClass::Priest) return 0.0f;
    int count = 0;
    for (int n = 0; n < CHAR_MASTERY_NODES_PER_BRANCH; ++n)
        if (charMasteryNodes[2][n]) count++;
    float bonus = count * 0.05f;
    if (count == CHAR_MASTERY_NODES_PER_BRANCH && charMasteryLevel > 20)
        bonus += (charMasteryLevel - 20) * 0.02f;
    return bonus;
}

float Player::GetCharMasteryManaCostReduction() const {
    if (characterClass != CharacterClass::Mage) return 0.0f;
    int count = 0;
    for (int n = 0; n < CHAR_MASTERY_NODES_PER_BRANCH; ++n)
        if (charMasteryNodes[2][n]) count++;
    float bonus = count * 0.03f;
    if (count == CHAR_MASTERY_NODES_PER_BRANCH && charMasteryLevel > 20)
        bonus += (charMasteryLevel - 20) * 0.01f;
    return bonus;
}

float Player::GetCharMasteryDodgeChance() const {
    if (characterClass != CharacterClass::Archer) return 0.0f;
    int count = 0;
    for (int n = 0; n < CHAR_MASTERY_NODES_PER_BRANCH; ++n)
        if (charMasteryNodes[2][n]) count++;
    float bonus = count * 0.03f;
    if (count == CHAR_MASTERY_NODES_PER_BRANCH && charMasteryLevel > 20)
        bonus += (charMasteryLevel - 20) * 0.01f;
    return bonus;
}

float Player::GetCharMasteryGoldFind() const {
    if (characterClass != CharacterClass::Merchant) return 0.0f;
    int count = 0;
    for (int n = 0; n < CHAR_MASTERY_NODES_PER_BRANCH; ++n)
        if (charMasteryNodes[1][n]) count++;
    float bonus = count * 0.05f;
    if (count == CHAR_MASTERY_NODES_PER_BRANCH && charMasteryLevel > 20)
        bonus += (charMasteryLevel - 20) * 0.02f;
    return bonus;
}

float Player::GetCharMasteryXPBonus() const {
    if (characterClass != CharacterClass::Merchant) return 0.0f;
    int count = 0;
    for (int n = 0; n < CHAR_MASTERY_NODES_PER_BRANCH; ++n)
        if (charMasteryNodes[2][n]) count++;
    float bonus = count * 0.03f;
    if (count == CHAR_MASTERY_NODES_PER_BRANCH && charMasteryLevel > 20)
        bonus += (charMasteryLevel - 20) * 0.01f;
    return bonus;
}
