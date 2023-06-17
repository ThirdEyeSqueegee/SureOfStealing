#include "Events.h"
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
            if (const auto& obj = a_event->objectActivated; obj->IsCrimeToActivate()) {
                if (obj->GetName() == "Door" || obj->GetName() == "Large Wooden Gate") return RE::BSEventNotifyControl::kContinue;

                logger::info("Attempting to steal {}", obj->GetName());

                const auto base_obj = obj->GetObjectReference();

                if (const auto found = Utility::objects.find(base_obj); found != Utility::objects.end()) {
                    Utility::objects.erase(found);
                    obj->SetActivationBlocked(false);
                } else {
                    Utility::objects.insert(base_obj);
                    obj->SetActivationBlocked(true);
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
