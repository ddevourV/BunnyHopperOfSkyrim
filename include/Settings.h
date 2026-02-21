#pragma once

namespace Settings
{
    inline bool enabled = true;

    // Multipliers used by Strafe.cpp
    inline float globalSpeedMult = 1.0f;
    inline float strafeSpeedMult = 1.0f;

    // Basic strafe tuning
    inline float strafeDeadzone = 0.10f;
    inline float minStrafeAngle = 5.0f;

    // Keep these so other modules don't break if referenced somewhere
    inline bool enableFovZoom = false;
    inline bool enableTremble = false;

    inline bool LoadSettings()
    {
        return true; // no JSON
    }
}
