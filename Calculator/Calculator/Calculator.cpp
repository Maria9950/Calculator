#include "Calculator.h"
#include <cctype>
#include <stdexcept>

Calculator::Calculator(const std::string& expression)
    : s_(expression), pos_(0) {
    skip_ws();
}

bool Calculator::eof() const {
    return pos_ >= s_.size();
}

void Calculator::skip_ws() {
    while (!eof() && std::isspace(static_cast<unsigned char>(s_[pos_]))) {
        ++pos_;
    }
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

double Calculator::evaluate() {
    double value = parse_number();
    skip_ws();
    if (!eof()) {
        throw std::runtime_error("Unexpected characters at end");
    }
    return value;
}
