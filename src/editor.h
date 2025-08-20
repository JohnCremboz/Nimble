
#pragma once
#include <string>
#include <vector>

class Editor {
public:
    Editor(const std::string& lang = "en");
    void run();
    bool openFile(const std::string& path);
    bool saveFile(const std::string& path = "");
    void search(const std::string& term);
    void replace(const std::string& term, const std::string& replacement, bool all = false);
    void undo();
    void redo();
private:
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
    void processInput(int ch);
    void searchPrompt();
    void replacePrompt();
    void helpPrompt();
    std::string lang;
};
