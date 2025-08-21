#pragma once
#include <string>

class Plugin {
public:
    virtual ~Plugin() = default;
    virtual std::string name() const = 0;
    virtual void onLoad() = 0;
    virtual void onUnload() = 0;
    // Breid uit met hooks zoals onSave, onOpen, onKeyPress, ...
};
