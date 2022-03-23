grammar Json;


json    :   object
        |   array
        ;


object  :   '{' pair (',' pair)* '}'
        |   '{' '}'
        ;


array   :   '[' value (',' value)* ']'
        |   '[' ']'
        ;


pair    :   STRING ':' value;


value   :   STRING
        |   NUMBER
        |   object
        |   array
        |   'true' | 'false'
        |   'null'
        ;



/* 字符串 */
STRING  :   '"' (ESC | ~["\\])* '"';

/* 转义字符 或 UNICODE */
fragment ESC        :   '\\' (["\\/bfnrt] | UNICODE);

/* 4位16进制数 */
fragment UNICODE    :   'u' HEX HEX HEX HEX;
fragment HEX        :   [0-9a-fA-F];


/* 数字 */
NUMBER  :   '-'? INT '.' INT EXP?
        |   '-'? INT EXP
        |   '-'? INT
        ;

fragment INT        :   '0' | [1-9] [0-9]*;

fragment EXP        :   [eE] [+\-]? INT;

WS      :   [ \t\n\r]+ -> skip;
