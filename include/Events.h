#pragma once

namespace Events {
    class OnActivateEventHandler : public RE::BSTEventSink<RE::TESActivateEvent> {
    protected:
        OnActivateEventHandler() = default;

    public:
        OnActivateEventHandler(const OnActivateEventHandler&) = delete;
        OnActivateEventHandler(OnActivateEventHandler&&) = delete;
        OnActivateEventHandler& operator=(const OnActivateEventHandler&) = delete;
        OnActivateEventHandler& operator=(OnActivateEventHandler&&) = delete;

        static OnActivateEventHandler* GetSingleton();

        RE::BSEventNotifyControl ProcessEvent(const RE::TESActivateEvent* a_event, RE::BSTEventSource<RE::TESActivateEvent>* a_eventSource) override;

        static void Register();
    };
}
