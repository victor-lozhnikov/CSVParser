//
// Created by victor on 13.12.19.
//

#include <iterator>
#include <iostream>
#include <algorithm>
#include "CSVParser.h"

#ifndef CSVPARSER_PARSERITERATOR_H
#define CSVPARSER_PARSERITERATOR_H

template <typename ...Args>
class CSVParser;

template <typename ...Args>
class ParserIterator : public std::iterator<std::input_iterator_tag, std::tuple<Args...>> {
public:
    friend class CSVParser<Args...>;
    explicit ParserIterator (std::ifstream *_file, bool _eof = false) {
        file = _file;
        tpl = new std::tuple <Args...>;
        if (_eof) {
            eof = true;
            return;
        }
        *file >> *tpl;
        if (file->eof()) {
            eof = true;
            return;x`
        }
        eof = false;
    }

    ParserIterator &operator++() {
        *file >> *tpl;
        if (file->eof()) {
            eof = true;
        }
        return *this;
    }

    bool operator==(ParserIterator const &other) const {
        if (eof && other.eof) {
            return true;
        }
        return *tpl == *other.tpl;
    }

    bool operator!=(ParserIterator const &other) const {
        return !(*this == other);
    }

    typename ParserIterator::reference operator*() const {
        return *tpl;
    }

    explicit operator std::tuple<Args...>() {
        return tpl;
    }

private:
    std::tuple<Args...>* tpl;
    std::ifstream *file;
    bool eof;
};

template <typename Head, typename ...Tail>
std::tuple <Head, Tail...> read_tuple(std::ifstream &is) {
    Head head;
    is.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    try {
        is >> head;
    }
    catch (std::ifstream::failure &e) {
        if (is.eof()) {
            return std::tuple<Head, Tail...>{};
        }
        std::cerr << "EXCEPTION: wrong format\n";
        exit(1);
    }

    if constexpr (sizeof...(Tail) == 0) {
        return std::tuple{head};
    }
    else {
        return std::tuple_cat(std::tuple{head}, read_tuple<Tail...>(is));
    }
}

template <typename ...Args>
std::ifstream& operator>> (std::ifstream &is, std::tuple <Args...> &t) {
    t = read_tuple<Args...>(is);
    return is;
}

template <std::size_t> struct Int{};

template <typename Ch, typename Tr, typename Tuple, size_t Pos>
std::ostream &print_tuple(std::basic_ostream<Ch, Tr> &os, const Tuple &t, Int<Pos>) {
    os << std::get <std::tuple_size<Tuple>::value - Pos> (t) << ", ";
    return print_tuple(os, t, Int<Pos - 1>());
}

template <typename Ch, typename Tr, typename Tuple>
std::ostream &print_tuple(std::basic_ostream<Ch, Tr> &os, const Tuple &t, Int<1>) {
    return os << std::get <std::tuple_size<Tuple>::value - 1> (t);
}

template <typename Ch, typename Tr, typename... Args>
std::ostream &operator<<(std::basic_ostream<Ch, Tr> &os, const std::tuple<Args...> &t) {
    os << "[";
    print_tuple(os, t, Int<sizeof...(Args)>());
    return os << "]";
}

#endif //CSVPARSER_PARSERITERATOR_H