#include "RaidRubySanctumActions.h"
#include "Playerbots.h"
#include "RtiTargetValue.h"

namespace
{
    // Spells reused in actions
    constexpr uint32 SPELL_ENERVATING_BRAND       = 74502;
    constexpr uint32 SPELL_FLAME_BEACON           = 74453;
    constexpr uint32 SPELL_FIERY_COMBUSTION       = 74562;
    constexpr uint32 SPELL_SOUL_CONSUMPTION       = 74792;

    // Creatures reused in actions
    constexpr uint32 NPC_BALTHARUS_CLONE          = 39899;
    constexpr uint32 NPC_ONYX_FLAMECALLER         = 39814;
    constexpr uint32 NPC_METEOR_STRIKE_MARK       = 40055;
    constexpr uint32 NPC_FIERY_COMBUSTION_MARK    = 40001;
    constexpr uint32 NPC_LIVING_INFERNO           = 40681;
    constexpr uint32 NPC_LIVING_EMBER             = 40683;

    constexpr uint32 GO_HALION_PORTAL_1           = 202794;
    constexpr uint32 GO_HALION_PORTAL_2           = 202795;

    constexpr float HALION_EDGE_DISTANCE          = 28.0f;

    void ClearControlledMovement(Player* bot)
    {
        if (!bot)
            return;

        if (!bot->HasUnitState(UNIT_STATE_STUNNED | UNIT_STATE_FLEEING | UNIT_STATE_CONFUSED | UNIT_STATE_ROOT) &&
            bot->GetMotionMaster()->GetMotionSlotType(MOTION_SLOT_CONTROLLED) != NULL_MOTION_TYPE)
        {
            bot->GetMotionMaster()->Clear(true);
            bot->StopMoving();
        }
    }
}

bool RubySanctumBaltharusBrandAction::Execute(Event /*event*/)
{
    ClearControlledMovement(bot);

    // If brand faded, immediately re-engage the boss/clone
    if (!bot->HasAura(SPELL_ENERVATING_BRAND))
    {
        Unit* target = AI_VALUE2(Unit*, "find target", "baltharus");
        Unit* clone = AI_VALUE2(Unit*, "find target", "baltharus clone");
        if (clone)
            target = clone;  // Prefer the active clone if present

        if (target)
            return Attack(target);

        return false;
    }

    // Brand active: spread out from allies to avoid stacking the debuff
    // Use group-based separation instead of self to ensure we leave the clump
    if (MoveFromGroup(15.0f))
        return true;

    // Fallback: step away from current position if group spread failed
    if (MoveAway(bot, 15.0f))
        return true;

    Unit* target = AI_VALUE2(Unit*, "find target", "baltharus");
    Unit* clone = AI_VALUE2(Unit*, "find target", "baltharus clone");
    if (clone)
        target = clone;

    return target ? Attack(target) : false;
}

bool RubySanctumBaltharusSplitAddAction::Execute(Event /*event*/)
{
    ClearControlledMovement(bot);

    Unit* add = AI_VALUE2(Unit*, "find target", "baltharus clone");
    Unit* boss = AI_VALUE2(Unit*, "find target", "baltharus");
    if (!add)
    {
        GuidVector npcs = AI_VALUE(GuidVector, "nearest hostile npcs");
        for (ObjectGuid const& guid : npcs)
        {
            Unit* u = botAI->GetUnit(guid);
            if (u && u->GetEntry() == NPC_BALTHARUS_CLONE)
            {
                add = u;
                break;
            }
        }
    }
    if (!add)
    {
        if (boss)
            return Attack(boss);
        return false;
    }

    if (botAI->IsTank(bot))
    {
        if (Group* group = bot->GetGroup())
        {
            uint8 index = RtiTargetValue::GetRtiIndex("skull");
            ObjectGuid iconGuid = group->GetTargetIcon(index);
            if (iconGuid != add->GetGUID())
                group->SetTargetIcon(index, bot->GetGUID(), add->GetGUID());
        }

        if (boss && (botAI->IsMainTank(bot) || boss->GetVictim() == bot))
            return Attack(boss);

        if (botAI->DoSpecificAction("taunt", Event("rubysanctum"), true))
            return true;
        if (botAI->DoSpecificAction("growl", Event("rubysanctum"), true))
            return true;

        return Attack(add);
    }

    if (botAI->IsHeal(bot))
        return false;

    if (boss && botAI->IsRanged(bot))
        return Attack(boss);

    return Attack(add);
}

bool RubySanctumSavianaEnrageAction::Execute(Event /*event*/)
{
    ClearControlledMovement(bot);
    Unit* boss = AI_VALUE2(Unit*, "find target", "saviana");
    if (!boss)
        return false;

    // Try a tranquilize/soothe style dispel
    if (botAI->DoSpecificAction("tranquilizing shot", Event("rubysanctum"), true))
        return true;
    if (botAI->DoSpecificAction("soothe", Event("rubysanctum"), true))
        return true;
    if (botAI->DoSpecificAction("anesthetic poison", Event("rubysanctum"), true))
        return true;

    return false;
}

