#include "superheader.h"  // Instead of individual includes


Scanner::Scanner(string filename) : pos(0), line(1) {
    ifstream inputFile(filename, ios::in);
    string lineContent;

    if (inputFile.is_open()) {
        while (getline(inputFile, lineContent)) {
            input.append(lineContent + '\n');
        }

        inputFile.close();
    }
    else
        cout << "Unable to open file\n";
}

int Scanner::getLine() {
    return line;
}

Token* Scanner::nextToken() {
    while (pos < input.length()) {
        char current = input[pos];

        // Ignorar comentários
        if (current == '/' && input[pos + 1] == '/') {
            while (input[pos] != '\n') 
                pos++;

            line++;
            pos++;
            continue;
        }

        if (current == '/' && input[pos + 1] == '*') {
            pos += 2;

            while (!(input[pos] == '*' && input[pos + 1] == '/')) {
                if (input[pos] == '\n') 
                    line++;
                
                pos++;
            }

            pos += 2;
            continue;
        }

        // Ignorar espaços e nova linha
        if (isspace(current)) {
            if (current == '\n') 
                line++;
            
            pos++;
            continue;
        }

        // Identificar palavras reservadas e identificadores
        if (isalpha(current)) {
            string lexeme;
            while (isalnum(input[pos]) || input[pos] == '_') {
                lexeme += input[pos++];
            }

            if (lexeme == "int") 
                return new Token(INT_TYPE, lexeme);
            if (lexeme == "char") 
                return new Token(CHAR_TYPE, lexeme);
            if (lexeme == "if") 
                return new Token(IF, lexeme);
            if (lexeme == "else") 
                return new Token(ELSE, lexeme);
            if (lexeme == "while") 
                return new Token(WHILE, lexeme);
            if (lexeme == "for") 
                return new Token(FOR, lexeme);
            if (lexeme == "return") 
                return new Token(RETURN, lexeme);
            if (lexeme == "void") 
                return new Token(VOID, lexeme);

            return new Token(ID, lexeme);  // Caso não seja uma palavra reservada, é um ID
        }

        // Constantes inteiras
        if (isdigit(current)) {
            string lexeme;
            while (isdigit(input[pos]))
                lexeme += input[pos++];

            return new Token(INTEGER, lexeme);
        }

        // Constantes de caractere
        if (current == '\'') {
            pos++;
            char charVal = input[pos++];

            if (input[pos] != '\'') 
                lexicalError("Invalid char constant");
            
            pos++;
            return new Token(CHAR, string(1, charVal));
        }

        // Constantes de string
        if (current == '\"') {
            pos++;
            string lexeme;

            while (input[pos] != '\"') {
                if (input[pos] == '\n' || pos >= input.length())
                    lexicalError("Invalid string constant");
                lexeme += input[pos++];
            }

            pos++;
            return new Token(STRING, lexeme);
        }

        // Operadores e símbolos especiais
        switch (current) {
            case '+': pos++; return new Token(PLUS, "+");
            case '-': pos++; return new Token(MINUS, "-");
            case '*': pos++; return new Token(MULTIPLY, "*");
            case '/': pos++; return new Token(DIVIDE, "/");
            case '=':
                if (input[pos + 1] == '=') { pos += 2; return new Token(EQUAL, "=="); }
                else { pos++; return new Token(ASSIGN, "="); }
            case '!':
                if (input[pos + 1] == '=') { pos += 2; return new Token(NOT_EQUAL, "!="); }
                else { pos++; return new Token(NOT, "!"); }
            case '<':
                if (input[pos + 1] == '=') { pos += 2; return new Token(LESS_EQUAL, "<="); }
                else { pos++; return new Token(LESS, "<"); }
            case '>':
                if (input[pos + 1] == '=') { pos += 2; return new Token(GREATER_EQUAL, ">="); }
                else { pos++; return new Token(GREATER, ">"); }
            case '&':
                if (input[pos + 1] == '&') { pos += 2; return new Token(AND, "&&"); }
                else lexicalError("Invalid symbol &");
            case '|':
                if (input[pos + 1] == '|') { pos += 2; return new Token(OR, "||"); }
                else lexicalError("Invalid symbol |");
            case '(': 
                pos++; return new Token(LPAREN, "(");
            case ')': 
                pos++; return new Token(RPAREN, ")");
            case '{': 
                pos++; return new Token(LBRACE, "{");
            case '}': 
                pos++; return new Token(RBRACE, "}");
            case '[': 
                pos++; return new Token(LBRACKET, "[");
            case ']': 
                pos++; return new Token(RBRACKET, "]");
            case ',': 
                pos++; return new Token(COMMA, ",");
            case ';': 
                pos++; return new Token(SEMICOLON, ";");
            default:
                lexicalError("Unrecognized character: " + string(1, current));
        }
    }
    
    return new Token(END_OF_FILE, "");
}

void Scanner::lexicalError(string msg) {
    cout << "Error on line " << line << ": " << msg << endl;
    exit(EXIT_FAILURE);
}