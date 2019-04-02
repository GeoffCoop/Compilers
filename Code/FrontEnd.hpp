#ifndef FRONTEND_HPP
#define FRONTEND_HPP

#include <string>

	std::string printStringTable();
	std::string initMIPS();
	void emitMIPS(int);
	void push_ST();
	void pop_ST();
	
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
	int StackLVal(int, int);

	int NewArgument(int);
	int StackArguments(int, int);

	int AssignStmt(int, int);
	int IfStmt(int, int, int, int);
	int StackElif(int, int, int);
	int WhileStmt(int, int);
	int RepeatStmt(int, int);
	void ForAddToST(char*);
	int ForStmt(char*, int, bool, int, int);
	int StopStmt();
	int ReturnStmt(int);
	int ReadStmt(int);
	int WriteStmt(int);
	int StackWriteArgs(int, int);
	int ProcCall(char*, int);
	int StackStatementSequence(int, int);
	
	int stackIdentList(int, char*);	
	int lookupType(char*);
	int addVar(int, int);
	int addConst(char*, int);
	int addType(char*, int);
	int addArrayType(int, int, int );
	int addRecordType(int);
	int addRecords(int, int);
	int stackRecords(int,int);
	int addParameter(int, int, int);
	int stackParameters(int, int);
	void addProcedure(char*, int, int);
	char* linkProcedure(char*);
	char* linkFunction(char*);
	void addFunction(char*, int, int, int);
#endif
