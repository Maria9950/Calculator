#include "Calculator.h"
#include <cctype>
#include <stdexcept>
#include <cmath>

Calculator::Calculator(std::string expression)
    : s_(expression), pos_(0) {
    skip_ws();
}

bool Calculator::eof(){
    return pos_ >= s_.size();
}

void Calculator::skip_ws() {
    while (!eof() && std::isspace(static_cast<unsigned char>(s_[pos_]))) {
        ++pos_;
    }
}

bool Calculator::match(char c) {
    skip_ws();
    if (!eof() && s_[pos_] == c) {
        ++pos_;
        return true;
    }
    return false;
}

double Calculator::parse_number() {
    skip_ws();
    std::size_t start = pos_;
    bool any = false;

    while (!eof() && std::isdigit(static_cast<unsigned char>(s_[pos_]))) {
        ++pos_;
        any = true;
    }
    if (!eof() && s_[pos_] == '.') {
        ++pos_;
        while (!eof() && std::isdigit(static_cast<unsigned char>(s_[pos_]))) {
            ++pos_;
            any = true;
        }
    }
    if (!any) {
        throw std::runtime_error("Expected number");
    }
    return std::stod(s_.substr(start, pos_ - start));
}

double Calculator::parse_primary() {
    skip_ws();
    if (match('(')) {
        double v = parse_expr();
        if (!match(')')) {
            throw std::runtime_error("Expected ')'");
        }
        return v;
    }
    return parse_number();
}

double Calculator::parse_unary() {
    skip_ws();
    if (match('+')) return parse_unary();
    if (match('-')) return -parse_unary();
    return parse_primary();
}

double Calculator::parse_term() {
    double v = parse_unary();
    while (true) {
        skip_ws();
        if (match('*')) {
            v *= parse_unary();
        }
        else if (match('/')) {
            double d = parse_unary();
            if (d == 0.0) {
                throw std::runtime_error("Division by zero");
            }
            v /= d;
        }
        else {
            break;
        }
    }
    return v;
}

double Calculator::parse_expr() {
    double v = parse_term();
    while (true) {
        skip_ws();
        if (match('+')) {
            v += parse_term();
        }
        else if (match('-')) {
            v -= parse_term();
        }
        else {
            break;
        }
    }
    return v;
}

double Calculator::evaluate() {
    double value = parse_expr();
    skip_ws();
    if (!eof()) {
        throw std::runtime_error("Unexpected characters at end");
    }
    return value;
}

