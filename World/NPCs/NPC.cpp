#include "NPC.hpp"

NPC::NPC(const std::string& name, const NPCDialogue& dialogue, int linkedQuestIndex)
    : name(name), dialogue(dialogue), linkedQuestIndex(linkedQuestIndex)
{
    if (linkedQuestIndex >= 0)
        linkedQuestIndices.push_back(linkedQuestIndex);
}

void NPC::AddLinkedQuest(int questIndex)
{
    linkedQuestIndices.push_back(questIndex);
}

// === Felrona (area 1) ===
// Quests: 0 Slime Exterminator, 1 Rat Catcher, 2 Bandit Troubles, 3 Gel Collection, 4 Rat Tails

NPC NPCDatabase::CreateFelronaBlacksmith()
{
    NPCDialogue d;
    d.greeting = "Welcome, traveler! I'm Garik, the blacksmith of Felrona.";
    d.questOffer = "I've got trouble with some slimes ruining my supplies. Can you help?";
    d.questComplete = "Excellent work! The slime menace is dealt with. Here's your reward.";
    d.farewell = "Stay sharp out there!";
    NPC npc("Garik the Blacksmith", d, 0);
    npc.AddLinkedQuest(3);
    return npc;
}

NPC NPCDatabase::CreateFelronaInnkeeper()
{
    NPCDialogue d;
    d.greeting = "Ah, an adventurer! I'm Mira, the innkeeper. Rest here while you can.";
    d.questOffer = "Rats have been getting into my food stores. Could you take care of them?";
    d.questComplete = "Thank you! No more rat problems for me.";
    d.farewell = "Safe travels!";
    NPC npc("Mira the Innkeeper", d, 1);
    npc.AddLinkedQuest(4);
    return npc;
}

NPC NPCDatabase::CreateFelronaGuard()
{
    NPCDialogue d;
    d.greeting = "Halt! I'm Captain Borin, guard captain of Felrona. Bandits have been causing trouble.";
    d.questOffer = "Bandits prey on travelers near the village. Can you teach them a lesson?";
    d.questComplete = "The roads are safer now. You have my thanks.";
    d.farewell = "Keep your weapon close.";
    return NPC("Captain Borin", d, 2);
}

// === Dark Woods (area 2) ===
// Quests: 5 Wolf Hunter, 6 Spider Menace, 7 Wolf Pelts, 8 Silk Harvest

NPC NPCDatabase::CreateDarkWoodsRanger()
{
    NPCDialogue d;
    d.greeting = "The woods are dangerous these days. I'm Theron, a ranger.";
    d.questOffer = "Wolves have been prowling near the village. Can you thin their numbers?";
    d.questComplete = "The village is safer now. You've done well.";
    d.farewell = "Watch your back in the shadows.";
    NPC npc("Theron the Ranger", d, 5);
    npc.AddLinkedQuest(7);
    return npc;
}

NPC NPCDatabase::CreateDarkWoodsHerbalist()
{
    NPCDialogue d;
    d.greeting = "Greetings! I'm Elara, an herbalist. The spiders here are a menace.";
    d.questOffer = "Giant Spiders are everywhere. Their silk is valuable, but they must be culled first.";
    d.questComplete = "The woods are safer. Thank you for the silk, too!";
    d.farewell = "May the forest heal.";
    NPC npc("Elara the Herbalist", d, 6);
    npc.AddLinkedQuest(8);
    return npc;
}

// === Salty Siren Coast (area 3) ===
// Quests: 9 Pirate Scourge, 10 Pirate Bounty, 11 Scale Collection

NPC NPCDatabase::CreateCoastPirateHunter()
{
    NPCDialogue d;
    d.greeting = "Ahoy! I'm Captain Vex, pirate hunter extraordinaire.";
    d.questOffer = "The pirates need to be taught a lesson. Interested?";
    d.questComplete = "Those pirates won't bother us again. Well done!";
    d.farewell = "May the winds be at your back!";
    NPC npc("Captain Vex", d, 9);
    npc.AddLinkedQuest(10);
    return npc;
}

