#ifndef MERCHANT_HPP
#define MERCHANT_HPP

#include "../../Skill.hpp"

class ThrowCoin : public Skill
{
public:
    ThrowCoin();
    void Use(Character& caster, Character& target) override;
};

class Appraise : public Skill
{
public:
    Appraise();
    void Use(Character& caster, Character& target) override;
};

class Haggle : public Skill
{
public:
    Haggle();
    void Use(Character& caster, Character& target) override;
};

class Bribery : public Skill
{
public:
    Bribery();
    void Use(Character& caster, Character& target) override;
};

class Invest : public Skill
{
public:
    Invest();
    void Use(Character& caster, Character& target) override;
};

class CoinStorm : public Skill
{
public:
    CoinStorm();
    void Use(Character& caster, Character& target) override;
};

class GoldenHurricane : public Skill
{
public:
    GoldenHurricane();
    void Use(Character& caster, Character& target) override;
};

#endif
