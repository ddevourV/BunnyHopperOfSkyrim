#include "Events.h"
#include "version.h"
#include "Strafe.h"
#include "Settings.h"

#include "RE/Skyrim.h"
#include "SKSE/API.h"
#include <SKSE/SKSE.h>

namespace
{
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

		TESObjectLoadedHandler(const TESObjectLoadedHandler&) = delete;
		TESObjectLoadedHandler(TESObjectLoadedHandler&&) = delete;
		TESObjectLoadedHandler& operator=(const TESObjectLoadedHandler&) = delete;
		TESObjectLoadedHandler& operator=(TESObjectLoadedHandler&&) = delete;

	protected:
		TESObjectLoadedHandler() = default;
		~TESObjectLoadedHandler() override = default;
	};

	void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type) {
		case SKSE::MessagingInterface::kDataLoaded:
		{
			auto sourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
			if (sourceHolder) {
				sourceHolder->AddEventSink<RE::TESObjectLoadedEvent>(TESObjectLoadedHandler::GetSingleton());
			}
		}
		break;
		default:
			break;
		}
	}
}

// ✅ New-style SKSE plugin version data (required for AE / SKSE 2.2.x)
extern "C" DLLEXPORT constinit SKSE::PluginVersionData SKSEPlugin_Version = []() {
	SKSE::PluginVersionData v{};
	v.PluginVersion({ BHOS_VERSION_MAJOR, BHOS_VERSION_MINOR, BHOS_VERSION_PATCH });
	v.PluginName("BunnyHopperOfSkyrim");
	v.AuthorName("gottyduke + ddevourV");
	v.UsesAddressLibrary(true);
	v.UsesNoStructs(true);
	return v;
}();

// ✅ New-style load entrypoint
extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	// CommonLibSSE-NG init
	SKSE::Init(a_skse);

	// old-style logger macros from SKSE/API.h are still used in the project
	_MESSAGE("BunnyHopperOfSkyrim loaded (AE entrypoint)");

	// Keep original init if present in the project
	if (!Init(a_skse)) {
		return false;
	}

	const auto messaging = SKSE::GetMessagingInterface();
	if (messaging && messaging->RegisterListener("SKSE", MessageHandler)) {
		_MESSAGE("Messaging interface registration successful");
	} else {
		_FATALERROR("Messaging interface registration failed!\n");
		return false;
	}

	if (Settings::LoadSettings()) {
		_MESSAGE("Settings successfully loaded");
	} else {
		_FATALERROR("Settings failed to load!\n");
		return false;
	}

	return true;
}
