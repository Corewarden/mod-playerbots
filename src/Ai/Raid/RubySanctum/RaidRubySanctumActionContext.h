#ifndef _PLAYERBOT_RAIDRUBYSANCTUMACTIONCONTEXT_H
#define _PLAYERBOT_RAIDRUBYSANCTUMACTIONCONTEXT_H

#include "Action.h"
#include "NamedObjectContext.h"
#include "RaidRubySanctumActions.h"

class RaidRubySanctumActionContext : public NamedObjectContext<Action>
{
public:
    RaidRubySanctumActionContext()
    {
        creators["rubysanctum baltharus brand"] = &RaidRubySanctumActionContext::baltharus_brand;
        creators["rubysanctum baltharus split add"] = &RaidRubySanctumActionContext::baltharus_split_add;

        creators["rubysanctum saviana enrage dispel"] = &RaidRubySanctumActionContext::saviana_enrage;
        creators["rubysanctum saviana flame beacon"] = &RaidRubySanctumActionContext::saviana_flame_beacon;

        creators["rubysanctum zarithrian cleave armor swap"] = &RaidRubySanctumActionContext::zarithrian_cleave_armor_swap;
        creators["rubysanctum zarithrian adds"] = &RaidRubySanctumActionContext::zarithrian_adds;
        creators["rubysanctum zarithrian fear"] = &RaidRubySanctumActionContext::zarithrian_fear;

        creators["rubysanctum halion combustion"] = &RaidRubySanctumActionContext::halion_combustion;
        creators["rubysanctum halion consumption"] = &RaidRubySanctumActionContext::halion_consumption;
        creators["rubysanctum halion meteor"] = &RaidRubySanctumActionContext::halion_meteor;
        creators["rubysanctum halion infernos"] = &RaidRubySanctumActionContext::halion_infernos;
        creators["rubysanctum halion enter portal"] = &RaidRubySanctumActionContext::halion_enter_portal;
        creators["rubysanctum halion cutter move"] = &RaidRubySanctumActionContext::halion_cutter_move;
        creators["rubysanctum halion balance"] = &RaidRubySanctumActionContext::halion_balance;
    }

private:
    static Action* baltharus_brand(PlayerbotAI* botAI) { return new RubySanctumBaltharusBrandAction(botAI); }
    static Action* baltharus_split_add(PlayerbotAI* botAI) { return new RubySanctumBaltharusSplitAddAction(botAI); }

    static Action* saviana_enrage(PlayerbotAI* botAI) { return new RubySanctumSavianaEnrageAction(botAI); }
    static Action* saviana_flame_beacon(PlayerbotAI* botAI) { return new RubySanctumSavianaFlameBeaconAction(botAI); }

    static Action* zarithrian_cleave_armor_swap(PlayerbotAI* botAI) { return new RubySanctumZarithrianCleaveArmorSwapAction(botAI); }
    static Action* zarithrian_adds(PlayerbotAI* botAI) { return new RubySanctumZarithrianAddsAction(botAI); }
    static Action* zarithrian_fear(PlayerbotAI* botAI) { return new RubySanctumZarithrianFearAction(botAI); }

    static Action* halion_combustion(PlayerbotAI* botAI) { return new RubySanctumHalionCombustionAction(botAI); }
    static Action* halion_consumption(PlayerbotAI* botAI) { return new RubySanctumHalionConsumptionAction(botAI); }
    static Action* halion_meteor(PlayerbotAI* botAI) { return new RubySanctumHalionMeteorStrikeAction(botAI); }
    static Action* halion_infernos(PlayerbotAI* botAI) { return new RubySanctumHalionInfernosAction(botAI); }
    static Action* halion_enter_portal(PlayerbotAI* botAI) { return new RubySanctumHalionEnterPortalAction(botAI); }
    static Action* halion_cutter_move(PlayerbotAI* botAI) { return new RubySanctumHalionTwilightCutterAction(botAI); }
    static Action* halion_balance(PlayerbotAI* botAI) { return new RubySanctumHalionCorporealityBalanceAction(botAI); }
};

#endif
