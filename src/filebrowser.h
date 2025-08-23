#pragma once
#include <string>
#include <vector>

class FileBrowser {
public:
    FileBrowser();
    std::string run(); // Geeft gekozen pad terug
    std::string currentPath;
    std::vector<std::string> entries;
    int selected = 0;
    void listDir(const std::string& path);
    void draw();
};
