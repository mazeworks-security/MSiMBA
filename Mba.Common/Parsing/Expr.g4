grammar Expr;

gamba: (expression EOF) | (dsl EOF);

dsl: function_group* rule_group+ EOF;


function_group: FUNCTIONS ID ('[') function+ (']');

function:
    BUILTIN ('fn') ID LPARAM function_argument ((',') function_argument)* RPARAM ('->') type (';') #BuiltinFunction
    | ('fn') ID LPARAM function_argument ((',') function_argument)* RPARAM '->' type ('{') RETURN expression (';') ('}') #ImplementedFunction;

function_argument: ID (':') type;

type: ID;

rule_group: RULES ID ('[') rewrite_rule+ (']');

rewrite_rule: ID (':') expression ('=>') expression ('::' expression)?;


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
            | expression ICMP_OPERATOR expression #ICmpExpression
            | expression ('?') expression (':') expression #SelectExpression
            | ('Const') LPARAM ID RPARAM #WildCardNumberExpression
            | ID LPARAM expression ((',') expression)* RPARAM #IntrinsicCallExpression
            | NUMBER (':' WIDTH_SPECIFIER)? #NumberExpression
            | ID (':' WIDTH_SPECIFIER)? #IdExpression
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
WIDTH_SPECIFIER: 'i' NUMBER;

ID          : [a-zA-Z_] ([a-zA-Z0-9_])*;


WS          : [ \t\r\n]+ -> skip ;
COMMENT
    : '/*' .*? '*/' -> skip
;

LINE_COMMENT
    : '//' ~[\r\n]* -> skip
;

