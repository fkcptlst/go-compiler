grammar DOT;

// options { language=Cpp; }

graph       :   STRICT? ( GRAPH | DIGRAPH ) id? '{' stmt_list '}' ;
stmt_list   :   ( stmt  ';'? )* ;
stmt        :   node_stmt
            |	edge_stmt
            |	attr_stmt
            |	id '=' id
            |	subgraph
            ;
attr_stmt   :   ( GRAPH | NODE | EDGE ) attr_list ;
attr_list   :   ( '[' a_list? ']' )+ ;
a_list      :   ( id '=' id (';' | ',')? )+ ;
edge_stmt   :   ( node_id | subgraph ) edgeRHS ( attr_list )? ;
edgeRHS     :   ( edgeop (node_id | subgraph) )+ ;
edgeop      :   '--'
            |   '->'
            ;
node_stmt   :   node_id attr_list? ;
node_id     :   id port? ;
port        :   ':' id ( ':' id )? ;
subgraph    :   ( SUBGRAPH id? )? '{' stmt_list '}' ;
id          :   ID
            |   NUMBER
            |   STRING
            |   HTML
            ;



STRICT      :   [Ss][Tt][Rr][Ii][Cc][Tt] ;
GRAPH       :   [Gg][Rr][Aa][Pp][Hh] ;
DIGRAPH     :   [Dd][Ii][Gg][Rr][Aa][Pp][Hh] ;
SUBGRAPH    :   [Ss][Uu][Bb][Gg][Rr][Aa][Pp][Hh] ;
NODE        :   [Nn][Oo][Dd][Ee] ;
EDGE        :   [Ee][Dd][Gg][Ee] ;
ID          :   LETTER ( LETTER | NUM )* ;
NUMBER      :   '-'? ( '.' NUM+ | NUM+ ( '.' NUM* ) ? ) ;
STRING      :   '"' ( '\\"' | . )*? '"';
HTML        :   '<' ( TAG | ~[<>] )* '>' ;
PRE         :   '#' .*? ->skip ;
WS          :   [ \n\r\t]->skip ;  

fragment NUM    :   [0-9] ;
fragment LETTER :   [a-zA-Z\u0080-\u00ff_] ;
fragment TAG    :   '<' .*? '>' ;

