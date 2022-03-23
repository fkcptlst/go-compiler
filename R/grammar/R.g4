grammar R;


prog
    :   (line)* EOF
    ;


/* 一行, 可能是 代码+; 代码+NL 或 NL */
line
    :   expr_or_assign (';' | NL)
    |   NL;


expr_or_assign
    :   expr ('<-' | '=' | '<<-') expr_or_assign
    |   expr
    ;


expr
    :   expr '[[' sublist ']' ']'
    |   expr '[' sublist ']'
    |   expr ('::' | ':::') expr
    |   expr ('$' | '@') expr
    |   expr '^'<assoc=right> expr      // 右边优先计算
    |   ('-' | '+') expr
    |   expr ':' expr
    |   expr USER_OP expr
    |   expr ('*' | '/') expr
    |   expr ('+' | '-') expr
    |   expr ('>' | '>=' | '<' | '<=' | '==' | '!=') expr
    |   '!' expr
    |   expr ('&' | '&&') expr
    |   expr ('|' | '||') expr
    |   '~' expr
    |   expr '~' expr
    |   expr ('->' | '>>=' | ':=') expr
    |   'function' '(' formlist? ')' expr   // 函数定义
    |   expr '(' sublist ')'                // 函数调用
    |   '{' exprlist '}'
    |   'if' '(' expr ')' expr ('else' expr)?
    |   'for' '(' ID 'in' expr ')' expr
    |   'while' '(' expr ')' expr
    |   'repeat' expr
    |   '?' expr        // 获取 expr 的帮助信息
    |   'next'
    |   'break'
    |   '(' expr ')'
    |   ID
    |   STRING
    |   HEX
    |   INT
    |   FLOAT
    |   COMPLEX
    |   'NULL'
    |   'NA'
    |   'Inf'
    |   'NaN'
    |   'TRUE'
    |   'FALSE'
    ;


USER_OP :   '%' .*? '%' ;


/* 函数定义参数列表 */
formlist    :   form (',' form)*;

form
    :   ID
    |   ID '=' expr
    |   '...'
    ;


/* 函数调用参数列表 */
sublist     :   sub (',' sub)*;

sub :   expr
    |   ID '='
    |   ID '=' expr
    |   STRING '='
    |   STRING '=' expr
    |   'NULL' '='
    |   'NULL' '=' expr
    |   '...'
    ;


exprlist
    :   expr_or_assign ((';' | NL) expr_or_assign?)*
    ;


ID  :   '.' (LETTER|'_'|'.') (LETTER|DIGIT|'_'|'.')*
    |   LETTER (LETTER|DIGIT|'_'|'.')*
    ;

fragment LETTER :   [a-zA-Z];


/* 词法 */
HEX     :   '0' ('x'|'X') HEXDIGIT+ [Ll]?;
fragment HEXDIGIT   :   ('0'..'9'|'a'..'f'|'A'..'F');

FLOAT   :   DIGIT+ '.' DIGIT* EXP? [Ll]?
        |   DIGIT+ EXP? [Ll]?
        |   '.' DIGIT+ EXP? [Ll]?
        ;
fragment DIGIT      :   '0'..'9';
fragment EXP        :   ('E'|'e') ('+'|'-')? INT;

INT     :   DIGIT+[Ll]?;

COMPLEX :   INT 'i'
        |   FLOAT 'i'
        ;


STRING  :   '"' ( ESC | ~[\\"] )*? '"'
        |   '\'' ( ESC | ~[\\'] )*? '\''
        ;
fragment ESC    :   '\\' ([abtnfrv]|'"'|'\'')
                |   UNICODE_ESCAPE
                |   HEX_ESCAPE
                |   OCTAL_ESCAPE
                ;
fragment UNICODE_ESCAPE
                :   '\\' 'u' HEXDIGIT HEXDIGIT HEXDIGIT HEXDIGIT
                |   '\\' 'u' '{' HEXDIGIT HEXDIGIT HEXDIGIT HEXDIGIT '}'
                ;

fragment OCTAL_ESCAPE
                :   '\\' [0-3] [0-7] [0-7]
                |   '\\' [0-7] [0-7]
                |   '\\' [0-7]
                ;

fragment HEX_ESCAPE
            :   '\\' HEXDIGIT HEXDIGIT?
            ;


COMMENT :   '#' .*? '\r'? '\n' -> type(NL) ;

NL      :   'r'? '\n';

WS      :   [ \t]+ -> skip ;
