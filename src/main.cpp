#include "pluginmanager.h"
#include <fstream>
#include "nlohmann/json.hpp"
using json = nlohmann::json;
#include <vector>
#include <utility>

struct Theme {
    std::string name;
    short fg;
    short bg;
    short keyword;
    short stringc;
};

std::vector<Theme> themes = {
    {"Default", COLOR_WHITE, -1, COLOR_CYAN, COLOR_YELLOW},
    {"Solarized Light", COLOR_BLACK, COLOR_YELLOW, COLOR_BLUE, COLOR_RED},
    {"Solarized Dark", COLOR_WHITE, COLOR_BLUE, COLOR_CYAN, COLOR_YELLOW},
    {"Monokai", COLOR_WHITE, COLOR_MAGENTA, COLOR_GREEN, COLOR_YELLOW},
    {"Dracula", COLOR_WHITE, COLOR_MAGENTA, COLOR_CYAN, COLOR_YELLOW},
    {"Gruvbox", COLOR_WHITE, COLOR_RED, COLOR_YELLOW, COLOR_GREEN},
    {"Nord", COLOR_WHITE, COLOR_BLUE, COLOR_CYAN, COLOR_MAGENTA},
    {"One Dark", COLOR_WHITE, COLOR_BLUE, COLOR_CYAN, COLOR_YELLOW},
    {"Tomorrow Night", COLOR_WHITE, COLOR_BLACK, COLOR_CYAN, COLOR_YELLOW},
    {"PaperColor", COLOR_BLACK, COLOR_WHITE, COLOR_BLUE, COLOR_RED},
    {"Cobalt2", COLOR_WHITE, COLOR_BLUE, COLOR_YELLOW, COLOR_CYAN}
};

int showPreferences(std::string& lang, int& themeIdx) {
    clear();
    int choice = 0;
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    int selLang = 0;
    int selTheme = themeIdx;
    std::vector<std::string> langs = {"nl", "en", "fr", "de", "es"};
    std::vector<std::string> langLabels = {"Nederlands", "English", "Français", "Deutsch", "Español"};
    bool inLang = true;
    while (true) {
        clear();
        mvprintw(0, 0, "Voorkeuren / Preferences");
        mvprintw(2, 2, inLang ? "> Taal / Language:" : "  Taal / Language:");
        for (size_t i = 0; i < langLabels.size(); ++i) {
            if (inLang && (int)i == selLang) attron(A_REVERSE);
            mvprintw(3 + i, 6, "%s", langLabels[i].c_str());
            if (inLang && (int)i == selLang) attroff(A_REVERSE);
        }
        mvprintw(10, 2, !inLang ? "> Thema / Theme:" : "  Thema / Theme:");
        for (size_t i = 0; i < themes.size(); ++i) {
            if (!inLang && (int)i == selTheme) attron(A_REVERSE);
            mvprintw(11 + i, 6, "%s", themes[i].name.c_str());
            if (!inLang && (int)i == selTheme) attroff(A_REVERSE);
        }
        mvprintw(maxY-2, 2, "Enter: selecteer / select | Tab: wissel | Esc: annuleer");
        refresh();
        int c = getch();
        if (c == 9) { // Tab
            inLang = !inLang;
        } else if (c == KEY_UP) {
            if (inLang && selLang > 0) selLang--;
            else if (!inLang && selTheme > 0) selTheme--;
        } else if (c == KEY_DOWN) {
            if (inLang && selLang + 1 < (int)langs.size()) selLang++;
            else if (!inLang && selTheme + 1 < (int)themes.size()) selTheme++;
        } else if (c == 10) { // Enter
            if (inLang) {
                lang = langs[selLang];
                // Sla direct op
                json j;
                j["language"] = lang;
                j["theme"] = themeIdx;
                std::ofstream pf("preferences.json");
                if (pf.is_open()) pf << j.dump(4);
            } else {
                themeIdx = selTheme;
                // Sla direct op
                json j;
                j["language"] = lang;
                j["theme"] = themeIdx;
                std::ofstream pf("preferences.json");
                if (pf.is_open()) pf << j.dump(4);
                return 1;
            }
        } else if (c == 27) {
            return 0;
        }
    }
}
#include "editor.h"
#include "filebrowser.h"
#include <ncurses.h>
#include <map>
#include <string>

