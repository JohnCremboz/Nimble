#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <cctype>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
void Editor::loadKeybindings(const std::string& path) {
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

#include "editor.h"
#include <ncurses.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>
#include <map>
#include <string>


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
    std::string helpLabel = translations[lang].count("help") ? translations[lang]["help"] : "Ctrl+G Help";
    mvprintw(maxY-1, 0, "%s: %s %s | %s: %d,%d | %s", fileLabel.c_str(), filename.c_str(), modified ? "*" : "", posLabel.c_str(), cursorY+1, cursorX+1, helpLabel.c_str());
}

void Editor::helpPrompt() {
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    echo();
    curs_set(1);
    std::string help = translations[lang].count("helptxt") ? translations[lang]["helptxt"] : "Sneltoetsen: Ctrl+O Opslaan | Ctrl+X Afsluiten | Ctrl+W Zoeken | Ctrl+R Vervangen | Ctrl+Z Undo | Ctrl+Y Redo | PgUp/PgDn Scroll | Ctrl+G Sluiten";
    mvprintw(maxY-6, 0, "%s", help.c_str());
    mvprintw(maxY-5, 0, "Plugin-ondersteuning: plaats .so/.dll in ./plugins, interface zie README");
    mvprintw(maxY-4, 0, "Voorbeeldplugin: plugins/example_plugin.cpp");
    // About-scherm
    std::string about = "Nimble Editor v0.01  Auteur: JohnCremboz";
    mvprintw(maxY-2, 0, "%s", about.c_str());
    mvprintw(maxY-1, 0, "Druk op een toets om te sluiten / Press any key to close");
    getch();
    noecho();
    curs_set(0);
}

void Editor::search(const std::string& term) {
    if (term.empty()) return;
    lastSearchTerm = term;
    for (size_t y = cursorY; y < lines.size(); ++y) {
        size_t x = (y == cursorY) ? cursorX + 1 : 0;
        size_t found = lines[y].find(term, x);
        if (found != std::string::npos) {
            cursorY = y;
            cursorX = found;
            return;
        }
    }
    // Niet gevonden, wrap naar boven
    for (size_t y = 0; y <= cursorY; ++y) {
        size_t x = 0;
        size_t found = lines[y].find(term, x);
        if (found != std::string::npos) {
            cursorY = y;
            cursorX = found;
            return;
        }
    }
}

void Editor::replace(const std::string& term, const std::string& replacement, bool all) {
    if (term.empty()) return;
    bool replaced = false;
    if (all) {
        for (auto& line : lines) {
            size_t pos = 0;
            while ((pos = line.find(term, pos)) != std::string::npos) {
                line.replace(pos, term.length(), replacement);
                pos += replacement.length();
                replaced = true;
            }
        }
    } else {
        for (size_t y = cursorY; y < lines.size(); ++y) {
            size_t x = (y == cursorY) ? cursorX : 0;
            size_t found = lines[y].find(term, x);
            if (found != std::string::npos) {
                lines[y].replace(found, term.length(), replacement);
                cursorY = y;
                cursorX = found + replacement.length();
                replaced = true;
                break;
            }
        }
    }
    if (replaced) modified = true;
}

void Editor::searchPrompt() {
    echo();
    curs_set(1);
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    char buf[256] = {0};
    std::string prompt = translations[lang].count("search") ? translations[lang]["search"] : "/Zoeken: ";
    mvprintw(maxY-1, 0, "%s", prompt.c_str());
    getnstr(buf, 255);
    noecho();
    curs_set(0);
    search(std::string(buf));
}

void Editor::replacePrompt() {
    echo();
    curs_set(1);
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    char findbuf[256] = {0};
    char repbuf[256] = {0};
    std::string prompt1 = translations[lang].count("replace") ? translations[lang]["replace"] : "/Vervang: ";
    std::string prompt2 = translations[lang].count("replacewith") ? translations[lang]["replacewith"] : "/Vervang door: ";
    mvprintw(maxY-1, 0, "%s", prompt1.c_str());
    getnstr(findbuf, 255);
    mvprintw(maxY-1, 0, "%s", prompt2.c_str());
    getnstr(repbuf, 255);
    noecho();
    curs_set(0);
    replace(std::string(findbuf), std::string(repbuf));
}

bool Editor::openFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return false;
    lines.clear();
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    filename = path;
    cursorX = cursorY = 0;
    modified = false;
    return true;
}

bool Editor::saveFile(const std::string& path) {
    std::string outPath = path.empty() ? filename : path;
    if (outPath.empty()) return false;
    std::ofstream file(outPath);
    if (!file.is_open()) return false;
    for (const auto& line : lines) {
        file << line << "\n";
    }
    modified = false;
    return true;
}

