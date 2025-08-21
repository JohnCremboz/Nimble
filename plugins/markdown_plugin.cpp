#include "plugin.h"
#include <iostream>
#include <fstream>
#include <string>
#include <regex>

class MarkdownPlugin : public Plugin {
public:
    std::string name() const override { return "Markdown Plugin"; }
    void onLoad() override {
        std::cout << "[Markdown Plugin] geladen!\n";
    }
    void onUnload() override {
        std::cout << "[Markdown Plugin] ontladen!\n";
    }
    // Geavanceerde markdown-highlighting in terminal
    void printWithHighlight(const std::string& path) {
        std::ifstream file(path);
        std::string line;
        while (std::getline(file, line)) {
            // Titels (H1-H6)
            if (std::regex_match(line, std::regex("^###### "))) {
                std::cout << "\033[1;36m" << line << "\033[0m\n";
            } else if (std::regex_match(line, std::regex("^##### "))) {
                std::cout << "\033[1;35m" << line << "\033[0m\n";
            } else if (std::regex_match(line, std::regex("^#### "))) {
                std::cout << "\033[1;34m" << line << "\033[0m\n";
            } else if (std::regex_match(line, std::regex("^### "))) {
                std::cout << "\033[1;33m" << line << "\033[0m\n";
            } else if (std::regex_match(line, std::regex("^## "))) {
                std::cout << "\033[1;32m" << line << "\033[0m\n";
            } else if (std::regex_match(line, std::regex("^# "))) {
                std::cout << "\033[1;31m" << line << "\033[0m\n";
            } 
            // Horizontale lijn
            else if (std::regex_match(line, std::regex("^---+$"))) {
                std::cout << "\033[2m----------------------------------------\033[0m\n";
            }
            // Lijsten
            else if (std::regex_match(line, std::regex("^\s*[-*+] "))) {
                std::cout << "\033[1;37m" << line << "\033[0m\n";
            }
            // Genummerde lijsten
            else if (std::regex_match(line, std::regex("^\s*\d+\. "))) {
                std::cout << "\033[1;37m" << line << "\033[0m\n";
            }
            // Codeblok (```) openen/sluiten
            else if (std::regex_match(line, std::regex("^```"))) {
                std::cout << "\033[1;30m" << line << "\033[0m\n";
            }
            // Blockquote
            else if (std::regex_match(line, std::regex("^> "))) {
                std::cout << "\033[3;32m" << line << "\033[0m\n";
            }
            // Afbeeldingen ![alt](url)
            else if (std::regex_search(line, std::regex("!\[.*\]\(.*\)"))) {
                std::cout << std::regex_replace(line, std::regex("!\\[(.*?)\\]\\((.*?)\\)"), "\033[1;35m[img: $1]\033[0m($2)") << "\n";
            }
            // Links [text](url)
            else if (std::regex_search(line, std::regex("\[.*\]\(.*\)"))) {
                std::cout << std::regex_replace(line, std::regex("\\[(.*?)\\]\\((.*?)\\)"), "\033[4;34m$1\033[0m($2)") << "\n";
            }
            // Inline code `code`
            else if (std::regex_search(line, std::regex("`[^"]*?`"))) {
                std::cout << std::regex_replace(line, std::regex("`([^`]*)`"), "\033[1;30m$1\033[0m") << "\n";
            }
            // Vetgedrukt **text** of __text__
            else if (std::regex_search(line, std::regex("(\*\*|__)(.*?)\1"))) {
                std::cout << std::regex_replace(line, std::regex("(\*\*|__)(.*?)\1"), "\033[1m$2\033[0m") << "\n";
            }
            // Cursief *text* of _text_
            else if (std::regex_search(line, std::regex("(\*|_)(.*?)\1"))) {
                std::cout << std::regex_replace(line, std::regex("(\*|_)(.*?)\1"), "\033[3m$2\033[0m") << "\n";
            }
            else {
                std::cout << line << "\n";
            }
        }
    }
};

extern "C" Plugin* createPlugin() {
    return new MarkdownPlugin();
}
