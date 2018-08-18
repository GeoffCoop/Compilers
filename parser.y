%{
#include<iostream>
#include<fstream>
#include "Code/FrontEnd.hpp"


extern int yylex();
extern int yyparse();
extern int lineNum;
void yyerror(const char*);
%}

%union
{
	char* str_val;
	int int_val;
	char char_val;
}

%error-verbose
%token ARRAY_SYMBOL
%token BEGIN_SYMBOL
%token CHR_SYMBOL
%token CONST_SYMBOL
%token DO_SYMBOL
%token DOWNTO_SYMBOL
%token ELSE_SYMBOL
%token ELSEIF_SYMBOL
%token END_SYMBOL
%token FOR_SYMBOL
%token FORWARD_SYMBOL
%token FUNCTION_SYMBOL
%token IF_SYMBOL
%token OF_SYMBOL
%token ORD_SYMBOL
%token PRED_SYMBOL
%token PROCEDURE_SYMBOL
%token READ_SYMBOL
%token RECORD_SYMBOL
%token REF_SYMBOL
%token REPEAT_SYMBOL
%token RETURN_SYMBOL
%token STOP_SYMBOL
%token SUCC_SYMBOL
%token THEN_SYMBOL
%token TO_SYMBOL
%token TYPE_SYMBOL
%token UNTIL_SYMBOL
%token VAR_SYMBOL
%token WHILE_SYMBOL
%token WRITE_SYMBOL

%token IDENT_SYMBOL
%token CHRCONST_SYMBOL
%token INT_SYMBOL
%token STRING_SYMBOL

%token PLUS_SYMBOL
%token MINUS_SYMBOL
%token MULT_SYMBOL
%token DIV_SYMBOL
%token AND_SYMBOL
%token OR_SYMBOL
%token NOT_SYMBOL
%token EQUAL_SYMBOL
%token NOTEQUAL_SYMBOL	
%token LT_SYMBOL
%token LTE_SYMBOL
%token GT_SYMBOL
%token GTE_SYMBOL
%token DOT_SYMBOL
%token COMMA_SYMBOL
%token COLON_SYMBOL
%token SCOLON_SYMBOL
%token LPAREN_SYMBOL
%token RPAREN_SYMBOL
%token LBRACKET_SYMBOL
%token RBRACKET_SYMBOL
%token ASSIGN_SYMBOL
%token MODULO_SYMBOL

%left AND_SYMBOL OR_SYMBOL
%right NOT_SYMBOL
%nonassoc EQUAL_SYMBOL NOTEQUAL_SYMBOL LT_SYMBOL LTE_SYMBOL GT_SYMBOL GTE_SYMBOL
%left PLUS_SYMBOL MINUS_SYMBOL MULT_SYMBOL DIV_SYMBOL MODULO_SYMBOL
%right UNARYMINUS_SYMBOL

%type <char_val> CHRCONST_SYMBOL
%type <int_val> INT_SYMBOL
%type <str_val> IDENT_SYMBOL
%type <str_val> STRING_SYMBOL

%type <int_val> Expression
%type <int_val> OptArguments
%type <int_val> Arguments

%%
Program: ProgramHead Block DOT_SYMBOL{}
	;

ProgramHead: OptConstDecl OptTypeDecl OptVarDecls PFDecl
	;

OptConstDecl: CONST_SYMBOL ConstDecls
	|
	;

ConstDecls: ConstDecls ConstDecl
	| ConstDecl
	;

ConstDecl: IDENT_SYMBOL EQUAL_SYMBOL Expression SCOLON_SYMBOL {}
	;

PFDecl: PFDecl ProcedureDecl
	| PFDecl FunctionDecl
	|
	;

ProcedureDecl: PROCEDURE_SYMBOL IDENT_SYMBOL LPAREN_SYMBOL FormalParameters RPAREN_SYMBOL SCOLON_SYMBOL FORWARD_SYMBOL SCOLON_SYMBOL {}
	| PROCEDURE_SYMBOL IDENT_SYMBOL LPAREN_SYMBOL FormalParameters RPAREN_SYMBOL SCOLON_SYMBOL Body SCOLON_SYMBOL
	;

