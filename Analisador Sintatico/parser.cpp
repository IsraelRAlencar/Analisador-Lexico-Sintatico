// parser.cpp
#include "parser.h"

Parser::Parser(string input) {
    scanner = new Scanner(input);
}

void Parser::advance() {
    lToken = scanner->nextToken();
    // Mensagem de debug: mostrar o token atual
    std::cout << "Avancando para o proximo token: " << lToken->name << " (" << lToken->lexeme << ")" << std::endl;
}

void Parser::match(int t) {
    // Mensagem de debug: mostrar o token esperado e o token atual
    std::cout << "Esperado: " << t << ", Atual: " << lToken->name << " (" << lToken->lexeme << ")" << std::endl;
    if (lToken->name == t)
        advance();
    else
        error("Erro inesperado em match()");
}

void Parser::error(string str) {
    std::cout << "Linha " << scanner->getLine() << ": " << str << std::endl;
    std::cout << "Token atual: " << lToken->name << " (" << lToken->lexeme << ")" << std::endl;
    exit(EXIT_FAILURE);
}

void Parser::run() {
    advance();    
    program();
    std::cout << "Compilação encerrada com sucesso!\n";
}

// Program -> (Function)* EOF
void Parser::program() {
    while (lToken->name != END_OF_FILE)
        function();
    match(END_OF_FILE);
}

// Function -> Type ID ( ParamTypes ) { (Type VarDeclaration (, VarDeclaration)*)* (Statement)* }
void Parser::function() {
    type();             // Espera 'int' ou 'char' para iniciar uma função
    match(ID);          // Nome da função, como 'main'
    match(LPAREN);      // '(' para parâmetros
    paramTypes();       // Parâmetros da função
    match(RPAREN);      // ')'
    match(LBRACE);      // '{' início do bloco da função
    
    // Declarações de variáveis e statements
    while (lToken->name == INT_TYPE || lToken->name == CHAR_TYPE) {
        type();
        varDeclaration();
        while (lToken->name == COMMA) {
            match(COMMA);
            varDeclaration();
        }
        match(SEMICOLON);
    }

    // Statements da função
    while (lToken->name != RBRACE) {
        statement();
    }

    match(RBRACE);
}

// VarDeclaration -> ID ([integerconstant])?
void Parser::varDeclaration() {
    match(ID);  // Nome da variável
    if (lToken->name == ASSIGN) {  // Permite inicialização com '='
        match(ASSIGN);
        expression();  // Expressão à direita da atribuição
    }
    if (lToken->name == LBRACKET) {  // Para arrays: int arr[5];
        match(LBRACKET);
        match(INTEGER);
        match(RBRACKET);
    }
}

// Type -> char | int
void Parser::type() {
    if (lToken->name == CHAR_TYPE)
        match(CHAR_TYPE);
    else if (lToken->name == INT_TYPE)
        match(INT_TYPE);
    else
        error("Tipo esperado: char ou int");
}

// ParamTypes -> void | Type ID([])? (, Type ID([])? )*
void Parser::paramTypes() {
    if (lToken->name == VOID) {
        match(VOID);  // Função com "void" como parâmetro
    } else if (lToken->name == RPAREN) {
        // Parênteses vazios: função sem parâmetros, apenas retorna
        return;
    } else {
        // Caso tenha parâmetros, processa-os normalmente
        type();
        match(ID);
        if (lToken->name == LBRACKET) {
            match(LBRACKET);
            match(RBRACKET);
        }
        while (lToken->name == COMMA) {
            match(COMMA);
            type();
            match(ID);
            if (lToken->name == LBRACKET) {
                match(LBRACKET);
                match(RBRACKET);
            }
        }
    }
}

// Statement para múltiplas declarações
void Parser::statement() {
    if (lToken->name == IF) {
        match(IF);
        match(LPAREN);
        expression();
        match(RPAREN);
        statement();
        if (lToken->name == ELSE) {
            match(ELSE);
            statement();
        }
    } else if (lToken->name == WHILE) {
        match(WHILE);
        match(LPAREN);
        expression();
        match(RPAREN);
        statement();
    } else if (lToken->name == FOR) {
        match(FOR);
        match(LPAREN);
        if (lToken->name == INT_TYPE || lToken->name == ID) assign();
        match(SEMICOLON);
        if (lToken->name != SEMICOLON) expression();
        match(SEMICOLON);
        if (lToken->name == ID) assign();
        match(RPAREN);
        statement();
    } else if (lToken->name == RETURN) {
        match(RETURN);
        if (lToken->name != SEMICOLON) expression();
        match(SEMICOLON);
    } else if (lToken->name == LBRACE) {
        match(LBRACE);
        while (lToken->name != RBRACE) statement();
        match(RBRACE);
    } else if (lToken->name == SEMICOLON) {
        match(SEMICOLON);
    } else if (lToken->name == ID) {
        assign();
        match(SEMICOLON);
    } else {
        error("Erro inesperado no Statement");
    }
}

// Assign -> ID ([ Expression ])? = Expression
void Parser::assign() {
    match(ID);
    if (lToken->name == LBRACKET) {
        match(LBRACKET);
        expression();
        match(RBRACKET);
    }
    match(ASSIGN);
    expression();
}

// Expression (simplificada)
void Parser::expression() {
    if (lToken->name == ID) {
        match(ID);
        if (lToken->name == LPAREN) {
            match(LPAREN);
            if (lToken->name != RPAREN) expression();
            while (lToken->name == COMMA) {
                match(COMMA);
                expression();
            }
            match(RPAREN);
        }
    } else if (lToken->name == INTEGER) {
        match(INTEGER);
    } else if (lToken->name == CHAR) {
        match(CHAR);
    } else if (lToken->name == STRING) {
        match(STRING);
    } else if (lToken->name == LPAREN) {
        match(LPAREN);
        expression();
        match(RPAREN);
    } else {
        error("Expressão esperada");
    }
}