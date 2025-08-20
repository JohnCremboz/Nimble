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
        {"help", "Ctrl+G Help"},
        {"search", "/Search: "},
        {"replace", "/Replace: "},
        {"replacewith", "/Replace with: "},
        {"helptxt", "Shortcuts: Ctrl+O Save | Ctrl+X Exit | Ctrl+W Search | Ctrl+R Replace | Ctrl+Z Undo | Ctrl+Y Redo | PgUp/PgDn Scroll | Ctrl+G Close"},
        {"unsaved", "Unsaved changes! Press y to save, n to ignore."}
    }},
    {"fr", {
        {"filebrowser", "Explorateur de fichiers:"},
        {"open", "Ouvrir"},
        {"directory", "Dossier"},
        {"tabtip", "Tab: changer | Esc: quitter"},
        {"file", "Fichier"},
        {"pos", "Pos"},
        {"help", "Ctrl+G Aide"},
        {"search", "/Rechercher: "},
        {"replace", "/Remplacer: "},
        {"replacewith", "/Remplacer par: "},
        {"helptxt", "Raccourcis: Ctrl+O Sauver | Ctrl+X Quitter | Ctrl+W Rechercher | Ctrl+R Remplacer | Ctrl+Z Annuler | Ctrl+Y Rétablir | PgUp/PgDn Défilement | Ctrl+G Fermer"},
        {"unsaved", "Modifications non enregistrées ! Appuyez sur y pour sauvegarder, n pour ignorer."}
    }},
    {"nl", {
        {"filebrowser", "Bestandsbrowser:"},
        {"open", "Openen"},
        {"directory", "Map"},
        {"tabtip", "Tab: wissel | Esc: sluit af"},
        {"file", "Bestand"},
        {"pos", "Pos"},
        {"help", "Ctrl+G Help"},
        {"search", "/Zoeken: "},
        {"replace", "/Vervang: "},
        {"replacewith", "/Vervang door: "},
        {"helptxt", "Sneltoetsen: Ctrl+O Opslaan | Ctrl+X Afsluiten | Ctrl+W Zoeken | Ctrl+R Vervangen | Ctrl+Z Undo | Ctrl+Y Redo | PgUp/PgDn Scroll | Ctrl+G Sluiten"},
        {"unsaved", "Niet-opgeslagen wijzigingen! Druk op y om op te slaan, n om te negeren."}
    }},
    {"de", {
        {"filebrowser", "Dateibrowser:"},
        {"open", "Öffnen"},
        {"directory", "Verzeichnis"},
        {"tabtip", "Tab: wechseln | Esc: beenden"},
        {"file", "Datei"},
        {"pos", "Pos"},
        {"help", "Ctrl+G Hilfe"},
        {"search", "/Suchen: "},
        {"replace", "/Ersetzen: "},
        {"replacewith", "/Ersetzen durch: "},
        {"helptxt", "Shortcuts: Ctrl+O Speichern | Ctrl+X Beenden | Ctrl+W Suchen | Ctrl+R Ersetzen | Ctrl+Z Rückgängig | Ctrl+Y Wiederholen | PgUp/PgDn Scrollen | Ctrl+G Schließen"},
        {"unsaved", "Nicht gespeicherte Änderungen! Drücken Sie y zum Speichern, n zum Ignorieren."}
    }},
    {"es", {
        {"filebrowser", "Explorador de archivos:"},
        {"open", "Abrir"},
        {"directory", "Directorio"},
        {"tabtip", "Tab: cambiar | Esc: salir"},
        {"file", "Archivo"},
        {"pos", "Pos"},
        {"help", "Ctrl+G Ayuda"},
        {"search", "/Buscar: "},
        {"replace", "/Reemplazar: "},
        {"replacewith", "/Reemplazar por: "},
        {"helptxt", "Atajos: Ctrl+O Guardar | Ctrl+X Salir | Ctrl+W Buscar | Ctrl+R Reemplazar | Ctrl+Z Deshacer | Ctrl+Y Rehacer | PgUp/PgDn Desplazar | Ctrl+G Cerrar"},
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
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    std::string lang = chooseLanguage();

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    int splitCol = maxX / 3; // 1/3 voor browser, 2/3 voor editor
    WINDOW* browserWin = newwin(maxY, splitCol, 0, 0);
    WINDOW* editorWin = newwin(maxY, maxX - splitCol, 0, splitCol);

    FileBrowser browser;
    Editor editor(lang);
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
    endwin();
    return 0;
}