NPC NPCDatabase::CreateCoastFisher()
{
    NPCDialogue d;
    d.greeting = "I'm Nereus, a fisherman. These waters hold dangers beyond fish.";
    d.questOffer = "Sea Serpents are attacking the fishing boats. Can you hunt them?";
    d.questComplete = "The serpents are dealt with! The fleet can sail again.";
    d.farewell = "May your nets be full.";
    NPC npc("Nereus the Fisher", d, 11);
    return npc;
}

// === Dragon's Peak (area 4) ===
// Quests: 12 Orc Bane, 13 Dragon Slayer, 14 Orc Tusks, 15 Essence Gathering

NPC NPCDatabase::CreateDragonPeakScholar()
{
    NPCDialogue d;
    d.greeting = "Fascinating creatures, dragons. I'm Elder Magnus, a scholar.";
    d.questOffer = "The Elder Dragon threatens us all. Will you face it?";
    d.questComplete = "Incredible! You've slain the beast. The realm owes you a debt.";
    d.farewell = "Knowledge is power. Use it wisely.";
    NPC npc("Elder Magnus", d, 13);
    return npc;
}

NPC NPCDatabase::CreateDragonPeakElementalist()
{
    NPCDialogue d;
    d.greeting = "I'm Pyra, an elementalist studying the peak's fire elementals.";
    d.questOffer = "Orc warbands are disrupting my research. Can you clear them out?";
    d.questComplete = "The orcs are gone. I can work in peace again.";
    d.farewell = "May the flames guide you.";
    NPC npc("Pyra the Elementalist", d, 12);
    npc.AddLinkedQuest(15);
    return npc;
}

// === Aran Highlands (area 5) ===
// Quests: 16 Highland Challenge, 17 Gryphon Hunt

NPC NPCDatabase::CreateHighlandsShieldbearer()
{
    NPCDialogue d;
    d.greeting = "I am Gorath, shieldbearer of the Highlands. Warriors seek worthy foes.";
    d.questOffer = "The Highland Warriors grow restless. Prove your strength against them.";
    d.questComplete = "You fight with honor. The Highlands respect your prowess.";
    d.farewell = "Stand tall.";
    return NPC("Shieldbearer Gorath", d, 16);
}

NPC NPCDatabase::CreateHighlandsSkywatcher()
{
    NPCDialogue d;
    d.greeting = "I'm Lyra, a skywatcher. The gryphons are nesting too close to the villages.";
    d.questOffer = "Can you thin the gryphon numbers? Their nests threaten our shepherds.";
    d.questComplete = "The skies are clearer now. Thank you, brave one.";
    d.farewell = "Watch the horizon.";
    return NPC("Skywatcher Lyra", d, 17);
}

// === Shadow Dungeon (area 6) ===
// Quests: 18 Shadow Purge, 19 Shadow Research, 20 Demon Parts

NPC NPCDatabase::CreateShadowDungeonPriest()
{
    NPCDialogue d;
    d.greeting = "The shadows grow ever darker. I'm Brother Aldric.";
    d.questOffer = "Dark forces stir in the depths. Will you purge them?";
    d.questComplete = "The darkness recedes thanks to your courage. Blessings upon you.";
    d.farewell = "May the light guide your path.";
    NPC npc("Brother Aldric", d, 18);
    npc.AddLinkedQuest(19);
    return npc;
}

NPC NPCDatabase::CreateShadowDungeonSlayer()
{
    NPCDialogue d;
    d.greeting = "I'm Kael, a void walker slayer. Demons lurk in these halls.";
    d.questOffer = "Demons are multiplying in the deep. Can you cull them?";
    d.questComplete = "The demon numbers are thinned. You have my respect.";
    d.farewell = "Strike true.";
    NPC npc("Kael the Slayer", d, 20);
    return npc;
}
