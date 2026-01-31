#include "RaidRubySanctumTriggers.h"
#include "Playerbots.h"

#include <algorithm>
#include <vector>

// Spell / aura and creature IDs used by triggers
namespace
{
    // Baltharus
    constexpr uint32 SPELL_ENERVATING_BRAND       = 74502;
    constexpr uint32 NPC_BALTHARUS_CLONE          = 39899;

    // Saviana
    constexpr uint32 SPELL_SAVIANA_ENRAGE         = 78722; // Tranquilizable enrage
    constexpr uint32 SPELL_FLAME_BEACON           = 74453;

    // Zarithrian
    constexpr uint32 SPELL_CLEAVE_ARMOR           = 74367;
    constexpr uint32 NPC_ONYX_FLAMECALLER         = 39814;
    constexpr uint32 SPELL_INTIMIDATING_ROAR      = 74384;

    // Halion fire side
    constexpr uint32 SPELL_FIERY_COMBUSTION       = 74562;
    constexpr uint32 SPELL_MARK_OF_COMBUSTION     = 74567;
    constexpr uint32 NPC_METEOR_STRIKE_MARK       = 40029;
    constexpr uint32 NPC_LIVING_INFERNO           = 40681;
    constexpr uint32 NPC_LIVING_EMBER             = 40683;
    // Halion shadow side
    constexpr uint32 SPELL_SOUL_CONSUMPTION       = 74792;
    constexpr uint32 SPELL_MARK_OF_CONSUMPTION    = 74795;
    constexpr uint32 SPELL_TWILIGHT_REALM         = 74807;
    // Cutter
    constexpr uint32 SPELL_TWILIGHT_CUTTER        = 74768;
    constexpr uint32 SPELL_TWILIGHT_CUTTER_TRIG   = 74769;

    constexpr uint32 GO_HALION_PORTAL_1           = 202794;
    constexpr uint32 GO_HALION_PORTAL_2           = 202795;

    bool ShouldEnterTwilightRealm(PlayerbotAI* botAI, Player* bot, Group* group)
    {
        if (!bot || !group)
            return false;

        std::vector<Player*> tanks;
        std::vector<Player*> healers;
        std::vector<Player*> dps;

        for (GroupReference* ref = group->GetFirstMember(); ref; ref = ref->next())
        {
            Player* member = ref->GetSource();
            if (!member || !member->IsAlive())
                continue;

            if (botAI->IsTank(member))
                tanks.push_back(member);
            else if (botAI->IsHeal(member))
                healers.push_back(member);
            else
                dps.push_back(member);
        }

        int desiredTanks = std::min(1, static_cast<int>(tanks.size()));
        int desiredHealers = std::min(2, static_cast<int>(healers.size()));
        int desiredDps = static_cast<int>((dps.size() + 1) / 2);

        for (int i = 0; i < desiredTanks; ++i)
        {
            if (tanks[i] == bot)
                return true;
        }

        for (int i = 0; i < desiredHealers; ++i)
        {
            if (healers[i] == bot)
                return true;
        }

        for (int i = 0; i < desiredDps; ++i)
        {
            if (dps[i] == bot)
                return true;
        }

        return false;
    }
}

bool RubySanctumZarithrianFearTrigger::IsActive()
{
    Unit* boss = AI_VALUE2(Unit*, "find target", "zarithrian");
    if (!boss || !boss->HasUnitState(UNIT_STATE_CASTING))
        return false;

    Spell* currentSpell = boss->GetCurrentSpell(CURRENT_GENERIC_SPELL);
    if (!currentSpell || !currentSpell->m_spellInfo)
        return false;

    return currentSpell->m_spellInfo->Id == SPELL_INTIMIDATING_ROAR;
}

bool RubySanctumBaltharusSplitAddTrigger::IsActive()
{
    auto hasClone = [&](GuidVector const& units)
    {
        for (ObjectGuid const& guid : units)
        {
            Unit* u = botAI->GetUnit(guid);
            if (u && u->GetEntry() == NPC_BALTHARUS_CLONE)
                return true;
        }
        return false;
    };

    if (hasClone(AI_VALUE(GuidVector, "nearest hostile npcs")))
        return true;

    if (hasClone(AI_VALUE(GuidVector, "nearest npcs")))
        return true;

    return false;
}

