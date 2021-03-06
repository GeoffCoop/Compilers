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
%left PLUS_SYMBOL MINUS_SYMBOL
%left MULT_SYMBOL DIV_SYMBOL MODULO_SYMBOL
%right UNARYMINUS_SYMBOL

%type <char_val> CHRCONST_SYMBOL
%type <int_val> INT_SYMBOL
%type <str_val> IDENT_SYMBOL
%type <str_val> STRING_SYMBOL

%type <int_val> Expression
%type <int_val> OptArguments
%type <int_val> Arguments
%type <int_val> LValue
%type <int_val> ProcedureCall
%type <str_val> Procedure
%type <str_val> Function
%type <int_val> WriteArgs
%type <int_val>	WriteStatement
%type <int_val> LValues
%type <int_val> ReadStatement
%type <int_val> ReturnStatement
%type <int_val> StopStatement
%type <int_val> ToDownTo
%type <int_val> ForStatement
%type <int_val> RepeatStatement
%type <int_val> WhileStatement
%type <int_val> OptElse
%type <int_val> ElseIf
%type <int_val> IfStatement
%type <int_val> Assignment
%type <int_val> Statement
%type <int_val> StatementSequence
%type <int_val> Block


%type <int_val> IdentList
%type <int_val> SimpleType
%type <int_val> ArrayType
%type <int_val> RecordType
%type <int_val> RecordDecl
%type <int_val> RecordDecls
%type <int_val> Type
%type <int_val> OptVarRef
%type <int_val> FormalParameter
%type <int_val> FormalParameters
%type <int_val> Body

%%
Program: ProgramHead Block DOT_SYMBOL{ emitMIPS($2); }
	;

ProgramHead: OptConstDecl OptTypeDecl OptVarDecls PFDecl
	;

OptConstDecl: CONST_SYMBOL ConstDecls
	|
	;

ConstDecls: ConstDecls ConstDecl
	| ConstDecl
	;

ConstDecl: IDENT_SYMBOL EQUAL_SYMBOL Expression SCOLON_SYMBOL { addConst($1, $3); }
	;

PFDecl: PFDecl ProcedureDecl
	| PFDecl FunctionDecl
	|
	;

Procedure: PROCEDURE_SYMBOL IDENT_SYMBOL { $$ = linkProcedure($2); }
	;

ProcedureDecl: Procedure LPAREN_SYMBOL FormalParameters RPAREN_SYMBOL SCOLON_SYMBOL FORWARD_SYMBOL SCOLON_SYMBOL { addProcedure($1, $3, -1); }
	|  Procedure LPAREN_SYMBOL FormalParameters RPAREN_SYMBOL SCOLON_SYMBOL Body SCOLON_SYMBOL					 { addProcedure($1, $3, $6); }
	;

FormalParameters: 										{ $$ = -1; }
	| FormalParameters SCOLON_SYMBOL FormalParameter 	{ $$ = stackParameters($1, $3); }
	| FormalParameter									{ $$ = stackParameters(-1, $1); }
	;

FormalParameter: OptVarRef IdentList COLON_SYMBOL Type 	{ $$ = addParameter($2, $4, $1); }
	;

Function: FUNCTION_SYMBOL IDENT_SYMBOL { $$ = linkFunction($2); }

FunctionDecl: Function LPAREN_SYMBOL FormalParameters RPAREN_SYMBOL COLON_SYMBOL Type SCOLON_SYMBOL FORWARD_SYMBOL SCOLON_SYMBOL 	{ addFunction($1, $3, $6, -1); }
	| Function LPAREN_SYMBOL FormalParameters RPAREN_SYMBOL COLON_SYMBOL Type SCOLON_SYMBOL Body SCOLON_SYMBOL	{ addFunction($1, $3, $6, $8); }
	;

OptVarRef: VAR_SYMBOL { $$ = 0; }
	| REF_SYMBOL { $$ = 1;}
	| { $$ = 0; }
	; 

Body: OptConstDecl OptTypeDecl OptVarDecls Block { $$ = $4; }
	;

Block: BEGIN_SYMBOL StatementSequence END_SYMBOL { $$ = $2; }

