#include "plugin.h"
#include <iostream>

class ExamplePlugin : public Plugin {
public:
    std::string name() const override { return "ExamplePlugin"; }
    void onLoad() override { std::cout << "[Plugin] Example geladen!\n"; }
    void onUnload() override { std::cout << "[Plugin] Example ontladen!\n"; }
};

extern "C" Plugin* createPlugin() {
    return new ExamplePlugin();
}
