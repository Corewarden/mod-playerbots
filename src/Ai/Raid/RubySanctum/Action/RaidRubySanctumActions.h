#ifndef _PLAYERBOT_RAIDRUBYSANCTUMACTIONS_H
#define _PLAYERBOT_RAIDRUBYSANCTUMACTIONS_H

#include "AttackAction.h"

// Baltharus
class RubySanctumBaltharusBrandAction : public AttackAction
{
public:
    explicit RubySanctumBaltharusBrandAction(PlayerbotAI* botAI) : AttackAction(botAI, "rubysanctum baltharus brand") {}
    bool Execute(Event event) override;
};

class RubySanctumBaltharusSplitAddAction : public AttackAction
{
public:
    explicit RubySanctumBaltharusSplitAddAction(PlayerbotAI* botAI) : AttackAction(botAI, "rubysanctum baltharus split add") {}
    bool Execute(Event event) override;
};

// Saviana
class RubySanctumSavianaEnrageAction : public MovementAction
{
public:
    explicit RubySanctumSavianaEnrageAction(PlayerbotAI* botAI) : MovementAction(botAI, "rubysanctum saviana enrage dispel") {}
    bool Execute(Event event) override;
};

class RubySanctumSavianaFlameBeaconAction : public MovementAction
{
public:
    explicit RubySanctumSavianaFlameBeaconAction(PlayerbotAI* botAI) : MovementAction(botAI, "rubysanctum saviana flame beacon") {}
    bool Execute(Event event) override;
};

// Zarithrian
class RubySanctumZarithrianCleaveArmorSwapAction : public MovementAction
{
public:
    explicit RubySanctumZarithrianCleaveArmorSwapAction(PlayerbotAI* botAI) : MovementAction(botAI, "rubysanctum zarithrian cleave armor swap") {}
    bool Execute(Event event) override;
};

class RubySanctumZarithrianAddsAction : public AttackAction
{
public:
    explicit RubySanctumZarithrianAddsAction(PlayerbotAI* botAI) : AttackAction(botAI, "rubysanctum zarithrian adds") {}
    bool Execute(Event event) override;
};

// Halion
class RubySanctumHalionCombustionAction : public MovementAction
{
public:
    explicit RubySanctumHalionCombustionAction(PlayerbotAI* botAI) : MovementAction(botAI, "rubysanctum halion combustion") {}
    bool Execute(Event event) override;
};

class RubySanctumHalionConsumptionAction : public MovementAction
{
public:
    explicit RubySanctumHalionConsumptionAction(PlayerbotAI* botAI) : MovementAction(botAI, "rubysanctum halion consumption") {}
    bool Execute(Event event) override;
};

class RubySanctumHalionMeteorStrikeAction : public MovementAction
{
public:
    explicit RubySanctumHalionMeteorStrikeAction(PlayerbotAI* botAI) : MovementAction(botAI, "rubysanctum halion meteor") {}
    bool Execute(Event event) override;
};

class RubySanctumHalionInfernosAction : public AttackAction
{
public:
    explicit RubySanctumHalionInfernosAction(PlayerbotAI* botAI) : AttackAction(botAI, "rubysanctum halion infernos") {}
    bool Execute(Event event) override;
};

class RubySanctumHalionEnterPortalAction : public MovementAction
{
public:
    explicit RubySanctumHalionEnterPortalAction(PlayerbotAI* botAI) : MovementAction(botAI, "rubysanctum halion enter portal") {}
    bool Execute(Event event) override;
};

class RubySanctumHalionTwilightCutterAction : public MovementAction
{
public:
    explicit RubySanctumHalionTwilightCutterAction(PlayerbotAI* botAI) : MovementAction(botAI, "rubysanctum halion cutter move") {}
    bool Execute(Event event) override;
};

class RubySanctumHalionCorporealityBalanceAction : public MovementAction
{
public:
    explicit RubySanctumHalionCorporealityBalanceAction(PlayerbotAI* botAI) : MovementAction(botAI, "rubysanctum halion balance") {}
    bool Execute(Event event) override;
};

#endif
