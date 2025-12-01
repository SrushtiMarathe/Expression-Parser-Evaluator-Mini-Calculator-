#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
#include <stdexcept>
using namespace std;

class Parser {
    string s;
    int i = 0;

    char peek() {
        while (i < (int)s.size() && s[i] == ' ') i++;
        return i < (int)s.size() ? s[i] : '\0';
    }
    char get() {
        char c = peek();
        if (c) i++;
        return c;
    }

    // factor = number | '(' expr ')' | unary
    double factor() {
        char c = peek();

        if (c == '+' || c == '-') {  // unary operators
            get();
            double val = factor();
            return c == '-' ? -val : val;
        }

        if (c == '(') {
            get();
            double val = expr();
            if (get() != ')') throw runtime_error("Missing ')'");
            return val;
        }

        if (isdigit(c) || c == '.') {
            return number();
        }

        throw runtime_error("Unexpected character");
    }

    // number parsing
    double number() {
        int start = i;
        while (isdigit(peek()) || peek() == '.') get();
        return stod(s.substr(start, i - start));
    }

    // term = factor {( '*' | '/' ) factor}
    double term() {
        double left = factor();
        while (true) {
            char c = peek();
            if (c == '*' || c == '/') {
                get();
                double right = factor();
                if (c == '*') left *= right;
                else {
                    if (right == 0) throw runtime_error("Division by zero");
                    left /= right;
                }
            } else break;
        }
        return left;
    }

    // expr = term {( '+' | '-' ) term}
    double expr() {
        double left = term();
        while (true) {
            char c = peek();
            if (c == '+' || c == '-') {
                get();
                double right = term();
                if (c == '+') left += right;
                else left -= right;
            } else break;
        }
        return left;
    }

public:
    double evaluate(const string &str) {
        s = str;
        i = 0;
        double val = expr();
        if (peek() != '\0') throw runtime_error("Unexpected extra characters");
        return val;
    }
};

int main() {
    Parser p;
    string input;

    cout << "Mini Expression Evaluator (C++)\n";
    cout << "Enter expression or 'quit':\n";

    while (true) {
        cout << "> ";
        if (!getline(cin, input) || input == "quit") break;

        try {
            double result = p.evaluate(input);
            cout << "Result = " << result << "\n";
        } catch (exception &e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
}
