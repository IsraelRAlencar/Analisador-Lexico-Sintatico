#include "scanner.h"

class Parser {
private:
    Scanner* scanner;
    Token* lToken;

    void advance();          // Avança para o próximo token
    void match(int);         // Verifica e consome o token esperado
    void error(string);      // Exibe erro e encerra

    // Funções para cada produção da gramática
    void program();          // Program -> (Function)* EOF
    void function();         // Function -> Type ID (ParamTypes) { (Type VarDeclaration (, VarDeclaration)*)* (Statement)* }
    void varDeclaration();   // VarDeclaration -> ID ([integerconstant])?
    void type();             // Type -> char | int
    void paramTypes();       // ParamTypes -> void | Type ID([])? (, Type ID([])?)*
    void statement();        // Statement para múltiplas declarações
    void assign();           // Assign -> ID ([ Expression ])? = Expression
    void expression();       // Expression com várias possibilidades

public:
    Parser(string);          // Construtor que inicializa o scanner
    void run();              // Inicia a análise sintática
};