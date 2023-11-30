#include "tool.h"
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

std::string to_string(int num) {
    std::stringstream ss;
    ss << num;
    return ss.str();
}

std::string to_string(double num) {
    std::stringstream ss;
    ss << num;
    return ss.str();
}

std::string to_string(long long num) {
    std::stringstream ss;
    ss << num;
    return ss.str();
}


int stoi(const std::string& str) {
    int result = 0;
    int sign = 1;
    size_t i = 0;

    // Handling sign if it exists
    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }

    while (i < str.length()) {
        char digit = str[i];
        if (digit >= '0' && digit <= '9') {
            int value = digit - '0';
            result = result * 10 + value;
            i++;
        } else {
            // Invalid character found, return 0 or handle error accordingly
            return 0;
        }
    }

    return result * sign;
}

