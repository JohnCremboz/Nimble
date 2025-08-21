
# 📝 GitHub Copilot Requirements Sheet

**Project Title:** C++ Clone of GNU Nano with File Browser  
**Objective:** Gebruik GitHub Copilot om een lichtgewicht, terminal-gebaseerde teksteditor in C++ te bouwen die de kernfunctionaliteit van GNU Nano nabootst, met extra ondersteuning voor het bladeren door mappen en bestanden zoals in Vim.

---


## 1. Functionele Vereisten

- [x] Voorkeurenscherm voor taal- en themakeuze bij opstarten.

- [x] Openen, bewerken en opslaan (Ctrl+S) van tekstbestanden.
- [x] Navigatie met pijltjestoetsen, pagina omhoog/omlaag.
- [x] Tekst invoegen, verwijderen en overschrijven (Insert).
- [x] Zoeken (Ctrl+F) en vervangen (Ctrl+R).
- [x] Undo (Ctrl+Z) / Redo (Ctrl+Y).
- [x] Statusbalk met bestandsinfo en cursorpositie.
- [x] Sneltoetsen (Ctrl+S, Ctrl+X, Ctrl+F, Ctrl+R, Ctrl+Z, Ctrl+Y, Ctrl+H).

### Bestandsbeheer
- [x] Openen van bestanden via command-line argumenten.
- [x] Waarschuwing bij niet-opgeslagen wijzigingen.
- [x] Ondersteuning voor UTF-8.

### Bestandsbrowser (Nieuw)
- [x] Mogelijkheid om een map te openen en bestanden te bekijken.
- [x] Navigatie tussen mappen en bestanden via toetsenbord.
- [x] Selecteren van een bestand om te openen in de editor.
- [x] Weergave van mappenstructuur in een paneel of popup.
- [ ] Filteren op bestandstype of naam.

---

## 2. Niet-functionele Vereisten

- **Prestaties:** Snelle opstart, lage geheugengebruik.
- **Compatibiliteit:** Linux (GCC/Clang); optioneel Windows via MinGW/WSL.
- **Codekwaliteit:** Modern C++ (C++17 of later), modulair en leesbaar.
- **Afhankelijkheden:** Bij voorkeur standaardbibliotheken; gebruik van `ncurses` voor UI.

---

## 3. Technische Beperkingen

- **Taal:** C++
- **UI:** Terminal-gebaseerd met `ncurses` of alternatief
- **Buildsysteem:** CMake of Makefile
- **Versiebeheer:** Git (GitHub)

---

## 4. Copilot Gebruiksrichtlijnen

✅ Gebruik Copilot voor:
- Genereren van boilerplate code.
- Suggesties voor bestandsbeheer en UI.
- Keyboard input en navigatie.
- Structuur van bestandsbrowser.

❌ Vermijd:
- Directe kopieën van GNU Nano code.
- Code genereren zonder begrip.
- Licentieconflicten (GNU Nano is GPL).

---

## 5. Stretch Goals (Optioneel)

- [x] Syntax highlighting.
- [x] Lijnnummering.
- [ ] Aanpasbare keybindings.
- [ ] Plugin-ondersteuning.
- [x] Split-screen voor file browser + editor.
- [x] Thema's (kleurenschema's) en voorkeurenscherm.
