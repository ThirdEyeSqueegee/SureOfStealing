#include "Events.h"

#include "Settings.h"
#include "Utility.h"

namespace Events {
    OnActivateEventHandler* OnActivateEventHandler::GetSingleton() {
        static OnActivateEventHandler singleton;
        return std::addressof(singleton);
    }

    RE::BSEventNotifyControl OnActivateEventHandler::ProcessEvent(const RE::TESActivateEvent* a_event,
                                                                  RE::BSTEventSource<RE::TESActivateEvent>* a_eventSource) {
        if (!a_event) return RE::BSEventNotifyControl::kContinue;

        if (!a_event->actionRef->IsPlayerRef()) return RE::BSEventNotifyControl::kContinue;

        if (const auto player = RE::PlayerCharacter::GetSingleton();
            player->Is3DLoaded() && !player->IsSneaking()) {
            if (const auto& obj = a_event->objectActivated) {
                if (!"Coin Purse"sv.compare(obj->GetName())
                    || !"Door"sv.compare(obj->GetName())
                    || !"Large Wooden Gate"sv.compare(obj->GetName()))
                    return RE::BSEventNotifyControl::kContinue;

                if (obj->IsCrimeToActivate()
                    || (Settings::chairs_and_benches_flag
                        && (!"Chair"sv.compare(obj->GetName()) || !"Bench"sv.compare(obj->GetName())))
                    || (Settings::empty_containers_flag
                        && obj->GetFormType() == RE::FormType::Container
                        && obj->GetInventoryCount(true) == 0
                        && !std::string_view(obj->GetName()).contains("Merchant"sv))) {
                    // Skip unread books
                    if (obj->GetBaseObject()->IsBook()) {
                        if (const auto book = obj->GetBaseObject()->As<RE::TESObjectBOOK>(); !book->IsRead())
                            return RE::BSEventNotifyControl::kContinue;
                    }

                    if (obj == Utility::last_activation) {
                        obj->SetActivationBlocked(false);
                        Utility::last_activation = nullptr;
                    } else {
                        if (Utility::last_activation) {
                            obj->SetActivationBlocked(false);
                            Utility::last_activation = nullptr;
                        }
                        obj->SetActivationBlocked(true);
                        Utility::last_activation = obj;
                    }
                }
            }
        } else if (player->Is3DLoaded() && player->IsSneaking() && Utility::last_activation) {
            Utility::last_activation->SetActivationBlocked(false);
            Utility::last_activation = nullptr;
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    void OnActivateEventHandler::Register() {
        const auto holder = RE::ScriptEventSourceHolder::GetSingleton();
        holder->AddEventSink(GetSingleton());
        logger::info("Registered activate event handler");
    }

    InputEventHandler* InputEventHandler::GetSingleton() {
        static InputEventHandler singleton;
        return std::addressof(singleton);
    }

    RE::BSEventNotifyControl InputEventHandler::ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource) {
        if (!a_event) return RE::BSEventNotifyControl::kContinue;

        if (!Utility::immersive_interactions_present) return RE::BSEventNotifyControl::kContinue;

        for (auto e = *a_event; e != nullptr; e = e->next) {
            if (const auto button_event = e->AsButtonEvent()) {
                const auto control_map = RE::ControlMap::GetSingleton();
                const auto activate_key = control_map->GetMappedKey("Activate"sv, RE::INPUT_DEVICE::kKeyboard);
                const auto activate_key_controller = control_map->GetMappedKey("Activate"sv, RE::INPUT_DEVICE::kGamepad);
                if (button_event->GetIDCode() == activate_key || button_event->GetIDCode() == activate_key_controller) {
                    if (button_event->IsPressed()) {
                        if (Utility::crosshair_ref) {
                            if (Utility::crosshair_ref->IsCrimeToActivate()) {
                                if (Utility::last_activation == Utility::crosshair_ref) {
                                    Utility::immersive_interactions_global->value = 1.0f;
                                } else {
                                    Utility::immersive_interactions_global->value = 0.0f;
                                }
                            }
                        }
                    }
                }
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    void InputEventHandler::Register() {
        const auto manager = RE::BSInputDeviceManager::GetSingleton();
        manager->AddEventSink(GetSingleton());
        logger::info("Registered input event handler");
    }

    CrosshairEventHandler* CrosshairEventHandler::GetSingleton() {
        static CrosshairEventHandler singleton;
        return std::addressof(singleton);
    }

    RE::BSEventNotifyControl CrosshairEventHandler::ProcessEvent(const SKSE::CrosshairRefEvent* a_event,
                                                                 RE::BSTEventSource<SKSE::CrosshairRefEvent>* a_eventSource) {
        if (!a_event) return RE::BSEventNotifyControl::kContinue;

        if (!Utility::immersive_interactions_present) return RE::BSEventNotifyControl::kContinue;

        if (const auto& ref = a_event->crosshairRef)
            Utility::crosshair_ref = ref;
        else
            Utility::crosshair_ref = nullptr;

        return RE::BSEventNotifyControl::kContinue;
    }

    void CrosshairEventHandler::Register() {
        if (const auto source = SKSE::GetCrosshairRefEventSource()) {
            source->AddEventSink(GetSingleton());
            logger::info("Registered crosshair event handler");
        }
    }
}
