#pragma once
#include <map>
#include <string>
#include <vector>

struct Theme {
    std::string name;
    short fg;
    short bg;
    short keyword;
    short stringc;
};

extern std::map<std::string, int> keymap;
void loadKeybindings(const std::string& path = "keybindings.json");

class Editor {
public:
    Editor(const std::string& lang = "en", int themeIdx = 0);
    void run();
    bool openFile(const std::string& path);
    bool saveFile(const std::string& path = "");
    void search(const std::string& term);
    void replace(const std::string& term, const std::string& replacement, bool all = false);
    void undo();
    void redo();
    std::string filename;
    std::vector<std::string> lines;
    int cursorX = 0;
    int cursorY = 0;
    bool modified = false;
    std::string lastSearchTerm;
    std::vector<std::vector<std::string>> undoStack;
    std::vector<std::vector<std::string>> redoStack;
    void draw();
    void drawStatusBar();
    void drawTitleBar();
    void processInput(int ch);
    void searchPrompt();
    void replacePrompt();
    void helpPrompt();
    std::string lang;
    int themeIdx = 0;
    void csvMenuPrompt();
    
    // CSV menu state variables
    int csvSortCol = -1;
    bool csvSortAsc = true;
    std::string csvFilter;
    bool csvShowStats = false;
};
