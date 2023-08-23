#pragma once

namespace Hooks {
    void Install();

    class PickupObject : public Singleton<PickupObject> {
    public:
        static void Thunk(RE::PlayerCharacter* a_this, RE::TESObjectREFR* a_object, uint32_t a_count, bool a_arg3, bool a_playSound);

        inline static REL::Relocation<decltype(&Thunk)> func;
        
        inline static std::size_t idx = 204; // 0xcc
    };

    class Activate : public Singleton<Activate> {
    public:
        static bool Thunk(RE::TESFlora* a_this, RE::TESObjectREFR* a_targetRef, RE::TESObjectREFR* a_activatorRef, std::uint8_t a_arg3,
                          RE::TESBoundObject* a_object, std::int32_t a_targetCount);

        inline static REL::Relocation<decltype(&Thunk)> func;

        inline static constexpr std::size_t idx = 55; // 0x37
    };
}