std::map<std::string, std::map<std::string, std::string>> translations = {
    {"en", {
        {"filebrowser", "File Browser:"},
        {"open", "Open"},
        {"directory", "Directory"},
        {"tabtip", "Tab: switch | Esc: quit"},
    {"file", "File"},
    {"pos", "Pos"},
    {"help", "Ctrl+H Help"},
    {"search", "/Search: "},
    {"replace", "/Replace: "},
    {"replacewith", "/Replace with: "},
    {"helptxt", "Shortcuts: Ctrl+S Save | Ctrl+X Exit | Ctrl+F Search | Ctrl+R Replace | Ctrl+Z Undo | Ctrl+Y Redo | Ctrl+H Help | PgUp/PgDn Scroll"},
    {"unsaved", "Unsaved changes! Press y to save, n to ignore."}
    }},
    {"fr", {
        {"filebrowser", "Explorateur de fichiers:"},
        {"open", "Ouvrir"},
        {"directory", "Dossier"},
        {"tabtip", "Tab: changer | Esc: quitter"},
    {"file", "Fichier"},
    {"pos", "Pos"},
    {"help", "Ctrl+H Aide"},
    {"search", "/Rechercher: "},
    {"replace", "/Remplacer: "},
    {"replacewith", "/Remplacer par: "},
    {"helptxt", "Raccourcis: Ctrl+S Sauver | Ctrl+X Quitter | Ctrl+F Rechercher | Ctrl+R Remplacer | Ctrl+Z Annuler | Ctrl+Y Rétablir | Ctrl+H Aide | PgUp/PgDn Défilement"},
    {"unsaved", "Modifications non enregistrées ! Appuyez sur y pour sauvegarder, n pour ignorer."}
    }},
    {"nl", {
        {"filebrowser", "Bestandsbrowser:"},
        {"open", "Openen"},
        {"directory", "Map"},
        {"tabtip", "Tab: wissel | Esc: sluit af"},
    {"file", "Bestand"},
    {"pos", "Pos"},
    {"help", "Ctrl+H Help"},
    {"search", "/Zoeken: "},
    {"replace", "/Vervang: "},
    {"replacewith", "/Vervang door: "},
    {"helptxt", "Sneltoetsen: Ctrl+S Opslaan | Ctrl+X Afsluiten | Ctrl+F Zoeken | Ctrl+R Vervangen | Ctrl+Z Undo | Ctrl+Y Redo | Ctrl+H Help | PgUp/PgDn Scroll"},
    {"unsaved", "Niet-opgeslagen wijzigingen! Druk op y om op te slaan, n om te negeren."}
    }},
    {"de", {
        {"filebrowser", "Dateibrowser:"},
        {"open", "Öffnen"},
        {"directory", "Verzeichnis"},
        {"tabtip", "Tab: wechseln | Esc: beenden"},
    {"file", "Datei"},
    {"pos", "Pos"},
    {"help", "Ctrl+H Hilfe"},
    {"search", "/Suchen: "},
    {"replace", "/Ersetzen: "},
    {"replacewith", "/Ersetzen durch: "},
    {"helptxt", "Shortcuts: Ctrl+S Speichern | Ctrl+X Beenden | Ctrl+F Suchen | Ctrl+R Ersetzen | Ctrl+Z Rückgängig | Ctrl+Y Wiederholen | Ctrl+H Hilfe | PgUp/PgDn Scrollen"},
    {"unsaved", "Nicht gespeicherte Änderungen! Drücken Sie y zum Speichern, n zum Ignorieren."}
    }},
    {"es", {
        {"filebrowser", "Explorador de archivos:"},
        {"open", "Abrir"},
        {"directory", "Directorio"},
        {"tabtip", "Tab: cambiar | Esc: salir"},
    {"file", "Archivo"},
    {"pos", "Pos"},
    {"help", "Ctrl+H Ayuda"},
    {"search", "/Buscar: "},
    {"replace", "/Reemplazar: "},
    {"replacewith", "/Reemplazar por: "},
    {"helptxt", "Atajos: Ctrl+S Guardar | Ctrl+X Salir | Ctrl+F Buscar | Ctrl+R Reemplazar | Ctrl+Z Deshacer | Ctrl+Y Rehacer | Ctrl+H Ayuda | PgUp/PgDn Desplazar"},
    {"unsaved", "¡Cambios no guardados! Pulsa y para guardar, n para ignorar."}
    }}
};

