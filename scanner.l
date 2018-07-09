%{
	#include <cstring>
	#include <cstdlib>
	extern int yylex();
	#include <iostream>
using namespace std;

	#include "parser.tab.h"
%}

%option noyywrap

%%
array|ARRAY		{ cout << "array " ; return ARRAY_SYMBOL; }
begin|BEGIN		{ cout << "begin " ; return BEGIN_SYMBOL; }
chr|CHR			{ cout << "chr "; return CHR_SYMBOL;}
const|CONST		{ cout << "const " ; return CONST_SYMBOL;}
do|DO			{ cout << "do " ; return DO_SYMBOL;}
downto|DOWNTO		{ cout << "downto " ; return DOWNTO_SYMBOL;}
else|ELSE		{ cout << "else " ; return ELSE_SYMBOL;}
elseif|ELSEIF		{ cout << "elseif " ; return ELSEIF_SYMBOL;} 
end|END			{ cout << "end " ; return END_SYMBOL;}
for|FOR			{ cout << "for " ; return FOR_SYMBOL;}
forward|FORWARD		{ cout << "forward " ; return FORWARD_SYMBOL;}
function|FUNCTION	{ cout << "function " ; return FUNCTION_SYMBOL;}
if|IF			{ cout << "if " ; return IF_SYMBOL;}
of|OF			{ cout << "of " ; return OF_SYMBOL;}
ord|ORD			{ cout << "ord " ; return ORD_SYMBOL;}
pred|PRED		{ cout << "pred " ; return PRED_SYMBOL;}
procedure|PROCEDURE	{ cout << "procedure " ; return PROCEDURE_SYMBOL;}
read|READ		{ cout << "read " ; return READ_SYMBOL;}
record|RECORD		{ cout << "record " ; return RECORD_SYMBOL;}
ref|REF			{ cout << "ref " ; return REF_SYMBOL;}
repeat|REPEAT		{ cout << "repeat " ; return REPEAT_SYMBOL;}
return|RETURN		{ cout << "return " ; return RETURN_SYMBOL;} 
stop|STOP		{ cout << "stop " ; return STOP_SYMBOL;}
succ|SUCC		{ cout << "succ "; return SUCC_SYMBOL;}
then|THEN		{ cout << "then " ; return THEN_SYMBOL;}
to|TO			{ cout << "to "; return TO_SYMBOL;}
type|TYPE		{ cout << "type " ; return TYPE_SYMBOL;}
until|UNTIL		{ cout << "until " ; return UNTIL_SYMBOL;}
var|VAR			{ cout << "var " ; return VAR_SYMBOL;}
while|WHILE		{ cout << "while " ; return WHILE_SYMBOL;}
write|WRITE		{ cout << "write "; return WRITE_SYMBOL;}

"+"			{ cout << "+" ; return PLUS_SYMBOL;}
"-"			{ cout << "-" ;return MINUS_SYMBOL;}
"*"			{ cout << "*" ; return MULT_SYMBOL;}
"/"			{ cout << "/" ; return DIV_SYMBOL;}
"&"			{ cout << "&" ; return AND_SYMBOL;}
"|"			{ cout << "|" ; return OR_SYMBOL;}
"~"			{ cout << "~" ; return NOT_SYMBOL;}
"="			{ cout << "=" ; return EQUAL_SYMBOL;}
"<>"			{ cout << "<>" ; return NOTEQUAL_SYMBOL;}
"<"			{ cout << "<" ; return LT_SYMBOL;}
"<="			{ cout << "<=" ; return LTE_SYMBOL;}
">"			{ cout << ">" ; return GT_SYMBOL;}
">="			{ cout << ">=" ; return GTE_SYMBOL;}
"."			{ cout << "." ; return DOT_SYMBOL;}
","			{ cout << "," ; return COMMA_SYMBOL;}
":"			{ cout << ":" ; return COLON_SYMBOL;}
";"			{ cout << ";" ; return SCOLON_SYMBOL;}
"("			{ cout << "(" ; return LPAREN_SYMBOL;}
")"			{ cout << ")" ; return RPAREN_SYMBOL;}
"["			{ cout << "[" ; return LBRACKET_SYMBOL;}
"]"			{ cout << "]" ; return RBRACKET_SYMBOL;}
":="			{ cout << ":=" ; return ASSIGN_SYMBOL;}
"%"			{ cout << "%" ; return MODULO_SYMBOL;}

[a-zA-Z][a-zA-Z0-9_]*	{ cout << "ident:" << yytext; /*yylval.str_val = strdup(yytext);*/ return IDENT_SYMBOL; }
		
[0][0-7]+		{ cout << "num_8 " ; /*yylval.int_val = strtol(yytext, nullptr, 0);*/ return INT_SYMBOL;}
[0][x][0-9a-fA-F]+	{ cout << "num_16 " ; /*yylval.int_val = strtol(yytext, nullptr, 0);*/ return INT_SYMBOL;}
[0-9]+			{ cout << "num_10 " ; /*yylval.int_val = strtol(yytext, nullptr, 0);*/ return INT_SYMBOL;}

'\\n'			{ cout << "\\n" << endl; /*yylval.char_val = '\n';*/ return CHRCONST_SYMBOL;}
'\\r'			{ cout << "\\r" << endl; /*yylval.char_val = '\r';*/ return CHRCONST_SYMBOL;}
'\\b'			{ cout << "\\b" << endl; /*yylval.char_val = '\b';*/ return CHRCONST_SYMBOL;}
'\\t'			{ cout << "\\t" << endl; /*yylval.char_val = '\t';*/ return CHRCONST_SYMBOL;}
'\\f'			{ cout << "\\f" << endl; /*yylval.char_val = '\f';*/ return CHRCONST_SYMBOL;}

\$.*$			{cout << "comment" << endl;}
\n			{cout << "newline" << endl;}
[ \t]+			{}
.			{}

\"[a-zA-Z0-9~`!@#$%^&*()_+=\-\[\]{}\\\/><,.:;| ]*\"	{ cout << "string" << endl; /*yylval.str_val = strdup(yytext);*/ return STRING_SYMBOL;}

%%

int main(int, char**) {
while (yylex());
}

