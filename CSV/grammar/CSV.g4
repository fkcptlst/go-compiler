grammar CSV; //grammar for CSV

options { language=Cpp; }

file: hdr row+ ;

hdr : row ;

row : field (',' field)* '\r' ? '\n';

field
    :   TEXT
    |   STRING
    |
    ;

TEXT: ~[,\n\r"]+ ;

STRING : '"' ('""'|~'"');
