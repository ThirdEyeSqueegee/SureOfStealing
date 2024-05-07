#include "Settings.h"

void Settings::LoadSettings() noexcept
{
    logger::info("Loading settings");

    CSimpleIniA ini;

    ini.SetUnicode();
    ini.LoadFile(R"(.\Data\SKSE\Plugins\SureOfStealing.ini)");

    debug_logging = ini.GetBoolValue("Log", "Debug");

    if (debug_logging) {
        spdlog::set_level(spdlog::level::debug);
        logger::debug("Debug logging enabled");
    }

    chairs_and_benches = ini.GetBoolValue("General", "bChairsAndBenches");

    logger::info("Loaded settings");
    logger::info("\tbChairsAndBenches = {}", chairs_and_benches);
    logger::info("");
}
