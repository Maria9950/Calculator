#include <string>

class Calculator {
public:
    Calculator(std::string expression);
    double evaluate();

private:
    bool eof();
    void skip_ws();
    bool match(char c);

    double parse_number();
    double parse_expr();
    double parse_term();
    double parse_unary();
    double parse_primary();

private:
    std::string s_;
    std::size_t pos_;
};
