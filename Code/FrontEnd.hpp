#ifndef FRONTEND_HPP
#define FRONTEND_HPP

#include <string>

	std::string printStringTable();
	std::string initMIPS();
	void emitMIPS();
	
	int StringExpr(char* x);
	int IntExpr(int x);
	int CharExpr(char x);
	int SuccExpr(int x);
	int PredExpr(int x);
	int ChrExpr(int x);
	int OrdExpr(int x);
	int CallFunction(char* x, int y);
	int UMinusExpr(int x);
	int NotExpr(int x);
	int ModExpr(int x, int y);
	int DivExpr(int x, int y);
	int MultExpr(int x, int y);
	int MinExpr(int x, int y);
	int GTExpr(int x, int y);
	int PlusExpr(int x, int y);
	int LTExpr(int x, int y);
	int GTEExpr(int x, int y);
	int LTEExpr(int x, int y);
	int NEExpr(int x, int y);
	int EQExpr(int x, int y);
	int AndExpr(int x, int y);
	int OrExpr(int x, int y);
	int LValueExpr(int x);

	int LValID(char*);
	int LValMemberAccess(int, char*);
	int LValArrayAccess(int, int);
	int NewLValList(int);
	int StackLVal(int, int);

	int NewArgument(int);
	int StackArgument(int, int);

	int AssignStmt(int, int);
	int MergeConditional(int, int);
	int IfStmt(int, int, int);
	int StackElif(int, int);
	int WhileStmt(int, int);
	int RepeatStmt(int, int);
	int ForStmt(char*, int, bool, int, int);
	int StopStmt();
	int ReturnStmt(int);
	int ReadStmt(int);
	int WriteStmt(int);
	int StackWriteArgs(int, int);
	int ProcCall(char*, int);
	int NewStatementSequence(int);
	int StackStatementSequence(int, int);
	
	int stackIdentList(int, char*);	
	char* lookupType(char*);
	int addVar(int, char*);
#endif