FormalParameters: {}
	| FormalParameters SCOLON_SYMBOL FormalParameter {}
	| FormalParameter	{}
	;

FormalParameter: OptVarRef IdentList COLON_SYMBOL Type {}
	;

FunctionDecl: FUNCTION_SYMBOL IDENT_SYMBOL LPAREN_SYMBOL FormalParameters RPAREN_SYMBOL COLON_SYMBOL Type SCOLON_SYMBOL FORWARD_SYMBOL SCOLON_SYMBOL
	| FUNCTION_SYMBOL IDENT_SYMBOL LPAREN_SYMBOL FormalParameters RPAREN_SYMBOL COLON_SYMBOL Type SCOLON_SYMBOL Body SCOLON_SYMBOL
	;

OptVarRef: VAR_SYMBOL {}
	| REF_SYMBOL {}
	| {}
	; 

Body: OptConstDecl OptTypeDecl OptVarDecls Block {}
	;

Block: BEGIN_SYMBOL StatementSequence END_SYMBOL {}
	;

OptTypeDecl: TYPE_SYMBOL TypeDecls
	|
	;

TypeDecls: TypeDecl
	| TypeDecls TypeDecl
	;

TypeDecl: IDENT_SYMBOL EQUAL_SYMBOL Type SCOLON_SYMBOL
	;

Type: SimpleType
	| RecordType
	| ArrayType
	;

SimpleType: IDENT_SYMBOL {}
	;

RecordType: RECORD_SYMBOL RecordDecls END_SYMBOL
	;

RecordDecls: 
	| RecordDecls RecordDecl
	;

RecordDecl: IdentList COLON_SYMBOL Type SCOLON_SYMBOL
	;

ArrayType: ARRAY_SYMBOL LBRACKET_SYMBOL Expression COLON_SYMBOL Expression RBRACKET_SYMBOL OF_SYMBOL Type
	;

IdentList: IDENT_SYMBOL
	| IdentList COMMA_SYMBOL IDENT_SYMBOL
	;

OptVarDecls:
	| VAR_SYMBOL VarDecls
	;

VarDecls: VarDecl
	| VarDecls VarDecl
	;

VarDecl: IdentList COLON_SYMBOL Type SCOLON_SYMBOL
	;

StatementSequence: Statement {}
	| StatementSequence SCOLON_SYMBOL Statement
	;

Statement: Assignment
	| IfStatement
	| WhileStatement
	| RepeatStatement
	| ForStatement
	| StopStatement
	| ReturnStatement
	| ReadStatement
	| WriteStatement
	| ProcedureCall
	|
	;

Assignment: LValue ASSIGN_SYMBOL Expression {}
	;

IfStatement: IF_SYMBOL Expression THEN_SYMBOL StatementSequence ElseIf OptElse END_SYMBOL
	;


ElseIf: ELSEIF_SYMBOL Expression THEN_SYMBOL StatementSequence ElseIf
	|
	;

OptElse: 
	| ELSE_SYMBOL StatementSequence {}
	;

WhileStatement: WHILE_SYMBOL Expression DO_SYMBOL StatementSequence END_SYMBOL
	;

RepeatStatement: REPEAT_SYMBOL StatementSequence UNTIL_SYMBOL Expression
	;

ForStatement: FOR_SYMBOL IDENT_SYMBOL ASSIGN_SYMBOL Expression ToDownTo Expression DO_SYMBOL StatementSequence END_SYMBOL {}
	;

ToDownTo: TO_SYMBOL
	| DOWNTO_SYMBOL
	;

StopStatement: STOP_SYMBOL
	;

ReturnStatement: RETURN_SYMBOL
	| RETURN_SYMBOL Expression
	;