void Editor::draw() {
    clear();
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    int linenoWidth = 1;
    if (!lines.empty()) {
        linenoWidth = std::to_string(lines.size()).size();
    }
    // Init kleuren voor highlighting
    start_color();
    Theme& th = themes[themeIdx];
    init_pair(1, th.keyword, th.bg); // keywords
    init_pair(2, th.stringc, th.bg); // strings
    init_pair(3, th.fg, th.bg); // gewone tekst
    std::vector<std::string> keywords = {"int","float","double","if","else","for","while","return","void","class","public","private","protected","include","define","namespace","using","std"};
    for (size_t i = 0; i < lines.size() && i < (size_t)maxY - 1; ++i) {
        attron(COLOR_PAIR(3));
        mvprintw(i, 0, "%*zu ", linenoWidth, i + 1);
        attroff(COLOR_PAIR(3));
        int x = linenoWidth + 1;
        std::string line = lines[i];
        size_t pos = 0;
        while (pos < line.size()) {
            // Highlight strings
            if (line[pos] == '"') {
                attron(COLOR_PAIR(2));
                mvaddch(i, x++, line[pos++]);
                while (pos < line.size() && line[pos] != '"') {
                    mvaddch(i, x++, line[pos++]);
                }
                if (pos < line.size()) mvaddch(i, x++, line[pos++]);
                attroff(COLOR_PAIR(2));
                continue;
            }
            // Highlight keywords
            bool matched = false;
            for (const auto& kw : keywords) {
                size_t len = kw.size();
                if (line.compare(pos, len, kw) == 0 && (pos == 0 || !isalnum(line[pos-1])) && (pos+len == line.size() || !isalnum(line[pos+len]))) {
                    attron(COLOR_PAIR(1));
                    for (size_t k = 0; k < len; ++k) mvaddch(i, x++, line[pos++]);
                    attroff(COLOR_PAIR(1));
                    matched = true;
                    break;
                }
            }
            if (!matched) {
                attron(COLOR_PAIR(3));
                mvaddch(i, x++, line[pos++]);
                attroff(COLOR_PAIR(3));
            }
        }
    }
    move(cursorY, cursorX + linenoWidth + 1);
    drawStatusBar();
    refresh();
}

void Editor::processInput(int ch) {
    if (ch == KEY_UP) {
        if (cursorY > 0) cursorY--;
    } else if (ch == KEY_DOWN) {
        if (cursorY + 1 < (int)lines.size()) cursorY++;
    } else if (ch == KEY_LEFT) {
        if (cursorX > 0) cursorX--;
    } else if (ch == KEY_RIGHT) {
        if (cursorX < (int)lines[cursorY].size()) cursorX++;
    } else if (ch == KEY_NPAGE) { // PageDown
        cursorY += 10;
        if (cursorY >= (int)lines.size()) cursorY = lines.size() - 1;
    } else if (ch == KEY_PPAGE) { // PageUp
        cursorY -= 10;
        if (cursorY < 0) cursorY = 0;
    } else if (ch == 10) { // Enter
        undoStack.push_back(lines);
        redoStack.clear();
        std::string rest = lines[cursorY].substr(cursorX);
        lines[cursorY] = lines[cursorY].substr(0, cursorX);
        lines.insert(lines.begin() + cursorY + 1, rest);
        cursorY++;
        cursorX = 0;
        modified = true;
    } else if (ch == KEY_BACKSPACE || ch == 127) {
        if (cursorX > 0) {
            undoStack.push_back(lines);
            redoStack.clear();
            lines[cursorY].erase(cursorX - 1, 1);
            cursorX--;
            modified = true;
        } else if (cursorY > 0) {
            undoStack.push_back(lines);
            redoStack.clear();
            cursorX = lines[cursorY - 1].size();
            lines[cursorY - 1] += lines[cursorY];
            lines.erase(lines.begin() + cursorY);
            cursorY--;
            modified = true;
        }
    } else if (isprint(ch)) {
        undoStack.push_back(lines);
        redoStack.clear();
        lines[cursorY].insert(cursorX, 1, (char)ch);
        cursorX++;
        modified = true;
    }
}

void Editor::run() {
    if (lines.empty()) lines = {""};
    loadKeybindings();
    draw();
    int ch;
    while (true) {
        ch = getch();
        if (ch == keymap["exit"]) {
            if (modified) {
                std::string msg = translations[lang].count("unsaved") ? translations[lang]["unsaved"] : "Niet-opgeslagen wijzigingen! Druk op y om op te slaan, n om te negeren.";
                mvprintw(0, 0, "%s", msg.c_str());
                int c = getch();
                if (c == 'y') saveFile("");
            }
            break;
        } else if (ch == keymap["save"]) {
            saveFile("");
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
        draw();
    }
}
