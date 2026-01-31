#include "RaidRubySanctumStrategy.h"

void RaidRubySanctumStrategy::InitTriggers(std::vector<TriggerNode*>& triggers)
{
    // Baltharus
    triggers.push_back(new TriggerNode("rubysanctum baltharus brand",
        { NextAction("rubysanctum baltharus brand", ACTION_RAID + 2) }));
    triggers.push_back(new TriggerNode("rubysanctum baltharus split add",
        { NextAction("rubysanctum baltharus split add", ACTION_RAID + 1) }));

    // Saviana
    triggers.push_back(new TriggerNode("rubysanctum saviana enrage dispel",
        { NextAction("rubysanctum saviana enrage dispel", ACTION_EMERGENCY + 3) }));
    triggers.push_back(new TriggerNode("rubysanctum saviana flame beacon",
        { NextAction("rubysanctum saviana flame beacon", ACTION_MOVE + 3) }));

    // Zarithrian
    triggers.push_back(new TriggerNode("rubysanctum zarithrian cleave armor swap",
        { NextAction("rubysanctum zarithrian cleave armor swap", ACTION_EMERGENCY + 2) }));
    triggers.push_back(new TriggerNode("rubysanctum zarithrian adds",
        { NextAction("rubysanctum zarithrian adds", ACTION_RAID + 2) }));
    triggers.push_back(new TriggerNode("rubysanctum zarithrian fear",
        { NextAction("rubysanctum zarithrian fear", ACTION_EMERGENCY + 1) }));

    // Halion P1/P2
    triggers.push_back(new TriggerNode("rubysanctum halion combustion",
        { NextAction("rubysanctum halion combustion", ACTION_MOVE + 4) }));
    triggers.push_back(new TriggerNode("rubysanctum halion consumption",
        { NextAction("rubysanctum halion consumption", ACTION_MOVE + 4) }));
    triggers.push_back(new TriggerNode("rubysanctum halion meteor",
        { NextAction("rubysanctum halion meteor", ACTION_RAID + 1) }));
    triggers.push_back(new TriggerNode("rubysanctum halion infernos",
        { NextAction("rubysanctum halion infernos", ACTION_RAID + 2) }));
    triggers.push_back(new TriggerNode("rubysanctum halion enter portal",
        { NextAction("rubysanctum halion enter portal", ACTION_RAID + 3) }));
    triggers.push_back(new TriggerNode("rubysanctum halion cutter move",
        { NextAction("rubysanctum halion cutter move", ACTION_MOVE + 5) }));

    // Halion P3 balance
    triggers.push_back(new TriggerNode("rubysanctum halion balance",
        { NextAction("rubysanctum halion balance", ACTION_RAID + 1) }));
}

void RaidRubySanctumStrategy::InitMultipliers(std::vector<Multiplier*>& multipliers)
{
}
