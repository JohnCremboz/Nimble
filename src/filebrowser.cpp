#include "filebrowser.h"
#include <ncurses.h>
#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

FileBrowser::FileBrowser() {
    currentPath = fs::current_path().string();
    listDir(currentPath);
}

void FileBrowser::listDir(const std::string& path) {
    entries.clear();
    for (const auto& entry : fs::directory_iterator(path)) {
        entries.push_back(entry.path().filename().string());
    }
}

void FileBrowser::draw() {
    clear();
    mvprintw(0, 0, "Map: %s", currentPath.c_str());
    for (size_t i = 0; i < entries.size(); ++i) {
        if ((int)i == selected) attron(A_REVERSE);
        mvprintw(i+1, 2, "%s", entries[i].c_str());
        if ((int)i == selected) attroff(A_REVERSE);
    }
    mvprintw(entries.size()+2, 0, "Enter: open | pijl omhoog/omlaag: navigeren | q: terug");
    refresh();
}

std::string FileBrowser::run() {
    int ch;
    while (true) {
        draw();
        ch = getch();
        if (ch == 'q') return "";
        else if (ch == KEY_UP && selected > 0) selected--;
        else if (ch == KEY_DOWN && selected + 1 < (int)entries.size()) selected++;
        else if (ch == 10 && !entries.empty()) {
            std::string sel = entries[selected];
            std::string fullPath = currentPath + "/" + sel;
            if (fs::is_directory(fullPath)) {
                currentPath = fullPath;
                listDir(currentPath);
                selected = 0;
            } else {
                return fullPath;
            }
        }
    }
}
