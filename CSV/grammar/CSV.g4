grammar CSV; //grammar for CSV

options { language=Cpp; }

file : hdr row+ ;
hdr : row ;

row : field (',' field)* '\r'? '\n' ;

field
    :   text
    |   string
    |   empty
    ;

text : TEXT ;
string : STRING ;
empty :  ;
TEXT : ~[,\n\r"]+ ;
STRING : '"' ('""'|~'"')* '"' ; // quote-quote is an escaped quote