ReadStatement: READ_SYMBOL LPAREN_SYMBOL LValues RPAREN_SYMBOL
	;

LValues: LValues COMMA_SYMBOL LValue
	| LValue
	;

WriteStatement: WRITE_SYMBOL LPAREN_SYMBOL WriteArgs RPAREN_SYMBOL
	;

WriteArgs: WriteArgs COMMA_SYMBOL Expression
	| Expression
	;

Arguments: Arguments COMMA_SYMBOL Expression 	{}
	| Expression				{}
	;

ProcedureCall: IDENT_SYMBOL LPAREN_SYMBOL OptArguments RPAREN_SYMBOL
	;

OptArguments: 	{$$ = -1;}
	| Arguments 	{$$ = $1;}
	;

Expression: Expression OR_SYMBOL Expression	{$$=FE::OrExpr($1,$3);}
	| Expression AND_SYMBOL Expression	{$$=FE::AndExpr($1,$3);}
	| Expression EQUAL_SYMBOL Expression	{$$=FE::EQExpr($1,$3);}
	| Expression NOTEQUAL_SYMBOL Expression	{$$=FE::NEExpr($1,$3);}
	| Expression LTE_SYMBOL Expression	{$$= FE::LTEExpr($1,$3);}	
	| Expression GTE_SYMBOL Expression	{ $$ = FE::GTEExpr($1,$3);}
	| Expression LT_SYMBOL Expression	{ $$ = FE::LTExpr($1,$3);}
	| Expression GT_SYMBOL Expression	{ $$ = FE::GTExpr($1,$3); }
	| Expression MINUS_SYMBOL Expression	{ $$ = FE::MinExpr($1,$3); }
	| Expression MULT_SYMBOL Expression	{ $$ = FE::MultExpr($1,$3); }
	| Expression DIV_SYMBOL Expression	{ $$ = FE::DivExpr($1,$3); }
	| Expression MODULO_SYMBOL Expression	{ $$ = FE::ModExpr($1,$3); }
	| NOT_SYMBOL Expression			{ $$ = FE::NotExpr($2); }
	| MINUS_SYMBOL Expression %prec UNARYMINUS_SYMBOL		{ $$ = FE::UMinusExpr($2); }
	| LPAREN_SYMBOL Expression RPAREN_SYMBOL{ $$ = $2; }
	| IDENT_SYMBOL LPAREN_SYMBOL OptArguments RPAREN_SYMBOL	{ $$ = FE::CallFunction($1, $3); }
	| CHR_SYMBOL LPAREN_SYMBOL Expression RPAREN_SYMBOL		{ $$ = FE::ChrExpr($3); }
	| ORD_SYMBOL LPAREN_SYMBOL Expression RPAREN_SYMBOL		{ $$ = FE::OrdExpr($3); }
	| PRED_SYMBOL LPAREN_SYMBOL Expression RPAREN_SYMBOL		{$$ = FE::PredExpr($3); }
	| SUCC_SYMBOL LPAREN_SYMBOL Expression RPAREN_SYMBOL		{$$ = FE::SuccExpr($3); }
	| LValue					{ $$ = FE::LValExpr($1); }
	| CHRCONST_SYMBOL			{ $$ = FE::CharExpr($1); }
	| INT_SYMBOL				{ $$ = FE::IntExpr($1); }
	| STRING_SYMBOL				{ $$ = FE::StringExpr($1); }
	;

LValue: IDENT_SYMBOL										{ $$ = FE::LValID($1); }
	| LValue DOT_SYMBOL IDENT_SYMBOL						{ $$ = FE::LValMemberAccess($1, $3); }
	| LValue LBRACKET_SYMBOL Expression RBRACKET_SYMBOL		{ $$ = FE::LValArrayAccess($1, $3);}
	;

%%

int main(int, char**) {
	yyparse();
}

void yyerror(const char* msg)
{
	std::cerr<< lineNum <<msg << std::endl;
}





