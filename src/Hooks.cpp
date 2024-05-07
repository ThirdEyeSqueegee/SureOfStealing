#include "Hooks.h"

#include "Settings.h"
#include "Utility.h"

namespace Hooks
{
    void Install() noexcept
    {
        if (REL::Module::IsVR()) {
            PickupObject::idx = 206; // 0xce
        }

        stl::write_vfunc<RE::PlayerCharacter, PickupObject>();
        logger::info("Installed PlayerCharacter::PickUpObject hook");

        stl::write_vfunc<RE::TESFlora, ActivateFlora>();
        logger::info("Installed TESFlora::Activate hook");

        if (Settings::chairs_and_benches) {
            stl::write_vfunc<RE::TESFurniture, ActivateFurniture>();
            logger::info("Installed TESFurniture::Activate hook");
        }

        stl::write_vfunc<RE::TESObjectCONT, ActivateContainer>();
        logger::info("Installed TESObjectCONT::Activate hook");
    }

    void PickupObject::Thunk(RE::PlayerCharacter* a_this, RE::TESObjectREFR* a_object, uint32_t a_count, bool a_arg3, bool a_playSound) noexcept
    {
        if (a_this->Is3DLoaded() && !a_this->IsSneaking()) {
            const auto name{ a_object->GetName() };
            const auto form_id{ a_object->GetFormID() };
            if (a_object->IsCrimeToActivate()) {
                // Skip unread books
                if (a_object->GetBaseObject()->IsBook()) {
                    if (const auto book{ a_object->GetBaseObject()->As<RE::TESObjectBOOK>() }; !book->IsRead())
                        return func(a_this, a_object, a_count, a_arg3, a_playSound);
                }
                if (Utility::last_activation) {
                    if (a_object->GetFormID() == Utility::last_activation->GetFormID()) {
                        logger::debug("Allowing steal for {} (0x{:x})", name, form_id);
                        if (Utility::immersive_interactions_present) {
                            Utility::immersive_interactions_global->value = 0.0f;
                        }
                        Utility::last_activation = nullptr;

                        return func(a_this, a_object, a_count, a_arg3, a_playSound);
                    }
                }
                logger::debug("Blocking steal for {} (0x{:x})", name, form_id);
                Utility::last_activation = a_object;
                if (Utility::immersive_interactions_present) {
                    Utility::immersive_interactions_global->value = 1.0f;
                }

                return;
            }
        }
        else if (a_this->Is3DLoaded() && a_this->IsSneaking() && Utility::last_activation) {
            Utility::last_activation = nullptr;
            if (Utility::immersive_interactions_present) {
                Utility::immersive_interactions_global->value = 1.0f;
            }

            return func(a_this, a_object, a_count, a_arg3, a_playSound);
        }

        if (Utility::immersive_interactions_present) {
            if (Utility::immersive_interactions_global->value == 0.0f) {
                Utility::immersive_interactions_global->value = 1.0f;
            }
        }

        return func(a_this, a_object, a_count, a_arg3, a_playSound);
    }

    bool ActivateFlora::Thunk(RE::TESFlora* a_this, RE::TESObjectREFR* a_targetRef, RE::TESObjectREFR* a_activatorRef, std::uint8_t a_arg3, RE::TESBoundObject* a_object,
                              std::int32_t a_targetCount) noexcept
    {
        const auto name{ a_targetRef->GetName() };

        if ("Coin Purse"sv.compare(name)) {
            return func(a_this, a_targetRef, a_activatorRef, a_arg3, a_object, a_targetCount);
        }

        if (const auto player{ RE::PlayerCharacter::GetSingleton() }; a_activatorRef->IsPlayerRef()) {
            if (player->Is3DLoaded() && !player->IsSneaking()) {
                const auto form_id{ a_targetRef->GetFormID() };
                if (a_targetRef->IsCrimeToActivate()) {
                    if (Utility::last_activation) {
                        if (form_id == Utility::last_activation->GetFormID()) {
                            logger::debug("Allowing steal for {} (0x{:x})", name, form_id);
                            if (Utility::immersive_interactions_present) {
                                Utility::immersive_interactions_global->value = 0.0f;
                            }
                            Utility::last_activation = nullptr;

                            return func(a_this, a_targetRef, a_activatorRef, a_arg3, a_object, a_targetCount);
                        }
                    }
                    logger::debug("Blocking steal for {} (0x{:x})", name, form_id);
                    Utility::last_activation = a_targetRef;
                    if (Utility::immersive_interactions_present) {
                        Utility::immersive_interactions_global->value = 1.0f;
                    }

                    return func(a_this, nullptr, a_activatorRef, a_arg3, a_object, 0);
                }
            }
            else if (player->Is3DLoaded() && player->IsSneaking() && Utility::last_activation) {
                Utility::last_activation = nullptr;
                if (Utility::immersive_interactions_present) {
                    Utility::immersive_interactions_global->value = 1.0f;
                }

                return func(a_this, a_targetRef, a_activatorRef, a_arg3, a_object, a_targetCount);
            }

            if (Utility::immersive_interactions_present) {
                if (Utility::immersive_interactions_global->value == 0.0f) {
                    Utility::immersive_interactions_global->value = 1.0f;
                }
            }
        }

        return func(a_this, a_targetRef, a_activatorRef, a_arg3, a_object, a_targetCount);
    }

