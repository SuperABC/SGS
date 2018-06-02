#pragma once
#include "syntax.h"
#include <iostream>

using namespace sgs;

class Tab {
private:
	int num;
public:
	Tab(int n) : num(n) {}
	friend std::ostream& operator<< (std::ostream& os, Tab t) {
		for (int i = 0; i < t.num; ++i)
			os << "  |";
		return os;
	}
};

enum conditionUseVarType {
	TYPEDEF,
	CLASS,
	EXP,
	FUNC,
	PROTO
};
void printBasicType(sgs::VarType *stmtVar);
void printArrayType(sgs::VarType *stmtVar);
void printClassType(sgs::VarType *stmtVar);
void opSwitchCase(SGSOPERATOR OP);
void printOpExp(sgs::Expression *stmtExp);
void printLiteralExp(sgs::Expression *stmtExp);
void printIdExp(sgs::Expression *stmtExp);
void printVisitExp(sgs::Expression *stmtExp);
void printCallExp(sgs::Expression *stmtExp);
void printAccessExp(sgs::Expression *stmtExp);
void printAssignStmt(sgs::Statement *stmtStmt);
void printCallStmt(sgs::Statement *stmtStmt);
void printBlockStmt(sgs::Statement *stmtStmt);
void printIfStmt(sgs::Statement *stmtStmt);
void printWhileStmt(sgs::Statement *stmtStmt);
void dealWithVarType(sgs::AST *s, enum conditionUseVarType choice);
void dealWithExpType(sgs::AST *s);
void dealWithStmtType(sgs::AST *s);
void dealWithFuncDefType(sgs::AST *s);
void dealWithFuncProtoType(sgs::AST *s);
void printAST(vector<sgs::AST *> stmts);