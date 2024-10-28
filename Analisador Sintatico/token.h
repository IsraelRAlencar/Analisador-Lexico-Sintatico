#include <iostream>
#include <string>

using namespace std;

enum Names {
    UNDEF = 0,
    ID,
    INTEGER,
    CHAR,
    STRING,
    PLUS, MINUS, MULTIPLY, DIVIDE, ASSIGN, EQUAL, NOT_EQUAL, LESS_EQUAL,
    LESS, GREATER_EQUAL, GREATER, AND, OR, NOT,
    LPAREN, RPAREN, LBRACE, RBRACE, LBRACKET, RBRACKET, COMMA, SEMICOLON,
    IF, ELSE, WHILE, FOR, RETURN, VOID,
    CHAR_TYPE,   // Token para tipo char
    INT_TYPE,    // Token para tipo int
    END_OF_FILE
};

class Token {
public: 
    int name;
    string lexeme;

    Token(int name) : name(name) {}

    Token(int name, string l) : name(name), lexeme(l) {}
};
