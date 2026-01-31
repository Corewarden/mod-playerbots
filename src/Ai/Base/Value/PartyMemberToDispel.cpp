/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "PartyMemberToDispel.h"

#include "Playerbots.h"

namespace
{
    constexpr uint32 SPELL_FIERY_COMBUSTION = 74562;
    constexpr uint32 SPELL_MARK_OF_COMBUSTION = 74567;
    constexpr uint32 SPELL_SOUL_CONSUMPTION = 74792;
    constexpr uint32 SPELL_MARK_OF_CONSUMPTION = 74795;
    constexpr uint32 MAP_RUBY_SANCTUM = 724;
    constexpr float HALION_DISPEL_EDGE_DISTANCE = 32.0f;
}

class PartyMemberToDispelPredicate : public FindPlayerPredicate, public PlayerbotAIAware
{
public:
    PartyMemberToDispelPredicate(PlayerbotAI* botAI, uint32 dispelType)
        : PlayerbotAIAware(botAI), FindPlayerPredicate(), dispelType(dispelType)
    {
    }

    bool Check(Unit* unit) override
    {
        if (!unit || !unit->IsAlive() || !botAI->HasAuraToDispel(unit, dispelType))
            return false;

        if (unit->GetMapId() != MAP_RUBY_SANCTUM)
            return true;

        if (unit->HasAura(SPELL_FIERY_COMBUSTION) || unit->HasAura(SPELL_MARK_OF_COMBUSTION) ||
            unit->HasAura(SPELL_SOUL_CONSUMPTION) || unit->HasAura(SPELL_MARK_OF_CONSUMPTION))
        {
            Unit* halion = botAI->GetAiObjectContext()->GetValue<Unit*>("find target", "halion")->Get();
            if (halion && unit->GetExactDist2d(halion) < HALION_DISPEL_EDGE_DISTANCE)
                return false;
        }

        return true;
    }

private:
    uint32 dispelType;
};

Unit* PartyMemberToDispel::Calculate()
{
    uint32 dispelType = atoi(qualifier.c_str());

    PartyMemberToDispelPredicate predicate(botAI, dispelType);
    return FindPartyMember(predicate);
}
