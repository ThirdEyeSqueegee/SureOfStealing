#include "Events.h"

#include "Settings.h"
#include "Utility.h"

namespace Events {
    OnActivateEventHandler* OnActivateEventHandler::GetSingleton() {
        static OnActivateEventHandler singleton;
        return std::addressof(singleton);
    }

    RE::BSEventNotifyControl OnActivateEventHandler::ProcessEvent(const RE::TESActivateEvent* a_event,
                                                                  RE::BSTEventSource<RE::TESActivateEvent>*
                                                                  a_eventSource) {
        if (!a_event) return RE::BSEventNotifyControl::kContinue;

        if (const auto player = RE::PlayerCharacter::GetSingleton();
            player->Is3DLoaded() && !player->IsSneaking()) {
            if (const auto& obj = a_event->objectActivated) {
                if (obj->IsCrimeToActivate() || ((!"Bench"sv.compare(obj->GetName())
                                                  || !"Chair"sv.compare(obj->GetName())) && Settings::sitting_flag)
                    || (obj->GetFormType() == RE::FormType::Container
                        && !std::string_view(obj->GetName()).contains("Merchant")
                        && Settings::container_flag)) {
                    if (!"Coin Purse"sv.compare(obj->GetName())
                        || !"Door"sv.compare(obj->GetName())
                        || !"Large Wooden Gate"sv.compare(obj->GetName()))
                        return RE::BSEventNotifyControl::kContinue;

                    if (a_event->actionRef->IsPlayerRef()) {
                        if (obj->GetBaseObject()->IsBook()) {
                            if (const auto book = obj->GetBaseObject()->As<RE::TESObjectBOOK>(); !book->IsRead())
                                return RE::BSEventNotifyControl::kContinue;
                        }

                        if (obj->GetFormType() == RE::FormType::Container) {
                            if (const auto inv = obj->GetInventory([](const RE::TESBoundObject& item) {
                                return true;
                            }); !inv.empty())
                                return RE::BSEventNotifyControl::kContinue;
                        }

                        if (obj == Utility::last_activation) {
                            obj->SetActivationBlocked(false);
                            Utility::last_activation = nullptr;
                        } else {
                            if (Utility::last_activation) {
                                Utility::last_activation->SetActivationBlocked(false);
                                Utility::last_activation = nullptr;
                            }
                            obj->SetActivationBlocked(true);
                            Utility::last_activation = obj;
                        }
                    }
                }
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    void OnActivateEventHandler::Register() {
        const auto holder = RE::ScriptEventSourceHolder::GetSingleton();
        holder->AddEventSink(GetSingleton());
        logger::info("Registered activate event handler");
    }
}
