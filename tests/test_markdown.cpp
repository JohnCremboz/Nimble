#include "../plugins/markdown_plugin.cpp"
#include <cassert>
#include <iostream>

int main() {
    MarkdownPlugin md;
    std::ofstream f("test.md");
    f << "# Titel\n**vet** _cursief_\n- lijst\n"; f.close();
    // Test: geen crash bij highlight
    md.printWithHighlight("test.md");
    remove("test.md");
    std::cout << "Alle Markdown-tests geslaagd!\n";
    return 0;
}
