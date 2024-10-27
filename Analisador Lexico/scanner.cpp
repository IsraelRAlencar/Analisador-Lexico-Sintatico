#include "scanner.h"    

//Construtor que recebe uma string com o nome do arquivo 
//de entrada e preenche input com seu conteúdo.
Scanner::Scanner(string input)
{
    /*this->input = input;
    cout << "Entrada: " << input << endl << "Tamanho: " 
         << input.length() << endl;*/
    pos = 0;
    line = 1;

    ifstream inputFile(input, ios::in);
    string line;

    if (inputFile.is_open())
    {
        while (getline(inputFile,line) )
        {
            this->input.append(line + '\n');
        }
        inputFile.close();
    }
    else 
        cout << "Unable to open file\n"; 

    //A próxima linha deve ser comentada posteriormente.
    //Ela é utilizada apenas para verificar se o 
    //preenchimento de input foi feito corretamente.
    cout << this->input;

}

int
Scanner::getLine()
{
    return line;
}

//Método que retorna o próximo token da entrada
Token* 
Scanner::nextToken()
{
    Token* tok;
    string lexeme;

    while (pos < input.length()) {
        char current = input[pos];

        // Ignorar comentarios de linha
        if (current == '/' && input[pos + 1] == '/') {
            while (input[pos] != '\n') pos++;
            line++;
            continue;
        }

        // Ignorar comentarios de bloco
        if (current == '/' && input[pos + 1] == '*') {
            pos += 2;
            while (!(input[pos] == '*' && input[pos + 1] == '/')) {
                if (input[pos] == '\n') line++;
                pos++;
            }
            pos += 2;
            continue;
        }

        // Ignorar espaços e nova linha
        if (isspace(current)) {
            if (current == '\n') {
                pos++;
                continue;
            }
        }

        // Identificadores (ID)
        if (isalpha(current) || current == '_') {
            while (isalnum(current) || input[pos] == '_') {
                lexeme += current;
            }

            return new Token(ID, lexeme);
        }

        // Inteiros (INTEGER)
        if (isdigit(current)) {
            while (isdigit(input[pos])) {
                lexeme += input[pos++];
            }

            return new Token(INTEGER, lexeme);
        }

        // Caracteres (CHAR)
        if (current == '\'') {
            pos++;
            char charValue = input[pos++];

            if (input[pos] != '\'') lexicalError("Invalid char constant");
            pos++;
            return new Token(CHAR, string(1, charValue));
        }

        // Strings (STRING)
        if (current == '\"') {
            pos++;

            while (input[pos] != '\"') {
                if (input[pos] == '\n' || pos >= input.length()) lexicalError("Invalid string constant");
                lexeme += input[pos++];
            }

            pos++;
            return new Token(STRING, lexeme);
        }

        switch (current) {
            case '(': pos++; return new Token(LPAREN, "(");
            case ')': pos++; return new Token(RPAREN, ")");
            case '{': pos++; return new Token(LBRACE, "{");
            case '}': pos++; return new Token(RBRACE, "}");
            case '[': pos++; return new Token(LBRACKET, "[");
            case ']': pos++; return new Token(RBRACKET, "]");
            case ',': pos++; return new Token(COMMA, ",");
            case ';': pos++; return new Token(SEMICOLON, ";");
            case '+': pos++; return new Token(PLUS, "+");
            case '-': pos++; return new Token(MINUS, "-");
            case '*': pos++; return new Token(MULTIPLY, "*");
            case '/': pos++; return new Token(DIVIDE, "/");
            case '=':
                if (input[pos + 1] == '=') {
                    pos += 2;
                    return new Token(EQUAL, "==");
                }
                else {
                    pos++;
                    return new Token(ASSIGN, "=");
                }
            case '!':
                if (input[pos + 1] == '=') {
                    pos += 2;
                    return new Token(NOT_EQUAL, "!=");
                }
                else {
                    pos++;
                    return new Token(NOT, "!");
                }
            case '<':
                if (input[pos + 1] == '=') {
                    pos += 2;
                    return new Token(LESS_EQUAL, "<=");
                }
                else {
                    pos++;
                    return new Token(LESS, "<");
                }
            case '>':
                if (input[pos + 1] == '=') {
                    pos += 2;
                    return new Token(GREATER_EQUAL, ">=");
                }
                else {
                    pos++;
                    return new Token(GREATER, ">");
                }
            case '&':
                if (input[pos + 1] == '&') {
                    pos += 2;
                    return new Token(AND, "&&");
                }
                else {
                    lexicalError("Invalid character &");
                }
            case '|':
                if (input[pos + 1] == '|') {
                    pos += 2;
                    return new Token(OR, "||");
                }
                else {
                    lexicalError("Invalid character |");
                }
            default:
                lexicalError("Invalid character " + string(1, current));
        }
    }

    return new Token(END_OF_FILE, "");
}

void 
Scanner::lexicalError(string msg)
{
    cout << "Error on line "<< line << ": " << msg << endl;
    
    exit(EXIT_FAILURE);
}
