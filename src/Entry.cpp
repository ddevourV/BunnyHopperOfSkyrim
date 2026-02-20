#include <SKSE/SKSE.h>
#include <RE/Skyrim.h>

#include <spdlog/sinks/basic_file_sink.h>

static void SetupLog()
{
    auto log = spdlog::basic_logger_mt("BunnyHopperOfSkyrim", "Data/SKSE/Plugins/BunnyHopperOfSkyrim.log", true);
    spdlog::set_default_logger(log);
    spdlog::set_pattern("[%H:%M:%S.%e] [%l] %v");
    spdlog::set_level(spdlog::level::info);
}

extern "C" DLLEXPORT constinit SKSE::PluginVersionData SKSEPlugin_Version = []() {
    SKSE::PluginVersionData v{};
    v.PluginVersion({ 1, 0, 0 });
    v.PluginName("BunnyHopperOfSkyrim");
    v.AuthorName("gottyduke + ddevourV");
    v.UsesAddressLibrary(true);
    v.UsesNoStructs(true);
    return v;
}();

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* skse)
{
    SKSE::Init(skse);
    SetupLog();
    spdlog::info("BunnyHopperOfSkyrim loaded (AE 1.6.1170+ entrypoint).");

    // IMPORTANT:
    // Здесь нужно вызвать установку хуков/инициализацию из оригинального кода проекта.
    // Ниже оставляю заглушку — мы подключим реальный вызов после первого успешного билда.
    if (auto* msg = SKSE::GetMessagingInterface()) {
        msg->RegisterListener([](SKSE::MessagingInterface::Message* m) {
            if (m->type == SKSE::MessagingInterface::kDataLoaded) {
                spdlog::info("DataLoaded event.");
            }
        });
    }

    return true;
}
