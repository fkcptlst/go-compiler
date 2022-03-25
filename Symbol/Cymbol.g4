grammar Cymbol;

file:   (functionDecl | varDecl)+ ;

varDecl
    :   type ID ('=' expr)? ';'
    |   type ID '[' expr ']' ('=' exprArray)? ';'
    ;
type:   'float' | 'int' | 'void' ; // user-defined types

functionDecl
    :   type ID '(' formalParameters? ')' block // "void f(int x) {...}"
    ;
formalParameters
    :   formalParameter (',' formalParameter)*
    ;
formalParameter
    :   type ID
    ;

block:  '{' stat* '}' ;   // possibly empty statement block
stat:   block
    |   varDecl
    |   'if' expr 'then' stat ('else' stat)?
    |   'for' '('assign? ';' expr? ';' expr? ')'(block | stat)
    |   'return' expr? ';' 
    |   expr '=' expr ';' // assignment
    |   expr ';'          // func call
    ;
assign: expr '=' expr // assignment
    |   type ID ('=' expr)?
    ;   
expr:   ID '(' exprList? ')'    // func call like f(), f(x), f(1,2)
    |   ID '[' expr ']'         // array index like a[i], a[i][j]
    |   '+''+' ID
    |   '-' expr                // unary minus
    |   '!' expr                // boolean not
    |   expr '*' expr
    |   expr ('+'|'-') expr
    |   expr ('<'|'>'|'<''='|'>''=') expr
    |   expr '==' expr          // equality comparison (lowest priority op)
    |   ID                      // variable reference
    |   INT
    |   ID '+''+'
    |   '(' expr ')'
    ;
exprList : expr (',' expr)* ;   // arg list

exprArray:  '{' expr ? (',' expr)* '}' ;


ID  :   LETTER (LETTER | [0-9])* ;
fragment
LETTER : [a-zA-Z] ;

INT :   [0-9]+ ;

WS  :   [ \t\n\r]+ -> skip ;

SL_COMMENT
    :   '//' .*? '\n' -> skip
    ;
