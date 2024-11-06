#include "superheader.h"  // Instead of individual includes


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

const std::unordered_map<int, std::string> tokenNames = {
    {UNDEF, "UNDEF"}, {ID, "ID"}, {INTEGER, "INTEGER"}, {CHAR, "CHAR"},
    {STRING, "STRING"}, {PLUS, "PLUS"}, {MINUS, "MINUS"}, {MULTIPLY, "MULTIPLY"},
    {DIVIDE, "DIVIDE"}, {ASSIGN, "ASSIGN"}, {EQUAL, "EQUAL"}, {NOT_EQUAL, "NOT_EQUAL"},
    {LESS_EQUAL, "LESS_EQUAL"}, {LESS, "LESS"}, {GREATER_EQUAL, "GREATER_EQUAL"},
    {GREATER, "GREATER"}, {AND, "AND"}, {OR, "OR"}, {NOT, "NOT"},
    {LPAREN, "LPAREN"}, {RPAREN, "RPAREN"}, {LBRACE, "LBRACE"}, {RBRACE, "RBRACE"},
    {LBRACKET, "LBRACKET"}, {RBRACKET, "RBRACKET"}, {COMMA, "COMMA"},
    {SEMICOLON, "SEMICOLON"}, {IF, "IF"}, {ELSE, "ELSE"}, {WHILE, "WHILE"},
    {FOR, "FOR"}, {RETURN, "RETURN"}, {VOID, "VOID"}, {CHAR_TYPE, "CHAR_TYPE"},
    {INT_TYPE, "INT_TYPE"}, {END_OF_FILE, "END_OF_FILE"}
};

class Token {
public: 
    int name;
    string lexeme;

    Token(int name) : name(name) {}

    Token(int name, string l) : name(name), lexeme(l) {}
};
