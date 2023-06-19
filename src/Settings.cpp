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

    sitting_flag = static_cast<bool>(std::atoi(ini.GetValue("General", "bSittingFlag")));
    container_flag = static_cast<bool>(std::atoi(ini.GetValue("General", "bContainerFlag")));

    logger::info("Loaded settings");
}
