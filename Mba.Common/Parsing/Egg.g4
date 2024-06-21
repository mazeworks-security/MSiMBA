grammar Egg;

egg: expr EOF;

expr: LPARAM OPERATOR expr expr? RPARAM #EggExpression
    | NEGATIVE_NUMBER_PREFIX? NUMBER #NumberExpression
    | ID #IdExpression
    ;



// Expression constructs
LPARAM      : '(';
RPARAM      : ')';
COMMA       : ',';
OPERATOR    : '**' | '~' | '+' | '*' | '&' | '^' | '|' | '>>>' | '>>' | '<<';

STRING      : ('"' ~["]* '"') | '%' STRING;

// DECIMAL      : [0-9]+;
// HEXADECIMAL: '0x' ([a-fA-F0-9])+;
NEGATIVE_NUMBER_PREFIX: '-';
NUMBER: ([0-9]+) | ('0x' ([a-fA-F0-9])+);

ID          : [a-zA-Z] ([a-zA-Z0-9_])*;

WS          : [ \t\r\n]+ -> skip ;


