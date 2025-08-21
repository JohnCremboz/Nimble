#include "plugin.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <map>

class CsvPlugin : public Plugin {
public:
    std::string name() const override { return "CSV Plugin"; }
    void onLoad() override {
        std::cout << "[CSV Plugin] geladen!\n";
    }
    void onUnload() override {
        std::cout << "[CSV Plugin] ontladen!\n";
    }

    // Detecteer delimiter (komma, puntkomma, tab, pipe)
    char detectDelimiter(const std::string& path) {
        std::ifstream file(path);
        std::string line;
        std::getline(file, line);
        std::map<char, int> counts;
        for (char delim : {',', ';', '\t', '|'}) {
            counts[delim] = std::count(line.begin(), line.end(), delim);
        }
        char best = ',';
        int maxc = 0;
        for (auto& [d, c] : counts) if (c > maxc) { best = d; maxc = c; }
        return best;
    }

    // Parse CSV met flexibel delimiter, support voor quotes
    std::vector<std::vector<std::string>> parseCsv(const std::string& path, char delim = 0) {
        if (delim == 0) delim = detectDelimiter(path);
        std::vector<std::vector<std::string>> data;
        std::ifstream file(path);
        std::string line;
        while (std::getline(file, line)) {
            std::vector<std::string> row;
            std::string cell;
            bool inQuotes = false;
            for (size_t i = 0; i < line.size(); ++i) {
                char c = line[i];
                if (c == '"') inQuotes = !inQuotes;
                else if (c == delim && !inQuotes) {
                    row.push_back(cell);
                    cell.clear();
                } else {
                    cell += c;
                }
            }
            row.push_back(cell);
            data.push_back(row);
        }
        return data;
    }


    // Pretty print CSV als tabel, met optioneel sorteren/filteren/statistieken
    void printTable(const std::string& path, int sortCol = -1, bool asc = true, const std::string& filter = "", bool showStats = false) {
        char delim = detectDelimiter(path);
        auto data = parseCsv(path, delim);
        if (data.empty()) return;
        // Filteren
        if (!filter.empty()) {
            std::vector<std::vector<std::string>> filtered;
            for (const auto& row : data) {
                for (const auto& cell : row) {
                    if (cell.find(filter) != std::string::npos) {
                        filtered.push_back(row);
                        break;
                    }
                }
            }
            data = filtered;
        }
        // Sorteren
        if (sortCol >= 0 && sortCol < (int)data[0].size()) {
            std::sort(data.begin() + 1, data.end(), [sortCol, asc](const auto& a, const auto& b) {
                if (sortCol >= (int)a.size() || sortCol >= (int)b.size()) return false;
                return asc ? a[sortCol] < b[sortCol] : a[sortCol] > b[sortCol];
            });
        }
        // Kolombreedtes
        std::vector<size_t> colWidths(data[0].size(), 0);
        for (const auto& row : data) {
            for (size_t i = 0; i < row.size(); ++i) {
                colWidths[i] = std::max(colWidths[i], row[i].size());
            }
        }
        // Print tabel
        for (const auto& row : data) {
            for (size_t i = 0; i < row.size(); ++i) {
                std::cout << std::left << std::setw(colWidths[i]+2) << row[i];
            }
            std::cout << "\n";
        }
        // Statistieken
        if (showStats) {
            size_t rows = data.size();
            size_t cols = rows ? data[0].size() : 0;
            size_t empty = 0;
            for (const auto& row : data) for (const auto& cell : row) if (cell.empty()) ++empty;
            std::cout << "[CSV] " << rows << " rijen, " << cols << " kolommen, " << empty << " lege cellen\n";
        }
    }

    // Exporteer CSV naar TSV
    void exportToTsv(const std::string& path, const std::string& outPath) {
        char delim = detectDelimiter(path);
        auto data = parseCsv(path, delim);
        std::ofstream out(outPath);
        for (const auto& row : data) {
            for (size_t i = 0; i < row.size(); ++i) {
                out << row[i];
                if (i+1 < row.size()) out << '\t';
            }
            out << '\n';
        }
    }

    // Statistieken: aantal rijen, kolommen, lege cellen
    void printStats(const std::string& path) {
        char delim = detectDelimiter(path);
        auto data = parseCsv(path, delim);
        size_t rows = data.size();
        size_t cols = rows ? data[0].size() : 0;
        size_t empty = 0;
        for (const auto& row : data) for (const auto& cell : row) if (cell.empty()) ++empty;
        std::cout << "[CSV] " << rows << " rijen, " << cols << " kolommen, " << empty << " lege cellen\n";
    }

    // Validatie: check op ongelijke kolommen
    bool validate(const std::string& path) {
        char delim = detectDelimiter(path);
        auto data = parseCsv(path, delim);
        if (data.empty()) return true;
        size_t cols = data[0].size();
        for (size_t i = 1; i < data.size(); ++i) {
            if (data[i].size() != cols) {
                std::cout << "[CSV] Rij " << i+1 << " heeft " << data[i].size() << " kolommen (verwacht: " << cols << ")\n";
                return false;
            }
        }
        std::cout << "[CSV] Alle rijen hebben " << cols << " kolommen.\n";
        return true;
    }
};

extern "C" Plugin* createPlugin() {
    return new CsvPlugin();
}
