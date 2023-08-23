#pragma once

class Settings : public Singleton<Settings> {
public:
    static void LoadSettings();

    inline static bool debug_logging = false;

    inline static bool chairs_and_benches = true;
    inline static bool empty_containers = true;
};