    bool ActivateFurniture::Thunk(RE::TESFurniture* a_this, RE::TESObjectREFR* a_targetRef, RE::TESObjectREFR* a_activatorRef, std::uint8_t a_arg3, RE::TESBoundObject* a_object,
                                  std::int32_t a_targetCount) noexcept
    {
        const auto name{ a_targetRef->GetName() };

        if ("Bench"sv.compare(name) && "Chair"sv.compare(name)) {
            return func(a_this, a_targetRef, a_activatorRef, a_arg3, a_object, a_targetCount);
        }

        if (const auto player{ RE::PlayerCharacter::GetSingleton() }; a_activatorRef->IsPlayerRef()) {
            if (player->Is3DLoaded() && !player->IsSneaking()) {
                const auto form_id{ a_targetRef->GetFormID() };
                if (Utility::last_activation) {
                    if (form_id == Utility::last_activation->GetFormID()) {
                        logger::debug("Allowing player to sit on {} (0x{:x})", name, form_id);
                        if (Utility::immersive_interactions_present) {
                            Utility::immersive_interactions_global->value = 0.0f;
                        }
                        Utility::last_activation = nullptr;

                        return func(a_this, a_targetRef, a_activatorRef, a_arg3, a_object, a_targetCount);
                    }
                }
                logger::debug("Blocking player from sitting on {} (0x{:x})", name, form_id);
                Utility::last_activation = a_targetRef;
                if (Utility::immersive_interactions_present) {
                    Utility::immersive_interactions_global->value = 1.0f;
                }

                return false;
            }
            if (player->Is3DLoaded() && player->IsSneaking() && Utility::last_activation) {
                Utility::last_activation = nullptr;
                if (Utility::immersive_interactions_present) {
                    Utility::immersive_interactions_global->value = 1.0f;
                }

                return func(a_this, a_targetRef, a_activatorRef, a_arg3, a_object, a_targetCount);
            }

            if (Utility::immersive_interactions_present) {
                if (Utility::immersive_interactions_global->value == 0.0f) {
                    Utility::immersive_interactions_global->value = 1.0f;
                }
            }
        }

        return func(a_this, a_targetRef, a_activatorRef, a_arg3, a_object, a_targetCount);
    }

    bool ActivateContainer::Thunk(RE::TESObjectCONT* a_this, RE::TESObjectREFR* a_targetRef, RE::TESObjectREFR* a_activatorRef, std::uint8_t a_arg3, RE::TESBoundObject* a_object,
                                  std::int32_t a_targetCount) noexcept
    {
        if (const auto player{ RE::PlayerCharacter::GetSingleton() }; a_activatorRef->IsPlayerRef()) {
            if (player->Is3DLoaded() && !player->IsSneaking()) {
                const auto name{ a_targetRef->GetName() };
                const auto form_id{ a_targetRef->GetFormID() };
                if (a_targetRef->IsCrimeToActivate()) {
                    if (Utility::last_activation) {
                        if (form_id == Utility::last_activation->GetFormID()) {
                            logger::debug("Allowing player to activate {} (0x{:x})", name, form_id);
                            if (Utility::immersive_interactions_present) {
                                Utility::immersive_interactions_global->value = 0.0f;
                            }
                            Utility::last_activation = nullptr;

                            return func(a_this, a_targetRef, a_activatorRef, a_arg3, a_object, a_targetCount);
                        }
                    }
                    RE::BSString activate_text;
                    a_this->GetActivateText(a_targetRef, activate_text);

                    if (std::string_view(activate_text.c_str()).contains("Empty")) {
                        return func(a_this, a_targetRef, a_activatorRef, a_arg3, a_object, a_targetCount);
                    }

                    logger::debug("Blocking player from activating {} (0x{:x})", name, form_id);
                    Utility::last_activation = a_targetRef;
                    if (Utility::immersive_interactions_present) {
                        Utility::immersive_interactions_global->value = 1.0f;
                    }

                    return false;
                }
            }
            else if (player->Is3DLoaded() && player->IsSneaking() && Utility::last_activation) {
                Utility::last_activation = nullptr;
                if (Utility::immersive_interactions_present) {
                    Utility::immersive_interactions_global->value = 1.0f;
                }

                return func(a_this, a_targetRef, a_activatorRef, a_arg3, a_object, a_targetCount);
            }

            if (Utility::immersive_interactions_present) {
                if (Utility::immersive_interactions_global->value == 0.0f) {
                    Utility::immersive_interactions_global->value = 1.0f;
                }
            }
        }

        return func(a_this, a_targetRef, a_activatorRef, a_arg3, a_object, a_targetCount);
    }
} // namespace Hooks
