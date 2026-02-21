#ifndef DLLEXPORT
#  ifdef _WIN32
#    define DLLEXPORT __declspec(dllexport)
#  else
#    define DLLEXPORT
#  endif
#endif

#include "Events.h"
#include "version.h"
#include "Strafe.h"
#include "Settings.h"

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

#include <spdlog/sinks/basic_file_sink.h>

namespace
{
    void SetupLog()
    {
        auto log = spdlog::basic_logger_mt("BunnyHopperOfSkyrim", "Data/SKSE/Plugins/BunnyHopperOfSkyrim.log", true);
        spdlog::set_default_logger(log);
        spdlog::set_pattern("[%H:%M:%S.%e] [%l] %v");
        spdlog::set_level(spdlog::level::info);
    }

    class TESObjectLoadedHandler final : public RE::BSTEventSink<RE::TESObjectLoadedEvent>
    {
    public:
        using EventResult = RE::BSEventNotifyControl;

        static TESObjectLoadedHandler* GetSingleton()
        {
            static TESObjectLoadedHandler singleton;
            return std::addressof(singleton);
        }

        auto ProcessEvent(const RE::TESObjectLoadedEvent* a_event, RE::BSTEventSource<RE::TESObjectLoadedEvent>*)
            -> EventResult override
        {
            if (!a_event) {
                return EventResult::kContinue;
            }

            const auto player = RE::PlayerCharacter::GetSingleton();
            if (player && a_event->formID == player->formID) {
                AnimationGraphEventHandler(Events::BHopHandler::GetSingleton());
            }

            return EventResult::kContinue;
        }
    };

    void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
    {
        if (!a_msg) {
            return;
        }

        if (a_msg->type == SKSE::MessagingInterface::kDataLoaded) {
            auto sourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
            if (sourceHolder) {
                sourceHolder->AddEventSink<RE::TESObjectLoadedEvent>(TESObjectLoadedHandler::GetSingleton());
            }

            Settings::LoadSettings();
            spdlog::info("DataLoaded: sinks registered.");
        }
    }
}

extern "C" DLLEXPORT constinit SKSE::PluginVersionData SKSEPlugin_Version = []() {
    SKSE::PluginVersionData v{};
    v.PluginVersion({ BHOS_VERSION_MAJOR, BHOS_VERSION_MINOR, BHOS_VERSION_PATCH });
    v.PluginName("BunnyHopperOfSkyrim");
    v.AuthorName("gottyduke + ddevourV");
    v.UsesAddressLibrary(true);
    v.UsesNoStructs(true);
    return v;
}();

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
    SKSE::Init(a_skse);
    SetupLog();

    spdlog::info("BunnyHopperOfSkyrim loaded (AE).");

    if (auto* messaging = SKSE::GetMessagingInterface()) {
        messaging->RegisterListener(MessageHandler);
        return true;
    }

    spdlog::error("Messaging interface not available.");
    return false;
}
