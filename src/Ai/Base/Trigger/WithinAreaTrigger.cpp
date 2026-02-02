/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license, you may redistribute it
 * and/or modify it under version 3 of the License, or (at your option), any later version.
 */

#include "WithinAreaTrigger.h"

#include "LastMovementValue.h"
#include "Log.h"
#include "Playerbots.h"

bool WithinAreaTrigger::IsActive()
{
    LastMovement& movement = context->GetValue<LastMovement&>("last area trigger")->Get();
    if (!movement.lastAreaTrigger)
        return false;

    // Cache the trigger ID locally to reduce race condition window
    uint32 const triggerId = movement.lastAreaTrigger;
    
    // Validate trigger ID is in reasonable range before accessing ObjectMgr stores
    // This helps avoid crashes during store reloads
    if (triggerId == 0 || triggerId > 100000)
        return false;

    AreaTrigger const* at = nullptr;
    
    try
    {
        at = sObjectMgr->GetAreaTrigger(triggerId);
        if (!at)
            return false;

        if (!sObjectMgr->GetAreaTriggerTeleport(triggerId))
            return false;
    }
    catch (...)
    {
        // Handle potential access violations during store reloads
        LOG_ERROR("playerbots", "WithinAreaTrigger::IsActive - Exception accessing area trigger {}", triggerId);
        return false;
    }

    return IsPointInAreaTriggerZone(at, bot->GetMapId(), bot->GetPositionX(), bot->GetPositionY(), bot->GetPositionZ(),
                                    0.5f);
}

bool WithinAreaTrigger::IsPointInAreaTriggerZone(AreaTrigger const* atEntry, uint32 mapid, float x, float y, float z,
                                                 float delta)
{
    if (mapid != atEntry->map)
        return false;

    if (atEntry->radius > 0)
    {
        // if we have radius check it
        float dist2 = (x - atEntry->x) * (x - atEntry->x) + (y - atEntry->y) * (y - atEntry->y) +
                      (z - atEntry->z) * (z - atEntry->z);
        if (dist2 > (atEntry->radius + delta) * (atEntry->radius + delta))
            return false;
    }
    else
    {
        // we have only extent

        // rotate the players position instead of rotating the whole cube, that way we can make a simplified
        // is-in-cube check and we have to calculate only one point instead of 4

        // 2PI = 360, keep in mind that ingame orientation is counter-clockwise
        double rotation = 2 * M_PI - atEntry->orientation;
        double sinVal = sin(rotation);
        double cosVal = cos(rotation);

        float playerBoxDistX = x - atEntry->x;
        float playerBoxDistY = y - atEntry->y;

        float rotPlayerX = float(atEntry->x + playerBoxDistX * cosVal - playerBoxDistY * sinVal);
        float rotPlayerY = float(atEntry->y + playerBoxDistY * cosVal + playerBoxDistX * sinVal);

        // box edges are parallel to coordiante axis, so we can treat every dimension independently :D
        float dz = z - atEntry->z;
        float dx = rotPlayerX - atEntry->x;
        float dy = rotPlayerY - atEntry->y;
        if ((fabs(dx) > atEntry->x / 2 + delta) || (fabs(dy) > atEntry->y / 2 + delta) ||
            (fabs(dz) > atEntry->z / 2 + delta))
        {
            return false;
        }
    }

    return true;
}
