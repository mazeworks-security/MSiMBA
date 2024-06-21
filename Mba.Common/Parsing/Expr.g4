grammar Expr;

gamba: expression EOF;

expression:   LPARAM expression RPARAM                              #ParenthesizedExpression
            | expression ('**') expression #PowExpression
            | ('~'|'-') expression #NegativeOrNegationExpression
            | expression ('*') expression #MulExpression
            | expression ('+'|'-') expression #AddOrSubExpression
            | expression ('<<') expression #ShiftExpression
            | expression ('&') expression #AndExpression
            | expression ('^') expression #XorExpression
            | expression ('|') expression #OrExpression
            | expression ('>>') expression #LshrExpression
            | ('Const') LPARAM ID RPARAM #WildCardNumberExpression
            | NUMBER #NumberExpression
            | ID #IdExpression
            ;



// Expression constructs
LPARAM      : '(';
RPARAM      : ')';
COMMA       : ',';


STRING      : ('"' ~["]* '"') | '%' STRING;

// DECIMAL      : [0-9]+;
// HEXADECIMAL: '0x' ([a-fA-F0-9])+;
NUMBER: ([0-9]+) | ('0x' ([a-fA-F0-9])+);

ID          : [a-zA-Z] ([a-zA-Z0-9_])*;

WS          : [ \t\r\n]+ -> skip ;