bool RubySanctumSavianaFlameBeaconAction::Execute(Event /*event*/)
{
    ClearControlledMovement(bot);
    // Move away from raid to drop conflagration
    return MoveAway(bot, 15.0f);
}

bool RubySanctumZarithrianCleaveArmorSwapAction::Execute(Event /*event*/)
{
    ClearControlledMovement(bot);
    Unit* boss = AI_VALUE2(Unit*, "find target", "zarithrian");
    if (!boss)
        return false;

    // Taunt swap
    if (botAI->DoSpecificAction("taunt", Event("rubysanctum"), true))
        return true;
    return botAI->DoSpecificAction("growl", Event("rubysanctum"), true);
}

bool RubySanctumZarithrianAddsAction::Execute(Event /*event*/)
{
    ClearControlledMovement(bot);

    Unit* target = nullptr;
    GuidVector npcs = AI_VALUE(GuidVector, "nearest hostile npcs");
    Unit* skullTarget = nullptr;
    if (Group* group = bot->GetGroup())
    {
        uint8 index = RtiTargetValue::GetRtiIndex("skull");
        ObjectGuid iconGuid = group->GetTargetIcon(index);
        skullTarget = botAI->GetUnit(iconGuid);
        if (skullTarget && (!skullTarget->IsAlive() || skullTarget->GetEntry() != NPC_ONYX_FLAMECALLER))
            skullTarget = nullptr;
    }

    auto selectAddTarget = [&](GuidVector const& units)
    {
        for (ObjectGuid const& guid : units)
        {
            Unit* u = botAI->GetUnit(guid);
            if (u && u->GetEntry() == NPC_ONYX_FLAMECALLER)
            {
                if (!target || u->GetHealth() < target->GetHealth())
                    target = u;
            }
        }
    };

    selectAddTarget(npcs);
    if (!target)
        selectAddTarget(AI_VALUE(GuidVector, "nearest npcs"));
    if (!target)
        selectAddTarget(AI_VALUE(GuidVector, "attackers"));

    if (skullTarget)
        target = skullTarget;
    if (!target)
    {
        if (Unit* boss = AI_VALUE2(Unit*, "find target", "zarithrian"))
            return Attack(boss);
        return false;
    }

    if (botAI->IsTank(bot))
    {
        if (Group* group = bot->GetGroup())
        {
            uint8 index = RtiTargetValue::GetRtiIndex("skull");
            ObjectGuid iconGuid = group->GetTargetIcon(index);
            if (iconGuid != target->GetGUID())
                group->SetTargetIcon(index, bot->GetGUID(), target->GetGUID());
        }
    }

    if (target->HasUnitState(UNIT_STATE_CASTING))
    {
        if (botAI->DoSpecificAction("kick", Event("rubysanctum"), true))
            return true;
        if (botAI->DoSpecificAction("pummel", Event("rubysanctum"), true))
            return true;
        if (botAI->DoSpecificAction("mind freeze", Event("rubysanctum"), true))
            return true;
        if (botAI->DoSpecificAction("shield bash", Event("rubysanctum"), true))
            return true;
        if (botAI->DoSpecificAction("earth shock", Event("rubysanctum"), true))
            return true;
        if (botAI->DoSpecificAction("wind shear", Event("rubysanctum"), true))
            return true;
        if (botAI->DoSpecificAction("counterspell", Event("rubysanctum"), true))
            return true;
        if (botAI->DoSpecificAction("spell lock", Event("rubysanctum"), true))
            return true;
        if (botAI->DoSpecificAction("rebuke", Event("rubysanctum"), true))
            return true;
    }

    // Main tank should hold the boss; let the other tank pick up adds
    Unit* boss = AI_VALUE2(Unit*, "find target", "zarithrian");
    if (botAI->IsTank(bot) && boss && boss->GetVictim() == bot)
        return false;

    if (botAI->IsTank(bot) && boss && botAI->IsMainTank(bot))
        return Attack(boss);

    if (botAI->IsTank(bot) && boss)
        MoveNear(boss, 6.0f);

    if (botAI->IsHeal(bot))
        return false;

    return Attack(target);
}

bool RubySanctumHalionCombustionAction::Execute(Event /*event*/)
{
    ClearControlledMovement(bot);
    Unit* boss = AI_VALUE2(Unit*, "find target", "halion");
    if (boss)
    {
        float dist = bot->GetExactDist2d(boss);
        if (dist < HALION_EDGE_DISTANCE)
            return MoveAway(boss, HALION_EDGE_DISTANCE);
    }

    Unit* mark = nullptr;
    GuidVector npcs = AI_VALUE(GuidVector, "nearest hostile npcs");
    for (ObjectGuid const& guid : npcs)
    {
        Unit* u = botAI->GetUnit(guid);
        if (u && u->GetEntry() == NPC_FIERY_COMBUSTION_MARK)
        {
            mark = u;
            break;
        }
    }

    return mark ? MoveAway(mark, 10.0f) : MoveAway(bot, 10.0f);
}

