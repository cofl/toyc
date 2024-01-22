grammar toy;

// Parser Rules
program             : top_statement* EOF;
top_statement       : statement
                    | procedure
                    ;
statement           : TODO;
expression          : TODO;
procedure           : procedure_head IS statement;
procedure_head      : PROCEDURE symbol generic? parameters?;
function            : function_head IS expression;
function_head       : FUNCTION symbol generic? parameters? RETURNS type;
type                : symbol;
symbol              : TODO;
generic             : TODO;
parameters          : TODO;

// Lexer Rules
WHITESPACE: [ \t\r\n]+ -> skip;
TODO: 'todo';

PROCEDURE : 'procedure';
FUNCTION  : 'function';
IS        : 'is';
RETURNS   : 'returns';
