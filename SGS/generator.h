#ifndef GENERATOR_H
#define GENERATOR_H
#include <iostream>
#include "syntax.h"

using std::string;
using namespace sgs;

class cppTab {
private:
	int num;
public:
	cppTab(int n) : num(n) {}
	friend std::ostream& operator<< (std::ostream& os, cppTab t) {
		for (int i = 0; i < t.num; ++i)
			os << "    ";
		return os;
	}
};
static int cppDepth = 0;

void generateCpp(vector<sgs::AST *> stmts,
	vector<sgs::ClassType *>classList, string file);

string removeSpace(const string& input);

void translateBasicType(sgs::VarType *s, std::ofstream &fout);
void translateVarType(sgs::VarType *t, std::ofstream &fout);
void translateOpExp(sgs::Expression *s, std::ofstream &fout);
void translateLiteralExp(sgs::Expression *s, std::ofstream &fout);
void translateIdExp(sgs::Expression *s, std::ofstream &fout);
void translateCallExp(sgs::Expression *s, std::ofstream &fout);
void translateVisitExp(sgs::Expression *s, std::ofstream &fout);
void translateAccessExp(sgs::Expression *s, std::ofstream &fout);
void translateExpType(sgs::AST *s, std::ofstream &fout);

void translateAssignStmt(sgs::Statement *s, std::ofstream &fout);
void translateCallStmt(sgs::Statement *s, std::ofstream &fout);
void translateBlockStmt(sgs::Statement *s, std::ofstream &fout);
void translateIfStmt(sgs::Statement *s, std::ofstream &fout);
void translateWhileStmt(sgs::Statement *s, std::ofstream &fout);

void translateVarDec(sgs::AST *s, std::ofstream &fout);
void translateClassDec(sgs::AST *s, std::ofstream &fout);
void translateFuncDec(sgs::AST *s, std::ofstream &fout);
void translateFuncDef(sgs::AST *s, std::ofstream &fout);
void translateStmtExe(sgs::AST *s, std::ofstream &fout);

#endif

