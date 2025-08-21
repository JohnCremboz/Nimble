// Uitgebreid interactief CSV-menu (Ctrl+M)
void Editor::csvMenuPrompt() {
    echo();
    curs_set(1);
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    char buf[64] = {0};
    // Kolomnamen ophalen via plugin
    std::vector<std::string> colnames;
    extern PluginManager pluginManager;
    for (auto& p : pluginManager.plugins) {
        if (p->name().find("CSV") != std::string::npos) {
            std::string tmp = "__nimble_tmp_csv__.csv";
            std::ofstream f(tmp);
            for (const auto& l : lines) f << l << "\n";
            f.close();
            char delim = ',';
            std::ifstream file(tmp);
            std::string header;
            if (std::getline(file, header)) {
                std::stringstream ss(header);
                std::string cell;
                while (std::getline(ss, cell, ',')) colnames.push_back(cell);
            }
            remove(tmp.c_str());
            break;
        }
    }
    bool done = false;
    while (!done) {
        clear();
        mvprintw(0, 0, "CSV-menu: sorteren/filteren/statistieken. Enter voor volgende optie, q=sluiten, r=reset");
        mvprintw(1, 0, "Kolommen: ");
        for (size_t i = 0; i < colnames.size(); ++i) {
            mvprintw(2, (int)(i*16), "%zu: %s", i, colnames[i].c_str());
        }
        mvprintw(4, 0, "Huidige sortering: kolom %d (%s)", csvSortCol, csvSortAsc ? "oplopend" : "aflopend");
        mvprintw(5, 0, "Huidige filter: '%s'", csvFilter.c_str());
        mvprintw(6, 0, "Statistieken tonen: %s", csvShowStats ? "ja" : "nee");
        mvprintw(8, 0, "Opties: s=sorteren, f=filter, t=statistieken, r=reset, q=sluiten, p=preview");
        int c = getch();
        if (c == 'q') { done = true; break; }
        if (c == 'r') { csvSortCol = -1; csvFilter.clear(); csvShowStats = false; continue; }
        if (c == 's') {
            mvprintw(10, 0, "Sorteren op kolomnummer: ");
            getnstr(buf, 63);
            int col = atoi(buf);
            if (col >= 0) csvSortCol = col;
            else csvSortCol = -1;
            mvprintw(11, 0, "Oplopend (1) of aflopend (0): ");
            getnstr(buf, 63);
            csvSortAsc = (buf[0] == '1');
        }
        if (c == 'f') {
            mvprintw(12, 0, "Filter (leeg=geen): ");
            getnstr(buf, 63);
            csvFilter = buf;
        }
        if (c == 't') {
            mvprintw(13, 0, "Statistieken tonen? (1/0): ");
            getnstr(buf, 63);
            csvShowStats = (buf[0] == '1');
        }
        if (c == 'p') {
            // Live preview
            draw();
            mvprintw(maxY-1, 0, "Druk op een toets om terug te keren naar menu");
            getch();
        }
    }
    noecho();
    curs_set(0);
    draw();
}
#include "pluginmanager.h"
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
    // Plugin event: onFileOpen
    extern PluginManager pluginManager;
    for (auto& p : pluginManager.plugins) {
        if (p) p->onFileOpen(path);
    }
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
    // Plugin event: onFileSave
    extern PluginManager pluginManager;
    for (auto& p : pluginManager.plugins) {
        if (p) p->onFileSave(outPath);
    }
    return true;
    // Plugin event: onKeyPress
    extern PluginManager pluginManager;
    for (auto& p : pluginManager.plugins) {
        if (p) p->onKeyPress(ch);
    }
}

