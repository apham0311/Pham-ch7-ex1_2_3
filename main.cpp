/*
 *  UCF COP3330 Fall 2021 Assignment 6 Solution
 *  Copyright 2021 first_name last_name
 */

#include <iostream>
#include "std_lib_facilities.h"


//creating a token
struct Token {
    char type;
    double value;
    string name;
    Token(char ch) :type(ch), value(0) { }
    Token(char ch, double val) :type(ch), value(val) { }
    Token(char ch, string val) : type(ch),name(val) { }
};

//tokens stream
class Token_stream {
    bool full;
    Token buffer;
public:
    Token_stream() :full(0), buffer(0) { }

    Token get();
    void putback(Token t) { buffer = t; full = true; }

    void ignore(char);
};

const char let = 'L';
const char print = ';';
const char name = 'N';
const char number = '1';
const char result = '=';
const char constant = 'c';
const char pi = 'pi';
const char e = 'e';
const char squareRoot = 'S';
const char quit = 'Q';


//gets chars from user
Token Token_stream::get()
{
    if (full) { full = false; return buffer; }
    char ch;
    cin >> ch;
    switch (ch) {
        case '(':
        case ')':
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case ';':
        case '=':
            return Token(ch);
        case '.':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {	cin.unget();
            double val;
            cin >> val;
            return Token(number, val);
        }
        default:
        {
            if (isalpha(ch) || ch == '_') {
                string s;
                s += ch;
                while (cin.get(ch) && (isalpha(ch) || isdigit(ch)) || ch == '_') 	//ex1
                {
                    s += ch;
                }
                cin.unget();
                if (s == "L") return Token(let);
                if (s == "quit") return Token(quit);
                if(s == "=") return Token(result);
                if(s == "const") return Token(constant);
                if (s == "pi") return Token(pi);
                if (s == "e") return Token(e);
                if (s == "sqrt") return Token(squareRoot);
                return Token(name, s);

            }
            error("Bad token");
            return Token(' ');
        }
    }
}

void Token_stream::ignore(char c)
{
    if (full && c == buffer.type) {
        full = false;
        return;
    }
    full = false;
    char ch = 0;
    while (cin >> ch)
        if (ch == c) return;
}

//Name and variable
struct Variable {
    string name;
    double value;
    Variable(string n, double v) :name(n), value(v) { }
};


vector<Variable> names;

//getter for value in vector
double getValue(string s)
{
    for (int i = 0; i < names.size(); ++i)
        if (names[i].name == s) return names[i].value;
    error("get: undefined name ", s);

    return 0;
}

//Set the value
void setValue(string s, double d) {
    for (int i = 0; i <= names.size(); ++i)
        if (names[i].name == s) {
            names[i].value = d;
            return;
        }
    error("set: undefined name ", s);
}

//checks if a variable has already been used
bool isDeclared(string s)
{
    for (int i = 0; i < names.size(); ++i)
        if (names[i].name == s) return true;
    return false;
}

Token_stream ts;

//calls expression
double expression();

//For the different cases
double primary()
{
    Token t = ts.get();
    switch (t.type) {
        case '(':
        {	double d = expression();
            t = ts.get();
            if (t.type != ')') error("'(' expected");
            return d;
        }
        case number:
            return t.value;
        case name:
            return getValue(t.name);
        case '+':
            return primary();
        case '-':
            return -primary();
        case pi:
        {
            return 3.14;
        }
        case e:
        {
            return 2.7182818284;
        }
        case squareRoot:
        {
            t = ts.get();
            double x = expression();
            if (x < 0) error("negative square root cannot be calculated");
            t = ts.get();
            return sqrt(x);
        }

        default:
            error("primary expected");
    }

    return 0;
}

//Math operations
double term()
{
    double left = primary();
    while (true) {
        Token t = ts.get();
        switch (t.type) {
            case '*':
                left *= primary();
                break;
            case '/':
            {	double d = primary();
                if (d == 0) error("Cannot divide by zero");
                left /= d;
                break;
            }
            case '%':
            {
                int i1 = narrow_cast<int>(left);
                int i2 = narrow_cast<int>(term());
                if (i2 == 0) error("%: Cannot divide by zero");
                left = i1%i2;
                t = ts.get();
                break;
            }
            default:
                ts.putback(t);
                return left;
        }
    }
}

double expression()
{
    double left = term();
    while (true) {
        Token t = ts.get();
        switch (t.type) {
            case '+':
                left += term();
                break;
            case '-':
                left -= term();
                break;
            default:
                ts.putback(t);
                return left;
        }
    }
}

//this checks how they are declaring variables and also declares them if correct
double declaration()
{
    Token t = ts.get();
    if (t.type != 'a') error("name expected in declaration");
    string name = t.name;
    if (isDeclared(name)) error(name, " declared twice");
    Token t2 = ts.get();
    if (t2.type != '=') error("= missing in declaration of ", name);
    double d = expression();
    names.push_back(Variable(name, d));
    return d;
}


double statement()
{
    Token t = ts.get();
    switch (t.type) {
        case let:
            return declaration();
        case result:
            return declaration();
        default:
            ts.putback(t);
            return expression();
    }
}

void cleanMess()
{
    ts.ignore(print);
}

const string prompt = "> ";
const string equal = "= ";


void calculate()
{
    while (true) try {
            cout << prompt;
            Token t = ts.get();
            while (t.type == print) t = ts.get();
            if (t.type == quit) return;
            ts.putback(t);
            cout << result << statement() << endl;
        }
        catch (runtime_error& e) {
            cerr << e.what() << endl;
            cleanMess();
        }
}


int main()

try {
    calculate();
    return 0;
}
catch (exception& e) {
    cerr << "exception: " << e.what() << endl;
    char c;
    while (cin >> c && c != ';');
    return 1;
}
catch (...) {
    cerr << "exception\n";
    char c;
    while (cin >> c && c != ';');
    return 2;
}