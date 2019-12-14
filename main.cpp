#include <iostream>
#include <tuple>
#include "CSVParser.h"

int main() {
    std::ifstream file;

    file.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    try {
        file.open("in.csv");
    }
    catch (std::ifstream::failure &e) {
        std::cerr << "EXCEPTION: can't open file\n";
        exit(1);
    }

    CSVParser <int, std::string, double> parser(file, 0, ',');
    for (auto &it : parser) {
        std::cout << it << "\n";
    }

    return 0;
}