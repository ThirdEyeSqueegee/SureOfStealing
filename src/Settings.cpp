#include "Settings.h"
#include "SimpleIni.h"

Settings* Settings::GetSingleton() {
    static Settings singleton;
    return std::addressof(singleton);
}

void Settings::LoadSettings() {
    logger::info("Loading settings");

    CSimpleIniA ini;

    ini.SetUnicode();
    ini.LoadFile(R"(.\Data\SKSE\Plugins\SureOfStealing.ini)");

    chairs_and_benches_flag = ini.GetValue("General", "bChairsAndBenches");
    empty_containers_flag = ini.GetValue("General", "bEmptyContainers");

    logger::info("Loaded settings");
    logger::info("    Chairs and benches: {}", chairs_and_benches_flag);
    logger::info("    Empty containers: {}", empty_containers_flag);
}
