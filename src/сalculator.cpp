#include <iostream>
#include <map>
#include <sstream>

#include "RPN/RPN.hpp"

struct Creator {
    long double out = 0;
    bool isempty = 1;

    bool empty() { return isempty; }

    void emplace(char c) {
        if ('0' <= c && c <= '9') {
            out *= 10;
            out += c - '0';
            isempty = 0;
        } 
    }

    long double get() {
        long double ret = out;
        out = 0;
        isempty = 1;
        return ret;
    }

    void clear() {
        isempty = 1;
        out = 0;
    }
};

long double plus(long double&& a, long double&& b) {
    return a + b;
}

long double multy(long double&& a, long double&& b) {
    return a * b;
}

long double minus(long double&& a, long double&& b) {
    return a - b;
}

long double del(long double&& a, long double&& b) {
    return a / b;
}


int main() {
    sb::RPN<Creator> rpn;
    rpn.addBinary('+', 1, plus);
    rpn.addBinary('*', 2, multy);
    rpn.addBinary('/', 2, del);
    rpn.addBinary('-', 1, minus);
    rpn.addBrackets('(', ')');
    
    std::cout << rpn(std::cin) << '\n';
}