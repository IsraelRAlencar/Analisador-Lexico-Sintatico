#include "parser.h"

Parser::Parser(string input) {
    scanner = new Scanner(input);
}

void Parser::advance() {
    lToken = scanner->nextToken();
    // Mensagem de debug: mostrar o token atual
    // std::cout << "Avancando para o proximo token: " << lToken->name << " (" << lToken->lexeme << ")" << std::endl;
}

void Parser::match(int t) {
    // Mensagem de debug: mostrar o token esperado e o token atual
    // std::cout << "Esperado: " << t << ", Atual: " << lToken->name << " (" << lToken->lexeme << ")" << std::endl;
    if (lToken->name == t)
        advance();

    else
        error("Unexpected error in match()");
}

void Parser::error(string str) {
    std::cout << "Line " << scanner->getLine() << ": " << str << std::endl;
    std::cout << "Token atual: " << lToken->name << " (" << lToken->lexeme << ")" << std::endl;
    exit(EXIT_FAILURE);
}

void Parser::run() {
    advance();    
    program();
    std::cout << "Compiled successfully!\n";
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
    match(ID);

    if (lToken->name == ASSIGN) {
        match(ASSIGN);
        expression();
    }

    if (lToken->name == LBRACKET) {
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
        error("Expected type: char ou int");
}

// ParamTypes -> void | Type ID([])? (, Type ID([])? )*
void Parser::paramTypes() {
    if (lToken->name == VOID) {
        match(VOID);  // Função com "void" como parâmetro
    } 
    
    else if (lToken->name == RPAREN) {
        // Parênteses vazios: função sem parâmetros, apenas retorna
        return;
    } 
    
    else {
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
        expression();        // Processa a expressão condicional completa
        match(RPAREN);
        statement();         // Processa o bloco do `if`
        if (lToken->name == ELSE) {
            match(ELSE);
            statement();     // Processa o bloco do `else` (opcional)
        }
    } else if (lToken->name == WHILE) {
        match(WHILE);
        match(LPAREN);
        expression();        // Processa a expressão condicional completa
        match(RPAREN);
        statement();
    } else if (lToken->name == FOR) {
        match(FOR);
        match(LPAREN);
        
        // Permitir declaração de variável na inicialização do for
        if (lToken->name == INT_TYPE || lToken->name == CHAR_TYPE) {
            type();               // Identifica o tipo de variável
            varDeclaration();     // Declara a variável
        } else if (lToken->name == ID) {
            assign();             // Atribuição se for uma variável já declarada
        }
        match(SEMICOLON);

        // Condição do for
        if (lToken->name != SEMICOLON) expression();
        match(SEMICOLON);

        // Incremento do for
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

// Expression 
void Parser::expression() {
    // Processa o primeiro elemento da expressão, que pode ser um ID ou um valor constante
    if (lToken->name == ID)
        match(ID); 
    
    else if (lToken->name == INTEGER)
        match(INTEGER); 
    
    else if (lToken->name == CHAR)
        match(CHAR); 
    
    else if (lToken->name == STRING)
        match(STRING); 
    
    else if (lToken->name == LPAREN) {
        match(LPAREN);
        expression();
        match(RPAREN);
    } 
    
    else
        error("Unexpected error in Expression");

    // Processa operadores adicionais para completar a expressão (comparação e lógica)
    while (lToken->name == EQUAL || lToken->name == NOT_EQUAL ||
           lToken->name == LESS || lToken->name == GREATER ||
           lToken->name == LESS_EQUAL || lToken->name == GREATER_EQUAL ||
           lToken->name == AND || lToken->name == OR ||
           lToken->name == PLUS || lToken->name == MINUS ||
           lToken->name == MULTIPLY || lToken->name == DIVIDE) {
        match(lToken->name);   // Consome o operador
        // Processa o próximo valor ou identificador após o operador
        if (lToken->name == ID)
            match(ID); 
        
        else if (lToken->name == INTEGER)
            match(INTEGER); 
        
        else if (lToken->name == CHAR)
            match(CHAR); 
        
        else if (lToken->name == STRING)
            match(STRING); 
        
        else if (lToken->name == LPAREN) {
            match(LPAREN);
            expression();
            match(RPAREN);
        } 
        
        else
            error("Unexpected error after operator in Expression");
    }
}