OptTypeDecl: TYPE_SYMBOL TypeDecls
	|
	;

TypeDecls: TypeDecl
	| TypeDecls TypeDecl
	;

TypeDecl: IDENT_SYMBOL EQUAL_SYMBOL Type SCOLON_SYMBOL { addType($1, $3); }
	;

Type: SimpleType { $$ = $1; }
	| RecordType { $$ = $1; }
	| ArrayType { $$ = $1; }
	;

SimpleType: IDENT_SYMBOL { $$ = lookupType($1); }
	;

RecordType: RECORD_SYMBOL RecordDecls END_SYMBOL { $$ = addRecordType($2); }
	;

RecordDecls: 					{ $$ = -1; }
	| RecordDecls RecordDecl 	{ $$ = stackRecords($1,$2);}
	;

RecordDecl: IdentList COLON_SYMBOL Type SCOLON_SYMBOL { $$ = addRecords($1, $3); }
	;

ArrayType: ARRAY_SYMBOL LBRACKET_SYMBOL Expression COLON_SYMBOL Expression RBRACKET_SYMBOL OF_SYMBOL Type { $$ = addArrayType($3, $5, $8); }
	;

IdentList: IDENT_SYMBOL						{ $$ = stackIdentList(-1, $1); }
	| IdentList COMMA_SYMBOL IDENT_SYMBOL 	{ $$ = stackIdentList($1, $3); }
	;

OptVarDecls:
	| VAR_SYMBOL VarDecls
	;

VarDecls: VarDecl
	| VarDecls VarDecl
	;

VarDecl: IdentList COLON_SYMBOL Type SCOLON_SYMBOL { addVar($1, $3); }
	;

StatementSequence: Statement 					{ $$ = StackStatementSequence(-1, $1); }
	| StatementSequence SCOLON_SYMBOL Statement	{ $$ = StackStatementSequence($1,$3); }
	;

Statement: Assignment		{ $$ = $1; }
	| IfStatement			{ $$ = $1; }
	| WhileStatement		{ $$ = $1; }
	| RepeatStatement		{ $$ = $1; }
	| ForStatement			{ $$ = $1; }
	| StopStatement			{ $$ = $1; }
	| ReturnStatement		{ $$ = $1; }
	| ReadStatement			{ $$ = $1; }
	| WriteStatement		{ $$ = $1; }
	| ProcedureCall			{ $$ = $1; }
	|						{ $$ = -1; }
	;

Assignment: LValue ASSIGN_SYMBOL Expression { $$ = AssignStmt($1, $3); }
	;

IfStatement: IF_SYMBOL Expression THEN_SYMBOL StatementSequence ElseIf OptElse END_SYMBOL	{ $$ = IfStmt($2,$4,$5, $6); }
	;


ElseIf: ELSEIF_SYMBOL Expression THEN_SYMBOL StatementSequence ElseIf	{ $$ = StackElif($5, $2, $4);}
	|																	{ $$ = -1; }
	;

OptElse: 							{ $$ = -1; }
	| ELSE_SYMBOL StatementSequence { $$ = $2; }
	;

WhileStatement: WHILE_SYMBOL Expression DO_SYMBOL StatementSequence END_SYMBOL	{ $$ = WhileStmt($2, $4); }
	;

RepeatStatement: REPEAT_SYMBOL StatementSequence UNTIL_SYMBOL Expression { $$ = RepeatStmt($2, $4); }
	;

ForStatement: FOR_SYMBOL IDENT_SYMBOL { ForAddToST($2); } ASSIGN_SYMBOL Expression ToDownTo Expression DO_SYMBOL StatementSequence END_SYMBOL { $$ = ForStmt($2, $5, $6, $7, $9); }
	;

ToDownTo: TO_SYMBOL	{ $$ = 0; }
	| DOWNTO_SYMBOL	{ $$ = 1; }
	;

StopStatement: STOP_SYMBOL	{ $$ = StopStmt(); }
	;

ReturnStatement: RETURN_SYMBOL	{ $$ = ReturnStmt(-1); }
	| RETURN_SYMBOL Expression	{ $$ = ReturnStmt($2); }
	;

ReadStatement: READ_SYMBOL LPAREN_SYMBOL LValues RPAREN_SYMBOL	{	$$ = ReadStmt($3); }
	;

