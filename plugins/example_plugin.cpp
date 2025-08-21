#include "plugin.h"
#include <iostream>

class ExamplePlugin : public Plugin {
public:
    std::string name() const override { return "ExamplePlugin"; }
    void onLoad() override { std::cout << "[Plugin] Example geladen!\n"; }
    void onUnload() override { std::cout << "[Plugin] Example ontladen!\n"; }
    void onFileOpen(const std::string& filename) override {
        std::cout << "[Plugin] Bestand geopend: " << filename << "\n";
    }
    void onFileSave(const std::string& filename) override {
        std::cout << "[Plugin] Bestand opgeslagen: " << filename << "\n";
    }
    void onKeyPress(int key) override {
        std::cout << "[Plugin] Key pressed: " << key << "\n";
    }
    void onEditorDraw() override {
        // Kan gebruikt worden voor overlays, logging, enz.
    }
};

extern "C" Plugin* createPlugin() {
    return new ExamplePlugin();
}