std::string chooseLanguage() {
    clear();
    mvprintw(0, 0, "Kies taal / Choose language / Choisissez la langue / Sprache wählen / Elige idioma:");
    mvprintw(2, 2, "1. Nederlands");
    mvprintw(3, 2, "2. English");
    mvprintw(4, 2, "3. Français");
    mvprintw(5, 2, "4. Deutsch");
    mvprintw(6, 2, "5. Español");
    refresh();
    int c = getch();
    switch (c) {
        case '1': return "nl";
        case '2': return "en";
        case '3': return "fr";
        case '4': return "de";
        case '5': return "es";
        default: return "en";
    }
}

int main(int argc, char* argv[]) {
    PluginManager pluginManager;
    pluginManager.loadPlugins();
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    std::string lang = "en";
    int themeIdx = 0;
    // Laad voorkeuren uit preferences.json
    std::ifstream pf("preferences.json");
    if (pf.is_open()) {
        json j;
        pf >> j;
        if (j.contains("language")) lang = j["language"].get<std::string>();
        if (j.contains("theme")) themeIdx = j["theme"].get<int>();
    }

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    int splitCol = maxX / 3; // 1/3 voor browser, 2/3 voor editor
    WINDOW* browserWin = newwin(maxY, splitCol, 0, 0);
    WINDOW* editorWin = newwin(maxY, maxX - splitCol, 0, splitCol);

    FileBrowser browser;
    Editor editor(lang, themeIdx);
    std::string fileToOpen;
    bool running = true;
    bool focusEditor = true;
    while (running) {
        // Teken filebrowser links
        wclear(browserWin);
        wmove(browserWin, 0, 0);
        int bY = 0;
        mvwprintw(browserWin, bY++, 0, "%s", translations[lang]["filebrowser"].c_str());
        for (size_t i = 0; i < browser.entries.size() && bY < maxY-1; ++i) {
            if ((int)i == browser.selected) wattron(browserWin, A_REVERSE);
            mvwprintw(browserWin, bY++, 2, "%s", browser.entries[i].c_str());
            if ((int)i == browser.selected) wattroff(browserWin, A_REVERSE);
        }
        mvwprintw(browserWin, maxY-2, 1, "%s", translations[lang]["tabtip"].c_str());
        box(browserWin, 0, 0);
        wrefresh(browserWin);

        // Teken editor rechts
        wclear(editorWin);
        wmove(editorWin, 0, 0);
        WINDOW* old = stdscr;
        stdscr = editorWin;
        editor.draw();
        stdscr = old;
        box(editorWin, 0, 0);
        wrefresh(editorWin);

        int ch = getch();
        if (ch == 9) { // Tab wisselt focus
            focusEditor = !focusEditor;
        } else if (focusEditor) {
            editor.processInput(ch);
        } else {
            // Filebrowser interactie
            if (ch == KEY_UP && browser.selected > 0) browser.selected--;
            else if (ch == KEY_DOWN && browser.selected + 1 < (int)browser.entries.size()) browser.selected++;
            else if (ch == 10 && !browser.entries.empty()) {
                std::string sel = browser.entries[browser.selected];
                std::string fullPath = browser.currentPath + "/" + sel;
                if (std::filesystem::is_directory(fullPath)) {
                    browser.currentPath = fullPath;
                    browser.listDir(browser.currentPath);
                    browser.selected = 0;
                } else {
                    editor.openFile(fullPath);
                    focusEditor = true;
                }
            }
        }
        if (ch == 27) running = false; // Escape sluit af
    }

    delwin(browserWin);
    delwin(editorWin);
    pluginManager.unloadPlugins();
    endwin();
    return 0;
}
