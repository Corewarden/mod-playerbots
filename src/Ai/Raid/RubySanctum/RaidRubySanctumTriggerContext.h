#ifndef _PLAYERBOT_RAIDRUBYSANCTUMTRIGGERCONTEXT_H
#define _PLAYERBOT_RAIDRUBYSANCTUMTRIGGERCONTEXT_H

#include "AiObjectContext.h"
#include "NamedObjectContext.h"
#include "RaidRubySanctumTriggers.h"

class RaidRubySanctumTriggerContext : public NamedObjectContext<Trigger>
{
public:
    RaidRubySanctumTriggerContext()
    {
        creators["rubysanctum baltharus brand"] = &RaidRubySanctumTriggerContext::baltharus_brand;
        creators["rubysanctum baltharus split add"] = &RaidRubySanctumTriggerContext::baltharus_split_add;

        creators["rubysanctum saviana enrage dispel"] = &RaidRubySanctumTriggerContext::saviana_enrage;
        creators["rubysanctum saviana flame beacon"] = &RaidRubySanctumTriggerContext::saviana_flame_beacon;

        creators["rubysanctum zarithrian cleave armor swap"] = &RaidRubySanctumTriggerContext::zarithrian_cleave_armor;
        creators["rubysanctum zarithrian adds"] = &RaidRubySanctumTriggerContext::zarithrian_adds;

        creators["rubysanctum halion combustion"] = &RaidRubySanctumTriggerContext::halion_combustion;
        creators["rubysanctum halion consumption"] = &RaidRubySanctumTriggerContext::halion_consumption;
        creators["rubysanctum halion meteor"] = &RaidRubySanctumTriggerContext::halion_meteor;
        creators["rubysanctum halion infernos"] = &RaidRubySanctumTriggerContext::halion_infernos;
        creators["rubysanctum halion enter portal"] = &RaidRubySanctumTriggerContext::halion_enter_portal;
        creators["rubysanctum halion cutter move"] = &RaidRubySanctumTriggerContext::halion_cutter;
        creators["rubysanctum halion balance"] = &RaidRubySanctumTriggerContext::halion_balance;
    }

private:
    static Trigger* baltharus_brand(PlayerbotAI* botAI) { return new RubySanctumBaltharusBrandTrigger(botAI); }
    static Trigger* baltharus_split_add(PlayerbotAI* botAI) { return new RubySanctumBaltharusSplitAddTrigger(botAI); }

    static Trigger* saviana_enrage(PlayerbotAI* botAI) { return new RubySanctumSavianaEnrageTrigger(botAI); }
    static Trigger* saviana_flame_beacon(PlayerbotAI* botAI) { return new RubySanctumSavianaFlameBeaconTrigger(botAI); }

    static Trigger* zarithrian_cleave_armor(PlayerbotAI* botAI) { return new RubySanctumZarithrianCleaveArmorTrigger(botAI); }
    static Trigger* zarithrian_adds(PlayerbotAI* botAI) { return new RubySanctumZarithrianAddsTrigger(botAI); }

    static Trigger* halion_combustion(PlayerbotAI* botAI) { return new RubySanctumHalionCombustionTrigger(botAI); }
    static Trigger* halion_consumption(PlayerbotAI* botAI) { return new RubySanctumHalionConsumptionTrigger(botAI); }
    static Trigger* halion_meteor(PlayerbotAI* botAI) { return new RubySanctumHalionMeteorTrigger(botAI); }
    static Trigger* halion_infernos(PlayerbotAI* botAI) { return new RubySanctumHalionInfernosTrigger(botAI); }
    static Trigger* halion_enter_portal(PlayerbotAI* botAI) { return new RubySanctumHalionEnterPortalTrigger(botAI); }
    static Trigger* halion_cutter(PlayerbotAI* botAI) { return new RubySanctumHalionCutterTrigger(botAI); }
    static Trigger* halion_balance(PlayerbotAI* botAI) { return new RubySanctumHalionCorporealityOffBalanceTrigger(botAI); }
};

#endif
