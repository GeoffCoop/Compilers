%{
	#include <cstring>
	#include <cstdlib>
	extern int yylex();
	#include <iostream>
using namespace std;

	#include "parser.tab.h"
	int lineNum = 1;
%}

%option noyywrap

%%
array|ARRAY		{  return ARRAY_SYMBOL; }
begin|BEGIN		{  return BEGIN_SYMBOL; }
chr|CHR			{  return CHR_SYMBOL;}
const|CONST		{  return CONST_SYMBOL;}
do|DO			{  return DO_SYMBOL;}
downto|DOWNTO		{  return DOWNTO_SYMBOL;}
else|ELSE		{  return ELSE_SYMBOL;}
elseif|ELSEIF		{  return ELSEIF_SYMBOL;} 
end|END			{  return END_SYMBOL;}
for|FOR			{  return FOR_SYMBOL;}
forward|FORWARD		{  return FORWARD_SYMBOL;}
function|FUNCTION	{  return FUNCTION_SYMBOL;}
if|IF			{  return IF_SYMBOL;}
of|OF			{  return OF_SYMBOL;}
ord|ORD			{  return ORD_SYMBOL;}
pred|PRED		{  return PRED_SYMBOL;}
procedure|PROCEDURE	{  return PROCEDURE_SYMBOL;}
read|READ		{  return READ_SYMBOL;}
record|RECORD		{  return RECORD_SYMBOL;}
ref|REF			{  return REF_SYMBOL;}
repeat|REPEAT		{  return REPEAT_SYMBOL;}
return|RETURN		{  return RETURN_SYMBOL;} 
stop|STOP		{  return STOP_SYMBOL;}
succ|SUCC		{  return SUCC_SYMBOL;}
then|THEN		{  return THEN_SYMBOL;}
to|TO			{  return TO_SYMBOL;}
type|TYPE		{  return TYPE_SYMBOL;}
until|UNTIL		{  return UNTIL_SYMBOL;}
var|VAR			{  return VAR_SYMBOL;}
while|WHILE		{  return WHILE_SYMBOL;}
write|WRITE		{  return WRITE_SYMBOL;}

"+"			{  return PLUS_SYMBOL;}
"-"			{  return MINUS_SYMBOL;}
"*"			{  return MULT_SYMBOL;}
"/"			{  return DIV_SYMBOL;}
"&"			{  return AND_SYMBOL;}
"|"			{  return OR_SYMBOL;}
"~"			{  return NOT_SYMBOL;}
"="			{  return EQUAL_SYMBOL;}
"<>"			{  return NOTEQUAL_SYMBOL;}
"<"			{  return LT_SYMBOL;}
"<="			{  return LTE_SYMBOL;}
">"			{  return GT_SYMBOL;}
">="			{  return GTE_SYMBOL;}
"."			{  return DOT_SYMBOL;}
","			{  return COMMA_SYMBOL;}
":"			{  return COLON_SYMBOL;}
";"			{  return SCOLON_SYMBOL;}
"("			{  return LPAREN_SYMBOL;}
")"			{  return RPAREN_SYMBOL;}
"["			{  return LBRACKET_SYMBOL;}
"]"			{  return RBRACKET_SYMBOL;}
":="			{  return ASSIGN_SYMBOL;}
"%"			{  return MODULO_SYMBOL;}

[a-zA-Z][a-zA-Z0-9_]*	{yylval.str_val = strdup(yytext); cout << "Line:  " << lineNum << "\tIdent: " << yytext << endl; return IDENT_SYMBOL; }
		
[0][0-7]+		{ yylval.int_val = strtol(yytext, nullptr, 0); return INT_SYMBOL;}
[0][x][0-9a-fA-F]+	{ yylval.int_val = strtol(yytext, nullptr, 0); return INT_SYMBOL;}
[0-9]+			{ yylval.int_val = strtol(yytext, nullptr, 0); return INT_SYMBOL;}

'\\n'			{ yylval.char_val = '\n'; return CHRCONST_SYMBOL;}
'\\r'			{ yylval.char_val = '\r'; return CHRCONST_SYMBOL;}
'\\b'			{ yylval.char_val = '\b'; return CHRCONST_SYMBOL;}
'\\t'			{ yylval.char_val = '\t'; return CHRCONST_SYMBOL;}
'\\f'			{ yylval.char_val = '\f'; return CHRCONST_SYMBOL;}
'\\?.'			{ yylval.char_val = yytext[1]; return CHRCONST_SYMBOL;}

\$.*$			{}
\n			{lineNum++;}
[ \t]+			{}
.			{}

\"[a-zA-Z0-9~`!@#$%^&*()_+=\-\[\]{}\\\/><,.:;| ]*\"	{ yylval.str_val = strdup(yytext); return STRING_SYMBOL;}

%%

