# Nimble Nano Editor

Een lichtgewicht, meertalige, terminal-gebaseerde teksteditor in C++ met bestandsbrowser.

## Features
- Openen, bewerken en opslaan van tekstbestanden
- Navigatie met pijltjestoetsen, PageUp/PageDown
- Undo/redo, zoeken/vervangen
- Statusbalk met bestandsinfo en cursorpositie
- Sneltoetsen (Ctrl+O, Ctrl+X, Ctrl+W, Ctrl+R, Ctrl+Z, Ctrl+Y, Ctrl+G)
- Lijnnummering
- Syntax highlighting (C++-achtig)
- Bestandsbrowser (split-screen)
- Meertalig: Nederlands, Engels, Frans, Duits, Spaans

## Sneltoetsen
- **Ctrl+O**: Opslaan
- **Ctrl+X**: Afsluiten
- **Ctrl+W**: Zoeken
- **Ctrl+R**: Vervangen
- **Ctrl+Z**: Undo
- **Ctrl+Y**: Redo
- **Ctrl+G**: Help
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

## Taalkeuze
Bij het opstarten kies je de gewenste taal voor de interface.

## Licentie
MIT
