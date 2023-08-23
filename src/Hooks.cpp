#include "Hooks.h"

#include "Settings.h"
#include "Utility.h"

namespace Hooks {
    void Install() {
        stl::write_vfunc<RE::PlayerCharacter, PickupObject>();
        logger::info("Installed PlayerCharacter::PickUpObject hook");

        stl::write_vfunc<RE::TESFlora, Activate>();
        logger::info("Installed TESFlora::Activate hook");
    }

    void PickupObject::Thunk(RE::PlayerCharacter* a_this, RE::TESObjectREFR* a_object, uint32_t a_count, bool a_arg3, bool a_playSound) {
        if (a_this->Is3DLoaded() && !a_this->IsSneaking()) {
            const auto name = a_object->GetName();
            const auto form_id = a_object->GetFormID();
            if ((a_object->IsCrimeToActivate() && a_object->GetFormType() != RE::FormType::Container)
                || (Settings::chairs_and_benches && !("Chair"sv.compare(name) && "Bench"sv.compare(name)))
                || (Settings::empty_containers && a_object->GetFormType() == RE::FormType::Container && !a_object->GetInventoryCount(true))) {
                // Skip unread books
                if (a_object->GetBaseObject()->IsBook()) {
                    if (const auto book = a_object->GetBaseObject()->As<RE::TESObjectBOOK>(); !book->IsRead())
                        return func(a_this, a_object, a_count, a_arg3, a_playSound);
                }
                if (Utility::last_activation) {
                    if (a_object->GetFormID() == Utility::last_activation->GetFormID()) {
                        logger::debug("Allowing steal for {} (0x{:x})", name, form_id);
                        if (Utility::immersive_interactions_present)
                            Utility::immersive_interactions_global->value = 0.0f;
                        Utility::last_activation = nullptr;

                        return func(a_this, a_object, a_count, a_arg3, a_playSound);
                    }
                }
                logger::debug("Blocking steal for {} (0x{:x})", name, form_id);
                Utility::last_activation = a_object;
                if (Utility::immersive_interactions_present)
                    Utility::immersive_interactions_global->value = 1.0f;

                return;
            }
        } else if (a_this->Is3DLoaded() && a_this->IsSneaking() && Utility::last_activation) {
            Utility::last_activation = nullptr;
            if (Utility::immersive_interactions_present)
                Utility::immersive_interactions_global->value = 1.0f;

            return func(a_this, a_object, a_count, a_arg3, a_playSound);
        }

        if (Utility::immersive_interactions_present) {
            if (Utility::immersive_interactions_global->value == 0.0f)
                Utility::immersive_interactions_global->value = 1.0f;
        }

        return func(a_this, a_object, a_count, a_arg3, a_playSound);
    }

    bool Activate::Thunk(RE::TESFlora* a_this, RE::TESObjectREFR* a_targetRef, RE::TESObjectREFR* a_activatorRef, std::uint8_t a_arg3,
                         RE::TESBoundObject* a_object, std::int32_t a_targetCount) {
        if ("Coin Purse"sv.compare(a_targetRef->GetName()))
            return func(a_this, a_targetRef, a_activatorRef, a_arg3, a_object, a_targetCount);

        if (const auto player = RE::PlayerCharacter::GetSingleton(); a_activatorRef->IsPlayerRef()) {
            if (player->Is3DLoaded() && !player->IsSneaking()) {
                const auto name = a_targetRef->GetName();
                const auto form_id = a_targetRef->GetFormID();
                if (a_targetRef->IsCrimeToActivate()) {
                    if (Utility::last_activation) {
                        if (a_targetRef->GetFormID() == Utility::last_activation->GetFormID()) {
                            logger::debug("Allowing steal for {} (0x{:x})", name, form_id);
                            if (Utility::immersive_interactions_present)
                                Utility::immersive_interactions_global->value = 0.0f;
                            Utility::last_activation = nullptr;

                            return func(a_this, a_targetRef, a_activatorRef, a_arg3, a_object, a_targetCount);
                        }
                    }
                    logger::debug("Blocking steal for {} (0x{:x})", name, form_id);
                    Utility::last_activation = a_targetRef;
                    if (Utility::immersive_interactions_present)
                        Utility::immersive_interactions_global->value = 1.0f;

                    return func(a_this, nullptr, a_activatorRef, a_arg3, a_object, 0);
                }
            } else if (player->Is3DLoaded() && player->IsSneaking() && Utility::last_activation) {
                Utility::last_activation = nullptr;
                if (Utility::immersive_interactions_present)
                    Utility::immersive_interactions_global->value = 1.0f;

                return func(a_this, a_targetRef, a_activatorRef, a_arg3, a_object, a_targetCount);
            }

            if (Utility::immersive_interactions_present) {
                if (Utility::immersive_interactions_global->value == 0.0f)
                    Utility::immersive_interactions_global->value = 1.0f;
            }
        }

        return func(a_this, a_targetRef, a_activatorRef, a_arg3, a_object, a_targetCount);
    }
}
