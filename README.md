# Nimble Editor v1.0

Een lichtgewicht, meertalige, terminal-gebaseerde teksteditor in C++ met bestandsbrowser, thema's, voorkeuren, aanpasbare sneltoetsen en een krachtig plugin-systeem.
## Features
- Openen, bewerken en opslaan van tekstbestanden
- Navigatie met pijltjestoetsen, PageUp/PageDown
- Undo/redo, zoeken/vervangen
- Statusbalk met bestandsinfo en cursorpositie
- Lijnnummering en syntax highlighting
- Bestandsbrowser (split-screen)
- Thema's (10 kleurenschema's)
- Voorkeurenscherm voor taal/thema (preferences.json)
- Aanpasbare sneltoetsen (keybindings.json)
- About/help-scherm (Ctrl+H)
- Plugin-ondersteuning (.so/.dll, events/callbacks, platformonafhankelijk)
- CSV- en Markdown-plugins (interactief menu, tabelweergave, statistiek, highlighting)
- Unit tests voor pluginlogica (tests/)
- Robuuste foutafhandeling (try/catch, bestandschecks, pluginvalidatie)

## Plugin-ondersteuning
Nimble ondersteunt dynamische plugins (.so/.dll) via de map `plugins`.

- Interface: zie `src/plugin.h` (abstracte klasse Plugin, event/callback API)
- Plugins worden automatisch geladen bij opstarten (platformonafhankelijk)
- Events: onFileOpen, onFileSave, onKeyPress, onEditorDraw
- Voorbeeldplugin: `plugins/example_plugin.cpp`
- CSV-plugin: `plugins/csv_plugin.cpp` (CSV-menu, sorteren/filteren/statistiek)
- Markdown-plugin: `plugins/markdown_plugin.cpp` (highlighting)
- Bouw plugins als shared library via CMake (zie CMakeLists.txt)

