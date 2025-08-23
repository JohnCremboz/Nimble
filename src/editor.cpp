#include "editor.h"
#include "pluginmanager.h"
#include <ncurses.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// global keymap definition
std::map<std::string, int> keymap;

// Global function to load keybindings
void loadKeybindings(const std::string& path) {
    keymap.clear();
    std::ifstream f(path);
    if (!f.is_open()) return;
    json j;
    f >> j;
    auto parse = [](const std::string& s) -> int {
        if (s.rfind("ctrl+", 0) == 0 && s.size() == 6) {
            char c = std::tolower(s[5]);
            return c - 'a' + 1;
        }
        return 0;
    };
    for (auto& [action, key]: j.items()) {
        keymap[action] = parse(key.get<std::string>());
    }
}

extern std::map<std::string, std::map<std::string, std::string>> translations;
extern std::vector<Theme> themes;

Editor::Editor(const std::string& l, int tIdx) : lang(l), themeIdx(tIdx) {}

void Editor::undo() {
    if (!undoStack.empty()) {
        redoStack.push_back(lines);
        lines = undoStack.back();
        undoStack.pop_back();
        modified = true;
    }
}

void Editor::redo() {
    if (!redoStack.empty()) {
        undoStack.push_back(lines);
        lines = redoStack.back();
        redoStack.pop_back();
        modified = true;
    }
}

void Editor::drawStatusBar() {
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    move(maxY-1, 0);
    clrtoeol();
    std::string fileLabel = translations[lang].count("file") ? translations[lang]["file"] : "Bestand";
    std::string posLabel = translations[lang].count("pos") ? translations[lang]["pos"] : "Pos";
    std::string helpLabel = translations[lang].count("help") ? translations[lang]["help"] : "Ctrl+H Help";
    std::string modMark = modified ? "*" : " ";
    mvprintw(maxY-1, 0, "%s: %s%s | %s: %d,%d | %s", 
             fileLabel.c_str(), 
             filename.empty() ? "Nieuw" : filename.c_str(),
             modMark.c_str(),
             posLabel.c_str(), cursorY+1, cursorX+1,
             helpLabel.c_str());
}

void Editor::drawTitleBar() {
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    move(0, 0);
    clrtoeol();
    std::string title = " Nimble Editor ";
    std::string fname = filename.empty() ? "[Nieuw bestand]" : filename;
    std::string bar = title + "- " + fname;
    // Truncate if too long
    if ((int)bar.size() > maxX) bar = bar.substr(0, maxX);
    attron(A_REVERSE);
    mvprintw(0, 0, "%s", bar.c_str());
    attroff(A_REVERSE);
}

bool Editor::openFile(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) return false;
    
    lines.clear();
    std::string line;
    while (std::getline(f, line)) {
        lines.push_back(line);
    }
    if (lines.empty()) lines.push_back("");
    
    filename = path;
    cursorX = cursorY = 0;
    modified = false;
    return true;
}

bool Editor::saveFile(const std::string& path) {
    std::string savePath = path.empty() ? filename : path;
    if (savePath.empty()) return false;
    
    std::ofstream f(savePath);
    if (!f.is_open()) return false;
    
    for (const auto& line : lines) {
        f << line << '\n';
    }
    
    filename = savePath;
    modified = false;
    return true;
}

void Editor::draw() {
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    clear();

    // Titelbalk
    drawTitleBar();

    // Tekstregels tussen titel- en statusbalk
    int contentRows = maxY - 2; // 1 voor titel, 1 voor status
    for (size_t i = 0; i < lines.size() && (int)i < contentRows; ++i) {
        mvprintw(i + 1, 0, "%s", lines[i].c_str());
    }

    // Statusbalk
    drawStatusBar();

    // Cursorpositie binnen content
    int cy = cursorY + 1;
    if (cy >= maxY - 1) cy = maxY - 2;
    move(cy, cursorX);
    refresh();
}

void Editor::processInput(int ch) {
    switch (ch) {
        case KEY_UP:
            if (cursorY > 0) cursorY--;
            break;
        case KEY_DOWN:
            if (cursorY + 1 < (int)lines.size()) cursorY++;
            break;
        case KEY_LEFT:
            if (cursorX > 0) cursorX--;
            break;
        case KEY_RIGHT:
            if (cursorX < (int)lines[cursorY].size()) cursorX++;
            break;
        case 10: case 13: // Enter
            {
                std::string rest = lines[cursorY].substr(cursorX);
                lines[cursorY] = lines[cursorY].substr(0, cursorX);
                lines.insert(lines.begin() + cursorY + 1, rest);
                cursorY++;
                cursorX = 0;
                modified = true;
            }
            break;
        case KEY_BACKSPACE: case 127:
            if (cursorX > 0) {
                lines[cursorY].erase(cursorX - 1, 1);
                cursorX--;
                modified = true;
            } else if (cursorY > 0) {
                cursorX = lines[cursorY - 1].size();
                lines[cursorY - 1] += lines[cursorY];
                lines.erase(lines.begin() + cursorY);
                cursorY--;
                modified = true;
            }
            break;
        default:
            if (ch >= 32 && ch < 127) { // Printable characters
                lines[cursorY].insert(cursorX, 1, ch);
                cursorX++;
                modified = true;
            }
            break;
    }
}

void Editor::searchPrompt() {
    // Basic search implementation
}

void Editor::replacePrompt() {
    // Basic replace implementation  
}

void Editor::helpPrompt() {
    // Basic help implementation
}

void Editor::csvMenuPrompt() {
    echo();
    curs_set(1);
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    
    mvprintw(0, 0, "CSV Menu - Press any key to continue");
    getch();
    
    noecho();
    curs_set(0);
    draw();
}

void Editor::run() {
    loadKeybindings();
    
    while (true) {
        draw();
        int ch = getch();
        
        if (ch == keymap["exit"]) {
            if (modified) {
                mvprintw(0, 0, "Save changes? (y/n)");
                refresh();
                int save = getch();
                if (save == 'y' || save == 'Y') {
                    saveFile();
                }
            }
            break;
        } else if (ch == keymap["save"]) {
            saveFile();
        } else if (ch == keymap["search"]) {
            searchPrompt();
        } else if (ch == keymap["help"]) {
            helpPrompt();
        } else if (ch == keymap["replace"]) {
            replacePrompt();
        } else if (ch == keymap["undo"]) {
            undo();
        } else if (ch == keymap["redo"]) {
            redo();
        } else {
            processInput(ch);
        }
    }
}
