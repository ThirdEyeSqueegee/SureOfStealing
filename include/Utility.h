#pragma once

class Utility {
protected:
    Utility() = default;
    ~Utility() = default;

public:
    Utility(const Utility&) = delete; // Prevent copy construction
    Utility(Utility&&) = delete; // Prevent move construction
    Utility& operator=(const Utility&) = delete; // Prevent copy assignment
    Utility& operator=(Utility&&) = delete; // Prevent move assignment

    static Utility* GetSingleton() {
        static Utility singleton;
        return std::addressof(singleton);
    }

    inline static RE::NiPointer<RE::TESObjectREFR> crosshair_ref = nullptr;
    inline static RE::NiPointer<RE::TESObjectREFR> last_activation = nullptr;
    inline static RE::TESGlobal* immersive_interactions_global = nullptr;
    inline static bool immersive_interactions_present = false;

    static void InitGlobal() {
        const auto handler = RE::TESDataHandler::GetSingleton();
        if (!handler->LookupModByName("Sure of Stealing - Immersive Interactions Patch.esp"sv)) {
            logger::info("Immersive Interactions patch not found");
            return;
        }
        if (handler->LookupModByName("ImmersiveInteractions.esp"sv)) {
            immersive_interactions_present = true;
            immersive_interactions_global = handler->LookupForm<RE::TESGlobal>(0x800, "Sure of Stealing - Immersive Interactions Patch.esp"sv);
            logger::info("Cached Immersive Interactions global: {} ({})", immersive_interactions_global->GetFormEditorID(),
                         immersive_interactions_global->value);
        } else {
            logger::info("Immersive Interactions not found");
        }
    }
};
