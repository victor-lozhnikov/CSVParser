//
// Created by victor on 12.12.19.
//

#ifndef CSVPARSER_CSVPARSER_H
#define CSVPARSER_CSVPARSER_H

#include <fstream>
#include <tuple>
#include <string>
#include <locale>
#include "ParserIterator.h"

class my_ctype : public std::ctype<char> {
    mask my_table[table_size];
public:
    my_ctype(char delim = ';', size_t refs = 0)
            : std::ctype <char> (&my_table[0], false, refs) {
        std::copy_n(classic_table(), table_size, my_table);
        my_table[' '] = (mask)lower;
        my_table[delim] = (mask)space;
    }
};

template <typename ...Args>
class CSVParser {
public:
    CSVParser(std::ifstream &_file, int skip, char delim = ',') {
        file = &_file;
        std::locale x(std::locale::classic(), new my_ctype(delim));
        file->imbue(x);
        std::string s;
        for (int i = 0; i < skip; ++i) {
            getline(*file, s);
        }
    }

    ParserIterator <Args...> begin() {
        return ParserIterator <Args...> (file);
    }

    ParserIterator <Args...> end() {
        return ParserIterator <Args...> (file, true);
    }

private:
    std::ifstream *file;
};


#endif //CSVPARSER_CSVPARSER_H
