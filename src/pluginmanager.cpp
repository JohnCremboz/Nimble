#include "pluginmanager.h"
#include <filesystem>
#if defined(_WIN32)
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <iostream>

void PluginManager::loadPlugins() {
    // Zoek .so/.dll plugins in ./plugins
    std::string dir = "plugins";
    if (!std::filesystem::exists(dir)) return;
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        auto ext = entry.path().extension().string();
#if defined(_WIN32)
        if (ext == ".dll") {
            HMODULE handle = LoadLibraryA(entry.path().string().c_str());
            if (!handle) continue;
            using create_t = Plugin* (*)();
            create_t create = (create_t)GetProcAddress(handle, "createPlugin");
            if (create) {
                plugins.emplace_back(create());
                plugins.back()->onLoad();
            }
        }
#else
        if (ext == ".so" || ext == ".dylib") {
            void* handle = dlopen(entry.path().c_str(), RTLD_LAZY);
            if (!handle) continue;
            using create_t = Plugin* (*)();
            create_t create = (create_t)dlsym(handle, "createPlugin");
            if (create) {
                plugins.emplace_back(create());
                plugins.back()->onLoad();
            }
        }
#endif
    }
}

void PluginManager::unloadPlugins() {
    for (auto& p : plugins) {
        p->onUnload();
    }
    plugins.clear();
}
