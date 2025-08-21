#include "../plugins/csv_plugin.cpp"
#include <cassert>
#include <iostream>

int main() {
    CsvPlugin csv;
    // Test delimiter detectie
    std::ofstream f("test1.csv");
    f << "a,b,c\n1,2,3\n"; f.close();
    assert(csv.detectDelimiter("test1.csv") == ',');
    // Test parsing
    auto data = csv.parseCsv("test1.csv");
    assert(data.size() == 2 && data[0].size() == 3);
    // Test pretty print (geen crash)
    csv.printTable("test1.csv");
    // Test statistieken
    csv.printStats("test1.csv");
    remove("test1.csv");
    std::cout << "Alle CSV-tests geslaagd!\n";
    return 0;
}
