#include "Hooks.h"
#include "Logging.h"
#include "Settings.h"
#include "Utility.h"

#include "SKSE/Interfaces.h"

void Listener(SKSE::MessagingInterface::Message* message) {
    if (message->type == SKSE::MessagingInterface::kDataLoaded) {
        Settings::LoadSettings();
        Utility::InitGlobal();
        Hooks::Install();
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* skse) {
    InitializeLogging();

    const auto plugin  = SKSE::PluginDeclaration::GetSingleton();
    const auto version = plugin->GetVersion();

    logger::info("{} {} is loading...", plugin->GetName(), version);

    Init(skse);

    if (const auto messaging = SKSE::GetMessagingInterface(); !messaging->RegisterListener(Listener))
        return false;

    logger::info("{} has finished loading.", plugin->GetName());

    return true;
}
