#include "pluginmanager.h"
#include <filesystem>
#include <dlfcn.h> // Voor Linux, voor Windows: gebruik LoadLibrary
#include <iostream>

void PluginManager::loadPlugins() {
    // Zoek .so/.dll plugins in ./plugins
    std::string dir = "plugins";
    if (!std::filesystem::exists(dir)) return;
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        if (entry.path().extension() == ".so" || entry.path().extension() == ".dll") {
            void* handle = dlopen(entry.path().c_str(), RTLD_LAZY);
            if (!handle) continue;
            using create_t = Plugin* (*)();
            create_t create = (create_t)dlsym(handle, "createPlugin");
            if (create) {
                plugins.emplace_back(create());
                plugins.back()->onLoad();
            }
        }
    }
}

void PluginManager::unloadPlugins() {
    for (auto& p : plugins) {
        p->onUnload();
    }
    plugins.clear();
}
