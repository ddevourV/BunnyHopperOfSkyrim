#pragma once

namespace Settings
{
    inline bool enabled = true;

    // базовые дефолты для мувмента
    inline float strafeDeadzone = 0.10f;
    inline float minStrafeAngle = 5.0f;

    // чтобы другие файлы не падали, если где-то упоминается
    inline bool enableFovZoom = false;
    inline bool enableTremble = false;

    inline bool LoadSettings()
    {
        return true;
    }
}