void Editor::draw() {
    // Plugin event: onEditorDraw
    extern PluginManager pluginManager;
    for (auto& p : pluginManager.plugins) {
        if (p) p->onEditorDraw();
    }
    clear();
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    // Markdown/csv-detectie
    bool isMarkdown = false, isCsv = false;
    if (!filename.empty() && filename.size() > 3 && filename.substr(filename.size()-3) == ".md") isMarkdown = true;
    if (!filename.empty() && filename.size() > 4 && filename.substr(filename.size()-4) == ".csv") isCsv = true;
    extern PluginManager pluginManager;
    if (isMarkdown) {
        for (auto& p : pluginManager.plugins) {
            if (p->name().find("Markdown") != std::string::npos) {
                std::string tmp = "__nimble_tmp_md__.md";
                std::ofstream f(tmp);
                for (const auto& l : lines) f << l << "\n";
                f.close();
                fflush(stdout);
                FILE* old = freopen("__nimble_md_out__.txt", "w", stdout);
                ((MarkdownPlugin*)p.get())->printWithHighlight(tmp);
                fflush(stdout);
                if (old) freopen("CON", "w", stdout);
                std::ifstream fin("__nimble_md_out__.txt");
                std::string l; int y=0;
                while (std::getline(fin, l) && y < maxY-1) {
                    mvprintw(y++, 0, "%s", l.c_str());
                }
                remove(tmp.c_str());
                remove("__nimble_md_out__.txt");
                break;
            }
        }
    } else if (isCsv) {
        for (auto& p : pluginManager.plugins) {
            if (p->name().find("CSV") != std::string::npos) {
                std::string tmp = "__nimble_tmp_csv__.csv";
                std::ofstream f(tmp);
                for (const auto& l : lines) f << l << "\n";
                f.close();
                fflush(stdout);
                FILE* old = freopen("__nimble_csv_out__.txt", "w", stdout);
                // Cast naar juiste type en pretty print
                struct CsvPlugin { void printTable(const std::string&); };
                ((CsvPlugin*)p.get())->printTable(tmp);
                fflush(stdout);
                if (old) freopen("CON", "w", stdout);
                std::ifstream fin("__nimble_csv_out__.txt");
                std::string l; int y=0;
                while (std::getline(fin, l) && y < maxY-1) {
                    mvprintw(y++, 0, "%s", l.c_str());
                }
                remove(tmp.c_str());
                remove("__nimble_csv_out__.txt");
                break;
            }
        }
    } else {
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
                    extern PluginManager pluginManager;
                    try {
                        std::string user = getenv("USERNAME") ? getenv("USERNAME") : "user";
                        std::string pid = std::to_string(GetCurrentProcessId());
                        if (isMarkdown) {
                            bool found = false;
                            for (auto& p : pluginManager.plugins) {
                                if (p && p->name().find("Markdown") != std::string::npos) {
                                    found = true;
                                    std::string tmp = "__nimble_tmp_md_" + user + "_" + pid + ".md";
                                    std::ofstream f(tmp);
                                    if (!f.is_open()) { mvprintw(0,0,"[Fout] Kan temp-bestand niet schrijven"); return; }
                                    for (const auto& l : lines) f << l << "\n";
                                    f.close();
                                    fflush(stdout);
                                    std::string outFile = "__nimble_md_out_" + user + "_" + pid + ".txt";
                                    FILE* old = freopen(outFile.c_str(), "w", stdout);
                                    ((MarkdownPlugin*)p.get())->printWithHighlight(tmp);
                                    fflush(stdout);
                                    if (old) freopen("CON", "w", stdout);
                                    std::ifstream fin(outFile);
                                    if (!fin.is_open()) { mvprintw(0,0,"[Fout] Kan output niet lezen"); remove(tmp.c_str()); return; }
                                    std::string l; int y=0;
                                    while (std::getline(fin, l) && y < maxY-1) {
                                        mvprintw(y++, 0, "%s", l.c_str());
                                    }
                                    remove(tmp.c_str());
                                    remove(outFile.c_str());
                                    break;
                                }
                            }
                            if (!found) mvprintw(0,0,"[Fout] Geen markdown-plugin geladen");
                        } else if (isCsv) {
                            bool found = false;
                            for (auto& p : pluginManager.plugins) {
                                if (p && p->name().find("CSV") != std::string::npos) {
                                    found = true;
                                    std::string tmp = "__nimble_tmp_csv_" + user + "_" + pid + ".csv";
                                    std::ofstream f(tmp);
                                    if (!f.is_open()) { mvprintw(0,0,"[Fout] Kan temp-bestand niet schrijven"); return; }
                                    for (const auto& l : lines) f << l << "\n";
                                    f.close();
                                    fflush(stdout);
                                    std::string outFile = "__nimble_csv_out_" + user + "_" + pid + ".txt";
                                    FILE* old = freopen(outFile.c_str(), "w", stdout);
                                    struct CsvPlugin { void printTable(const std::string&, int, bool, const std::string&, bool); };
                                    ((CsvPlugin*)p.get())->printTable(tmp, csvSortCol, csvSortAsc, csvFilter, csvShowStats);
                                    fflush(stdout);
                                    if (old) freopen("CON", "w", stdout);
                                    std::ifstream fin(outFile);
                                    if (!fin.is_open()) { mvprintw(0,0,"[Fout] Kan output niet lezen"); remove(tmp.c_str()); return; }
                                    std::string l; int y=0;
                                    while (std::getline(fin, l) && y < maxY-1) {
                                        mvprintw(y++, 0, "%s", l.c_str());
                                    }
                                    remove(tmp.c_str());
                                    remove(outFile.c_str());
                                    break;
                                }
                            }
                            if (!found) mvprintw(0,0,"[Fout] Geen CSV-plugin geladen");
                        } else {
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
