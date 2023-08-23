#include "Settings.h"

void Settings::LoadSettings() {
    logger::info("Loading settings");

    CSimpleIniA ini;

    ini.SetUnicode();
    ini.LoadFile(R"(.\Data\SKSE\Plugins\SureOfStealing.ini)");

    debug_logging = ini.GetBoolValue("Log", "Debug");

    if (debug_logging) {
        spdlog::get("Global")->set_level(spdlog::level::level_enum::debug);
        logger::debug("Debug logging enabled");
    }

    chairs_and_benches = ini.GetBoolValue("General", "bChairsAndBenches");
    empty_containers = ini.GetBoolValue("General", "bEmptyContainers");

    logger::info("Loaded settings");
    logger::info("\tbChairsAndBenches = {}", chairs_and_benches);
    logger::info("\tbEmptyContainers = {}", empty_containers);
}