**Plugin interface:**
```cpp
class Plugin {
public:
   virtual void onFileOpen(const std::string& path) {}
   virtual void onFileSave(const std::string& path) {}

   # Nimble Editor v1.0

   ---

   ## NL — Overzicht

   Een lichtgewicht, meertalige, terminal-gebaseerde teksteditor in C++ met bestandsbrowser, thema's, voorkeuren, aanpasbare sneltoetsen en een krachtig plugin-systeem.
   Auteur: JohnCremboz

   ### Features

   - Openen, bewerken en opslaan van tekstbestanden
   - Navigatie met pijltjestoetsen, PageUp/PageDown
   - Undo/redo, zoeken/vervangen
   - Statusbalk met bestandsinfo en cursorpositie
   - Lijnnummering en syntax highlighting
   - Bestandsbrowser (split-screen)
   - Meertalig: Nederlands, Engels, Frans, Duits, Spaans
   - Thema's (10 kleurenschema's)
   - Voorkeurenscherm voor taal/thema (preferences.json)
   - Aanpasbare sneltoetsen (keybindings.json)
   - About/help-scherm (Ctrl+H)
   - Plugin-ondersteuning (.so/.dll, events/callbacks, platformonafhankelijk)
   - CSV- en Markdown-plugins (interactief menu, tabelweergave, statistiek, highlighting)
   - Unit tests voor pluginlogica (tests/)
   - Robuuste foutafhandeling (try/catch, bestandschecks, pluginvalidatie)

   ### Plugin-ondersteuning

   Nimble ondersteunt dynamische plugins (.so/.dll) via de map `plugins`.

   - Interface: zie `src/plugin.h` (abstracte klasse Plugin, event/callback API)
   - Plugins worden automatisch geladen bij opstarten (platformonafhankelijk)
   - Events: onFileOpen, onFileSave, onKeyPress, onEditorDraw
   - Voorbeeldplugin: `plugins/example_plugin.cpp`
   - CSV-plugin: `plugins/csv_plugin.cpp` (CSV-menu, sorteren/filteren/statistiek)
   - Markdown-plugin: `plugins/markdown_plugin.cpp` (highlighting)
   - Bouw plugins als shared library via CMake (zie CMakeLists.txt)

   **Plugin interface:**

   ```cpp
   class Plugin {
   public:
      virtual ~Plugin() = default;
      virtual std::string name() const = 0;
      virtual void onLoad() = 0;
      virtual void onUnload() = 0;
      virtual void onFileOpen(const std::string& path) {}
      virtual void onFileSave(const std::string& path) {}
      virtual void onKeyPress(int key) {}
      virtual void onEditorDraw() {}
   };
   ```

   **Plugin entrypoint:**

   ```cpp
   extern "C" Plugin* createPlugin();
   ```

   Plaats de .so/.dll in de map `plugins` en herstart de editor.

   ### Sneltoetsen (standaard, aanpasbaar via keybindings.json)

   - Ctrl+S: Opslaan
   - Ctrl+X: Afsluiten
   - Ctrl+F: Zoeken
   - Ctrl+R: Vervangen
   - Ctrl+Z: Undo
   - Ctrl+Y: Redo
   - Ctrl+H: Help/About
   - Tab: Wissel focus tussen editor en browser
   - Esc: Sluit de app
   - Ctrl+M: CSV-menu (indien plugin actief)

   ### Installatie & Build

   1. Vereisten: C++17, ncurses, CMake
   2. Build:

      ```sh
      mkdir build
      cd build
      cmake ..
      cmake --build .
      ```

   3. Start de app:

      ```sh
      ./nimble
      ```

   ### Voorkeuren & Configuratie

   - Bij het opstarten worden taal en thema automatisch geladen uit `preferences.json`.
   - Pas taal/thema aan via het voorkeurenscherm (wordt direct opgeslagen).
   - Sneltoetsen zijn aanpasbaar via `keybindings.json`.

   ### About/Help

   Druk op **Ctrl+H** voor een help- en aboutscherm met sneltoetsen, versie en auteur.


   ### Licentie
   GPL-3.0

   ---

   ## EN — Overview

   Nimble is a lightweight, terminal-based text editor in C++ with file browser, multilingual support, themes, preferences, customizable keybindings, and a powerful plugin system.
   Author: JohnCremboz

   ### Features

   - Open, edit and save text files
   - Navigation with arrow keys, PageUp/PageDown
   - Undo/redo, search/replace
   - Status bar with file info and cursor position
   - Line numbering and syntax highlighting
   - File browser (split-screen)
   - Multilingual: Dutch, English, French, German, Spanish
   - Themes (10 color schemes)
   - Preferences screen for language/theme (preferences.json)
   - Customizable keybindings (keybindings.json)
   - About/help screen (Ctrl+H)
   - Plugin support (.so/.dll, events/callbacks, cross-platform)
   - CSV and Markdown plugins (interactive menu, table view, stats, highlighting)
   - Unit tests for plugin logic (tests/)
   - Robust error handling (try/catch, file checks, plugin validation)

   ### Plugin support

   Nimble supports dynamic plugins (.so/.dll) via the `plugins` folder.

   - Interface: see `src/plugin.h` (abstract Plugin class, event/callback API)
   - Plugins are loaded automatically at startup (cross-platform)
   - Events: onFileOpen, onFileSave, onKeyPress, onEditorDraw
   - Example plugin: `plugins/example_plugin.cpp`
   - CSV plugin: `plugins/csv_plugin.cpp` (CSV menu, sort/filter/stats)
   - Markdown plugin: `plugins/markdown_plugin.cpp` (highlighting)
   - Build plugins as shared library via CMake (see CMakeLists.txt)

   **Plugin interface:**

   ```cpp
   class Plugin {
   public:
      virtual ~Plugin() = default;
      virtual std::string name() const = 0;
      virtual void onLoad() = 0;
      virtual void onUnload() = 0;
      virtual void onFileOpen(const std::string& path) {}
      virtual void onFileSave(const std::string& path) {}
      virtual void onKeyPress(int key) {}
      virtual void onEditorDraw() {}
   };
   ```

   **Plugin entrypoint:**

   ```cpp
   extern "C" Plugin* createPlugin();
   ```

   Place the .so/.dll in the `plugins` folder and restart the editor.

   ### Shortcuts (default, customizable via keybindings.json)

   - Ctrl+S: Save
   - Ctrl+X: Exit
   - Ctrl+F: Find
   - Ctrl+R: Replace
   - Ctrl+Z: Undo
   - Ctrl+Y: Redo
   - Ctrl+H: Help/About
   - Tab: Switch focus editor/browser
   - Esc: Exit app
   - Ctrl+M: CSV menu (if plugin active)

   ### Installation & Build

   1. Requirements: C++17, ncurses, CMake
   2. Build:

      ```sh
      mkdir build
      cd build
      cmake ..
      cmake --build .
      ```

   3. Start the app:

      ```sh
      ./nimble
      ```

   ### Preferences & Configuration

   - On startup, language and theme are loaded from `preferences.json`.
   - Change language/theme via the preferences screen (saved immediately).
   - Keybindings are customizable via `keybindings.json`.

   ### About/Help

   Press **Ctrl+H** for a help/about screen with shortcuts, version and author.

   ### License
   GPL-3.0
   MIT


## Sneltoetsen (standaard, aanpasbaar via keybindings.json)
- **Ctrl+S**: Opslaan
- **Ctrl+X**: Afsluiten
- **Ctrl+F**: Zoeken
- **Ctrl+R**: Vervangen
- **Ctrl+Z**: Undo
- **Ctrl+Y**: Redo
- **Ctrl+H**: Help/About
- **Tab**: Wissel focus tussen editor en browser
- **Esc**: Sluit de app


## Installatie & Build
1. Vereisten: C++17, ncurses, CMake
2. Build:
   ```sh
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```
3. Start de app:
   ```sh
   ./nimble
   ```


## Voorkeuren & Configuratie
- Bij het opstarten worden taal en thema automatisch geladen uit `preferences.json`.
- Pas taal/thema aan via het voorkeurenscherm (wordt direct opgeslagen).
- Sneltoetsen zijn aanpasbaar via `keybindings.json`.
## About/Help
Druk op **Ctrl+H** voor een help- en aboutscherm met sneltoetsen, versie en auteur.


## Licentie
MIT
