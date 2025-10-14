#include <string>

class Calculator {
public:
    explicit Calculator(const std::string& expression);
    double evaluate();

private:
    bool eof() const;
    void skip_ws();
    double parse_number();

private:
    std::string s_;
    std::size_t pos_;
};
