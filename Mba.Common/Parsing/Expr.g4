grammar Expr;

gamba: (expression EOF) | (dsl EOF);

dsl: functionGroup* ruleGroup+ EOF;


functionGroup: FUNCTIONS ID ('[') function+ (']');

function:
    BUILTIN ('fn') ID LPARAM functionArgument ((',') functionArgument)* RPARAM ('->') type (';') #BuiltinFunction
    | ('fn') ID LPARAM functionArgument ((',') functionArgument)* RPARAM '->' type ('{') RETURN expression (';') ('}') #ImplementedFunction;

functionArgument: ID (':') type;

type: ID;

ruleGroup: RULES ID ('[') rewriteRule+ (']');

rewriteRule: ID (':') expression ('=>') expression ('::' expression)?;


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
            | expression ('zx') type #ZextExpression
            | expression ('sx') type #SextExpression
            | expression ('tr') type #TruncExpression
            | expression ('&&') expression #ConditionalAndExpression
            | expression ('||') expression #ConditionalOrExpression
            | expression ICMP_OPERATOR expression #ICmpExpression
            | expression ('?') expression (':') expression #SelectExpression
            | ('Const') LPARAM ID RPARAM #WildCardNumberExpression
            | ID LPARAM expression ((',') expression)* RPARAM #IntrinsicCallExpression
            | NUMBER (':' type)? #NumberExpression
            | ID (':' type)? #IdExpression
            ;



// Expression constructs
LPARAM      : '(';
RPARAM      : ')';
COMMA       : ',';
BUILTIN     : 'builtin';
RETURN      : 'return';
FUNCTIONS      : 'FUNCTIONS';
RULES      : 'RULES';

ICMP_OPERATOR: '==' | '!=' | '>' | '>=' | '<' | '<=' | '>s' | '>=s' | '<' | '<=s';

STRING      : ('"' ~["]* '"') | '%' STRING;

// DECIMAL      : [0-9]+;
// HEXADECIMAL: '0x' ([a-fA-F0-9])+;
NUMBER: ([0-9]+) | ('0x' ([a-fA-F0-9])+);

ID          : [a-zA-Z_] ([a-zA-Z0-9_])*;


WS          : [ \t\r\n]+ -> skip ;
COMMENT
    : '/*' .*? '*/' -> skip
;

LINE_COMMENT
    : '//' ~[\r\n]* -> skip
;

