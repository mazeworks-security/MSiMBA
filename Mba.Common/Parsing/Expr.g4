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
            | expression ('zx') WIDTH_SPECIFIER #ZextExpression
            | expression ('sx') WIDTH_SPECIFIER #SextExpression
            | expression ('tr') WIDTH_SPECIFIER #TruncExpression
            | ('Const') LPARAM ID RPARAM #WildCardNumberExpression
            | NUMBER (':' WIDTH_SPECIFIER)? #NumberExpression
            | ID (':' WIDTH_SPECIFIER)? #IdExpression
            ;



// Expression constructs
LPARAM      : '(';
RPARAM      : ')';
COMMA       : ',';


STRING      : ('"' ~["]* '"') | '%' STRING;

// DECIMAL      : [0-9]+;
// HEXADECIMAL: '0x' ([a-fA-F0-9])+;
NUMBER: ([0-9]+) | ('0x' ([a-fA-F0-9])+);
WIDTH_SPECIFIER: 'i' NUMBER;

ID          : [a-zA-Z] ([a-zA-Z0-9_])*;

WS          : [ \t\r\n]+ -> skip ;


