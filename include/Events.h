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

    class InputEventHandler : public RE::BSTEventSink<RE::InputEvent*> {
    protected:
        InputEventHandler() = default;

    public:
        InputEventHandler(const InputEventHandler&) = delete;
        InputEventHandler(InputEventHandler&&) = delete;
        InputEventHandler& operator=(const InputEventHandler&) = delete;
        InputEventHandler& operator=(InputEventHandler&&) = delete;

        static InputEventHandler* GetSingleton();

        RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource) override;

        static void Register();
    };

    class CrosshairEventHandler : public RE::BSTEventSink<SKSE::CrosshairRefEvent> {
    protected:
        CrosshairEventHandler() = default;

    public:
        CrosshairEventHandler(const CrosshairEventHandler&) = delete;
        CrosshairEventHandler(CrosshairEventHandler&&) = delete;
        CrosshairEventHandler& operator=(const CrosshairEventHandler&) = delete;
        CrosshairEventHandler& operator=(CrosshairEventHandler&&) = delete;

        static CrosshairEventHandler* GetSingleton();

        RE::BSEventNotifyControl ProcessEvent(const SKSE::CrosshairRefEvent* a_event,
                                              RE::BSTEventSource<SKSE::CrosshairRefEvent>* a_eventSource) override;

        static void Register();
    };
}
