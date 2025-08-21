#pragma once
#include <string>

class Plugin {
public:
    virtual ~Plugin() = default;
    virtual std::string name() const = 0;
    virtual void onLoad() = 0;
    virtual void onUnload() = 0;
    // Event/callback API:
    virtual void onFileOpen(const std::string& filename) {}
    virtual void onFileSave(const std::string& filename) {}
    virtual void onKeyPress(int key) {}
    virtual void onEditorDraw() {}
};
