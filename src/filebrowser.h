#pragma once
#include <string>

class FileBrowser {
public:
    FileBrowser();
    std::string run(); // Geeft gekozen pad terug
private:
    std::string currentPath;
    std::vector<std::string> entries;
    int selected = 0;
    void listDir(const std::string& path);
    void draw();
};
