## Plugin-ondersteuning

Nimble ondersteunt dynamische plugins (.so/.dll) via de map `plugins`.

- Interface: zie `src/plugin.h` (abstracte klasse Plugin)
- Plugins worden automatisch geladen bij opstarten
- Voorbeeldplugin: `plugins/example_plugin.cpp`
- Bouw plugins als shared library via CMake (zie CMakeLists.txt)

**Plugin interface:**
```cpp
class Plugin {
public:
   virtual ~Plugin() = default;
   virtual std::string name() const = 0;
   virtual void onLoad() = 0;
   virtual void onUnload() = 0;
   // Breid uit met hooks zoals onSave, onOpen, onKeyPress, ...
};
```

**Plugin entrypoint:**
```cpp
extern "C" Plugin* createPlugin();
```

Plaats de .so/.dll in de map `plugins` en herstart de editor.

# Nimble Editor v0.01

Een lichtgewicht, meertalige, terminal-gebaseerde teksteditor in C++ met bestandsbrowser, thema's, voorkeuren en aanpasbare sneltoetsen.
Auteur: JohnCremboz


## Features
- Openen, bewerken en opslaan van tekstbestanden
- Navigatie met pijltjestoetsen, PageUp/PageDown
- Undo/redo, zoeken/vervangen
- Statusbalk met bestandsinfo en cursorpositie
- Lijnnummering
- Syntax highlighting (C++-achtig)
- Bestandsbrowser (split-screen)
- Meertalig: Nederlands, Engels, Frans, Duits, Spaans
- Thema's (10 kleurenschema's)
- Voorkeurenscherm voor taal en thema (wordt opgeslagen in preferences.json)
- Aanpasbare sneltoetsen via keybindings.json
- About/help-scherm (Ctrl+H) met versie en auteur


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