bool RubySanctumHalionEnterPortalTrigger::IsActive()
{
    Unit* boss = AI_VALUE2(Unit*, "find target", "halion");
    if (!boss)
        return false;

    bool phase2 = boss->HealthBelowPct(75) && !boss->HealthBelowPct(50);
    bool phase3 = boss->HealthBelowPct(50);
    if (!phase2 && !phase3)
        return false;

    if (bot->HasAura(SPELL_TWILIGHT_REALM))
        return false;

    if (phase3)
    {
        Group* group = bot->GetGroup();
        if (!group || !ShouldEnterTwilightRealm(botAI, bot, group))
            return false;
    }

    return bot->FindNearestGameObject(GO_HALION_PORTAL_1, 100.0f) ||
           bot->FindNearestGameObject(GO_HALION_PORTAL_2, 100.0f);
}

bool RubySanctumBaltharusBrandTrigger::IsActive()
{
    return bot->HasAura(SPELL_ENERVATING_BRAND);
}

bool RubySanctumSavianaEnrageTrigger::IsActive()
{
    Unit* boss = AI_VALUE2(Unit*, "find target", "saviana");
    return boss && boss->HasAura(SPELL_SAVIANA_ENRAGE);
}

bool RubySanctumSavianaFlameBeaconTrigger::IsActive()
{
    return bot->HasAura(SPELL_FLAME_BEACON);
}

bool RubySanctumZarithrianCleaveArmorTrigger::IsActive()
{
    if (!botAI->IsTank(bot))
        return false;

    Unit* boss = AI_VALUE2(Unit*, "find target", "zarithrian");
    if (!boss)
        return false;

    Unit* victim = boss->GetVictim();
    if (!victim || victim == bot)
        return false;

    Aura const* aura = victim->GetAura(SPELL_CLEAVE_ARMOR);
    return aura && aura->GetStackAmount() >= 2;
}

bool RubySanctumZarithrianAddsTrigger::IsActive()
{
    GuidVector npcs = AI_VALUE(GuidVector, "nearest hostile npcs");
    for (ObjectGuid const& guid : npcs)
    {
        Unit* u = botAI->GetUnit(guid);
        if (u && u->GetEntry() == NPC_ONYX_FLAMECALLER)
            return true;
    }
    return false;
}

bool RubySanctumHalionCombustionTrigger::IsActive()
{
    return bot->HasAura(SPELL_FIERY_COMBUSTION) || bot->HasAura(SPELL_MARK_OF_COMBUSTION);
}

bool RubySanctumHalionConsumptionTrigger::IsActive()
{
    return bot->HasAura(SPELL_SOUL_CONSUMPTION) || bot->HasAura(SPELL_MARK_OF_CONSUMPTION);
}

bool RubySanctumHalionMeteorTrigger::IsActive()
{
    GuidVector npcs = AI_VALUE(GuidVector, "nearest hostile npcs");
    for (ObjectGuid const& guid : npcs)
    {
        Unit* u = botAI->GetUnit(guid);
        if (u && u->GetEntry() == NPC_METEOR_STRIKE_MARK)
            return true;
    }
    return false;
}

bool RubySanctumHalionInfernosTrigger::IsActive()
{
    GuidVector npcs = AI_VALUE(GuidVector, "nearest hostile npcs");
    for (ObjectGuid const& guid : npcs)
    {
        Unit* u = botAI->GetUnit(guid);
        if (u && (u->GetEntry() == NPC_LIVING_INFERNO || u->GetEntry() == NPC_LIVING_EMBER))
            return true;
    }
    return false;
}

bool RubySanctumHalionCutterTrigger::IsActive()
{
    return bot->HasAura(SPELL_TWILIGHT_CUTTER) || bot->HasAura(SPELL_TWILIGHT_CUTTER_TRIG);
}

bool RubySanctumHalionCorporealityOffBalanceTrigger::IsActive()
{
    // If any corporeality aura is present and not neutral, consider off-balance
    static uint32 const corpAuras[] = { 74836,74835,74834,74833,74832,74826,74827,74828,74829,74830,74831 };
    for (uint32 auraId : corpAuras)
    {
        if (Aura const* aura = bot->GetAura(auraId))
        {
            // 74826/74827 roughly neutral; anything else is imbalance
            if (auraId != 74826 && auraId != 74827)
                return true;
        }
    }
    return false;
}
