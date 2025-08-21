
# Nimble Editor v1.0 — Help / Hulp

---

## NL — Overzicht
Nimble is een lichtgewicht, terminal-gebaseerde teksteditor in C++ met bestandsbrowser, meertalige ondersteuning, thema's, voorkeuren, aanpasbare sneltoetsen en een krachtig plugin-systeem.

### Belangrijkste features

- Openen, bewerken en opslaan van tekstbestanden
- Undo/redo, zoeken/vervangen
- Statusbalk met bestandsinfo en cursorpositie
- Lijnnummering en syntax highlighting
- Bestandsbrowser (split-screen)
- Meertalig: Nederlands, Engels, Frans, Duits, Spaans
- Thema's (10 kleurenschema's)
- Voorkeurenscherm voor taal/thema (preferences.json)
- Aanpasbare sneltoetsen (keybindings.json)
- Plugin-ondersteuning (.so/.dll, events/callbacks)
- About/help-scherm (Ctrl+H)

### Sneltoetsen (standaard)

- Ctrl+S: Opslaan
- Ctrl+X: Afsluiten
- Ctrl+F: Zoeken
- Ctrl+R: Vervangen
- Ctrl+Z: Undo
- Ctrl+Y: Redo
- Ctrl+H: Help/About
- Tab: Wissel focus editor/browser
- Esc: Sluit de app
- Ctrl+M: CSV-menu (indien plugin actief)

### Plugins

- Plugins worden automatisch geladen uit de map `plugins`.
- Events: onFileOpen, onFileSave, onKeyPress, onEditorDraw
- Voorbeeld: csv_plugin (CSV-menu, sorteren/filteren/statistiek)

### Configuratie

- Taal/thema: voorkeurenscherm of direct in preferences.json
- Sneltoetsen: keybindings.json

### Problemen?

- Controleer of alle afhankelijkheden (C++17, ncurses, CMake) aanwezig zijn.
- Bekijk de README.md voor build-instructies.


### Licentie
GPL-3.0

Auteur: JohnCremboz
### License
GPL-3.0

Author: JohnCremboz

---

## EN — Overview
Nimble is a lightweight, terminal-based text editor in C++ with file browser, multilingual support, themes, preferences, customizable keybindings, and a powerful plugin system.

### Key features

- Open, edit and save text files
- Undo/redo, search/replace
- Status bar with file info and cursor position
- Line numbering and syntax highlighting
- File browser (split-screen)
- Multilingual: Dutch, English, French, German, Spanish
- Themes (10 color schemes)
- Preferences screen for language/theme (preferences.json)
- Customizable keybindings (keybindings.json)
- Plugin support (.so/.dll, events/callbacks)
- About/help screen (Ctrl+H)

### Shortcuts (default)

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

### Plugins

- Plugins are automatically loaded from the `plugins` folder.
- Events: onFileOpen, onFileSave, onKeyPress, onEditorDraw
- Example: csv_plugin (CSV menu, sort/filter/stats)

### Configuration

- Language/theme: preferences screen or directly in preferences.json
- Keybindings: keybindings.json

### Problems?

- Check if all dependencies (C++17, ncurses, CMake) are present.
- See README.md for build instructions.

### License
MIT

Author: JohnCremboz