LValues: LValues COMMA_SYMBOL LValue	{ $$ = StackLVal($1, $3); 	}
	| LValue							{ $$ = StackLVal(-1, $1); }
	;

WriteStatement: WRITE_SYMBOL LPAREN_SYMBOL WriteArgs RPAREN_SYMBOL	{ $$ = WriteStmt($3); }
	;

WriteArgs: WriteArgs COMMA_SYMBOL Expression	{ $$ = StackWriteArgs($1, $3); }
	| Expression								{ $$ = StackWriteArgs(-1, $1); }
	;

Arguments: Arguments COMMA_SYMBOL Expression 	{ $$ = StackArguments($1, $3); }
	| Expression								{ $$ = StackArguments(-1, $1); }
	;

ProcedureCall: IDENT_SYMBOL LPAREN_SYMBOL OptArguments RPAREN_SYMBOL	{ $$ = ProcCall($1, $3); }
	;

OptArguments: 	{$$ = -1;}
	| Arguments 	{$$ = $1;}
	;

Expression: Expression OR_SYMBOL Expression	{$$=OrExpr($1,$3);}
	| Expression AND_SYMBOL Expression	{$$=AndExpr($1,$3);}
	| Expression EQUAL_SYMBOL Expression	{$$=EQExpr($1,$3);}
	| Expression NOTEQUAL_SYMBOL Expression	{$$=NEExpr($1,$3);}
	| Expression LTE_SYMBOL Expression	{$$= LTEExpr($1,$3);}	
	| Expression GTE_SYMBOL Expression	{ $$ = GTEExpr($1,$3);}
	| Expression LT_SYMBOL Expression	{ $$ = LTExpr($1,$3);}
	| Expression GT_SYMBOL Expression	{ $$ = GTExpr($1,$3); }
	| Expression MINUS_SYMBOL Expression	{ $$ = MinExpr($1,$3); }
	| Expression PLUS_SYMBOL Expression	{ $$ = PlusExpr($1,$3); }
	| Expression MULT_SYMBOL Expression	{ $$ = MultExpr($1,$3); }
	| Expression DIV_SYMBOL Expression	{ $$ = DivExpr($1,$3); }
	| Expression MODULO_SYMBOL Expression	{ $$ = ModExpr($1,$3); }
	| NOT_SYMBOL Expression			{ $$ = NotExpr($2); }
	| MINUS_SYMBOL Expression %prec UNARYMINUS_SYMBOL		{ $$ = UMinusExpr($2); }
	| LPAREN_SYMBOL Expression RPAREN_SYMBOL { $$ = $2; }
	| IDENT_SYMBOL LPAREN_SYMBOL OptArguments RPAREN_SYMBOL	{ $$ = CallFunction($1, $3); }
	| CHR_SYMBOL LPAREN_SYMBOL Expression RPAREN_SYMBOL		{ $$ = ChrExpr($3); } //int to chr
	| ORD_SYMBOL LPAREN_SYMBOL Expression RPAREN_SYMBOL		{ $$ = OrdExpr($3); } //chr to int
	| PRED_SYMBOL LPAREN_SYMBOL Expression RPAREN_SYMBOL		{$$ = PredExpr($3); } //decrement
	| SUCC_SYMBOL LPAREN_SYMBOL Expression RPAREN_SYMBOL		{$$ = SuccExpr($3); } //increment
	| LValue					{ $$ = LValueExpr($1); }
	| CHRCONST_SYMBOL			{ $$ = CharExpr($1); }
	| INT_SYMBOL				{ $$ = IntExpr($1); }
	| STRING_SYMBOL				{ $$ = StringExpr($1); }
	;

LValue: IDENT_SYMBOL				{ $$ = LValID($1);}
	| LValue DOT_SYMBOL IDENT_SYMBOL						{ $$ = LValMemberAccess($1, $3); }
	| LValue LBRACKET_SYMBOL Expression RBRACKET_SYMBOL		{ $$ = LValArrayAccess($1, $3); }
	;

%%



void yyerror(const char* msg)
{
	std::cerr<< lineNum <<msg << std::endl;
}





