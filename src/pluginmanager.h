#pragma once
#include <vector>
#include <memory>
#include "plugin.h"

class PluginManager {
public:
    void loadPlugins();
    void unloadPlugins();
    std::vector<std::unique_ptr<Plugin>> plugins;
};
