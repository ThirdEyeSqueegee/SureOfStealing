#pragma once

class Utility : public Singleton<Utility> {
public:
    inline static RE::TESObjectREFR* crosshair_ref = nullptr;
    inline static RE::TESObjectREFR* last_activation = nullptr;
    inline static RE::TESGlobal* immersive_interactions_global = nullptr;
    inline static bool immersive_interactions_present = false;

    static void InitGlobal() {
        if (const auto handler = RE::TESDataHandler::GetSingleton(); handler->LookupModByName("ImmersiveInteractions.esp"sv)) {
            immersive_interactions_present = true;
            logger::info("Immersive Interactions compatibility enabled");
            if (!handler->LookupModByName("Sure of Stealing - Immersive Interactions Patch.esp"sv)) {
                logger::error("Immersive Interactions patch not found");
                stl::report_and_fail("ERROR: Sure of Stealing - Immersive Interactions Patch not installed"sv);
            }
            immersive_interactions_global = handler->LookupForm<RE::TESGlobal>(0x800, "Sure of Stealing - Immersive Interactions Patch.esp"sv);
            logger::info("Cached Immersive Interactions global: {} ({})", immersive_interactions_global->GetFormEditorID(),
                         immersive_interactions_global->value);
            return;
        }
        logger::info("Immersive Interactions not present");
    }
};