bool RubySanctumHalionConsumptionAction::Execute(Event /*event*/)
{
    ClearControlledMovement(bot);
    Unit* boss = AI_VALUE2(Unit*, "find target", "halion");
    if (boss)
    {
        float dist = bot->GetExactDist2d(boss);
        if (dist < HALION_EDGE_DISTANCE)
            return MoveAway(boss, HALION_EDGE_DISTANCE);
    }

    return MoveAway(bot, 10.0f);
}

bool RubySanctumHalionMeteorStrikeAction::Execute(Event /*event*/)
{
    ClearControlledMovement(bot);
    GuidVector npcs = AI_VALUE(GuidVector, "nearest hostile npcs");
    Unit* mark = nullptr;
    for (ObjectGuid const& guid : npcs)
    {
        Unit* u = botAI->GetUnit(guid);
        if (u && u->GetEntry() == NPC_METEOR_STRIKE_MARK)
        {
            mark = u;
            break;
        }
    }
    if (mark)
    {
        if (Unit* boss = AI_VALUE2(Unit*, "find target", "halion"))
        {
            if (botAI->IsTank(bot))
            {
                float angle = bot->GetAngle(boss) + ANGLE_90_DEG;
                return Move(angle, 3.0f);
            }
        }

        bool moved = MoveAway(mark, 12.0f);

        // Reposition to Halion's flank after dodging to avoid breath/tail and keep inside arena bounds
        if (Unit* boss = AI_VALUE2(Unit*, "find target", "halion"))
        {
            if (bot->GetExactDist2d(boss) > 30.0f)
                return Follow(boss, 10.0f, ANGLE_90_DEG);

            Follow(boss, 8.0f, ANGLE_90_DEG);
        }

        return moved;
    }
    return false;
}

bool RubySanctumHalionInfernosAction::Execute(Event /*event*/)
{
    ClearControlledMovement(bot);
    Unit* target = nullptr;
    GuidVector npcs = AI_VALUE(GuidVector, "nearest hostile npcs");
    for (ObjectGuid const& guid : npcs)
    {
        Unit* u = botAI->GetUnit(guid);
        if (u && (u->GetEntry() == NPC_LIVING_INFERNO || u->GetEntry() == NPC_LIVING_EMBER))
        {
            target = u;
            break;
        }
    }
    if (!target)
        return false;
    return Attack(target);
}

bool RubySanctumHalionEnterPortalAction::Execute(Event /*event*/)
{
    ClearControlledMovement(bot);

    GameObject* portal = bot->FindNearestGameObject(GO_HALION_PORTAL_1, 100.0f);
    GameObject* portalAlt = bot->FindNearestGameObject(GO_HALION_PORTAL_2, 100.0f);
    if (portalAlt && (!portal || bot->GetDistance2d(portalAlt) < bot->GetDistance2d(portal)))
        portal = portalAlt;

    if (!portal)
        return false;

    if (!portal->IsAtInteractDistance(bot))
        return MoveTo(portal, std::max(portal->GetInteractionDistance() - 1.0f, 0.0f));

    WorldPacket data(CMSG_GAMEOBJ_USE);
    data << portal->GetGUID();
    bot->GetSession()->HandleGameObjectUseOpcode(data);
    return true;
}

bool RubySanctumHalionTwilightCutterAction::Execute(Event /*event*/)
{
    ClearControlledMovement(bot);
    Unit* boss = AI_VALUE2(Unit*, "find target", "halion");
    if (boss)
    {
        float dist = bot->GetExactDist2d(boss);
        float orbitDistance = botAI->IsTank(bot) ? 8.0f : 12.0f;
        if (dist < orbitDistance - 2.0f)
            return MoveAway(boss, orbitDistance);
        if (dist > 30.0f)
            return Follow(boss, orbitDistance, ANGLE_90_DEG);

        float delta = botAI->IsTank(bot) ? -0.35f : 0.35f;
        float angle = boss->GetAngle(bot) + delta;
        return Follow(boss, orbitDistance, angle);
    }

    return MoveAway(bot, 10.0f);
}

bool RubySanctumHalionCorporealityBalanceAction::Execute(Event /*event*/)
{
    ClearControlledMovement(bot);
    Unit* boss = AI_VALUE2(Unit*, "find target", "halion");
    if (!boss || !boss->HealthBelowPct(50))
        return false;

    if (!botAI->ContainsStrategy(STRATEGY_TYPE_DPS))
        return true;

    static uint32 const corpAuras[] = { 74836,74835,74834,74833,74832,74826,74827,74828,74829,74830,74831 };
    int corpIndex = -1;
    for (int i = 0; i < static_cast<int>(sizeof(corpAuras) / sizeof(corpAuras[0])); ++i)
    {
        if (bot->HasAura(corpAuras[i]))
        {
            corpIndex = i;
            break;
        }
    }

    if (corpIndex == -1)
        return true;

    if (corpIndex <= 4)
        botAI->ChangeStrategy("-dps", BOT_STATE_COMBAT);
    else if (corpIndex >= 7)
        botAI->ChangeStrategy("+dps", BOT_STATE_COMBAT);

    return true;
}
