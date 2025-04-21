#ifndef NUMBER_H
#define NUMBER_H

#include "../src/config.h"

inline std::string intToRoman(int n) {
    std::vector<std::string> notation = {"I", "IV", "V", "IX", "X", "XL", "L", "XC", "C", "CD", "D", "CM", "M"};
    std::vector<int> numbers = {1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000};
    int i = numbers.size() - 1;
    std::string out;

    while(n > 0) {
        if(n / numbers[i] != 0) {
            out += notation[i];
            n -= numbers[i];
        } else {
            --i;
        }
    }

    return out;
}

#endif
