grammar Verilog;

sourceText
  : (moduleDecl)* EOF
  ;

moduleDecl
  : MODULE moduleId modulePortList? SEMI moduleItem* ENDMODULE
  ;

moduleId
  : identifier
  ;

modulePortList
  : LPAREN (port (COMMA port)*)? RPAREN
  ;

port
  : identifier
  ;

moduleItem
  : decl
  | assignStmt
  | moduleInstantiation
  | SEMI
  ;

decl
  : (INPUT | OUTPUT | WIRE) range? idList SEMI
  ;

range
  : LBRACK number COLON number RBRACK
  ;

idList
  : identifier (COMMA identifier)*
  ;

assignStmt
  : ASSIGN lhs EQUAL expression SEMI
  ;

lhs
  : identifier bitSelect?
  ;

bitSelect
  : LBRACK number RBRACK
  ;

moduleInstantiation
  : identifier paramOverride? identifier? LPAREN portConnList? RPAREN SEMI
  ;

paramOverride
  : HASH LPAREN namedArgList? RPAREN
  ;

portConnList
  : portConn (COMMA portConn)*
  ;

portConn
  : DOT identifier LPAREN expression? RPAREN
  ;

namedArgList
  : namedArg (COMMA namedArg)*
  ;

namedArg
  : DOT identifier LPAREN expression? RPAREN
  ;

expression
  :     LPAREN expression RPAREN #ParenthesizedExpression |
        expression ('&') expression #AndExpression |
        expression ('^') expression #XorExpression |
        expression ('|') expression #OrExpression  |
        ('~') expression #NegExpression |
        lhs #LhsExpression
  ;

//ternaryExpr
//  : orExpr (QUESTION expr COLON expr)?
//  ;
//
//orExpr
//  : xorExpr (PIPE xorExpr)*
//  ;
//
//xorExpr
//  : andExpr (CARET andExpr)*
//  ;
//
//andExpr
//  : unaryExpr (AMP unaryExpr)*
//  ;
//
//unaryExpr
//  : (TILDE | AMP | PIPE)* primary
//  ;

//primary
//  : LPAREN expr RPAREN
//  | concat
//  | atom
//  ;
//
//concat
//  : LBRACE expr (COMMA expr)+ RBRACE
//  ;

atom
  : identifier bitSelect?
  | number
  ;

number
  : NUMBER
  | UNSIZED_INT
  ;

identifier
  : IDENT
  | ESCAPED_IDENT
  ;

MODULE     : 'module';
ENDMODULE  : 'endmodule';
INPUT      : 'input';
OUTPUT     : 'output';
WIRE       : 'wire';
ASSIGN     : 'assign';

LPAREN   : '(';
RPAREN   : ')';
LBRACK   : '[';
RBRACK   : ']';
LBRACE   : '{';
RBRACE   : '}';

SEMI     : ';';
COMMA    : ',';
COLON    : ':';
DOT      : '.';
HASH     : '#';

EQUAL    : '=';
QUESTION : '?';

TILDE    : '~';
AMP      : '&';
PIPE     : '|';
CARET    : '^';

NUMBER
  : DIGITS '\'' [bBdDhHoO] [0-9a-fA-FxXzZ_]+
  ;

UNSIZED_INT
  : DIGITS
  ;

IDENT
  : [a-zA-Z_$] [a-zA-Z0-9_$]*
  ;

ESCAPED_IDENT
  : '\\' ~[ \t\r\n]+
  ;

fragment DIGITS
  : [0-9]+
  ;

ATTRIBUTE
  : '(*' .*? '*)' -> skip
  ;

LINE_COMMENT
  : '//' ~[\r\n]* -> skip
  ;

BLOCK_COMMENT
  : '/*' .*? '*/' -> skip
  ;

WS
  : [ \t\r\n]+ -> skip
  ;
