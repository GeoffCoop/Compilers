#ifndef FRONTEND_HPP
#define FRONTEND_HPP

#include<string>
namespace FE
{
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
};
#endif
