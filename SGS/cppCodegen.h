#pragma once

#include "lexeme.h"
#include "machine.h"
#include <fstream>
#include "printAST.h"
#include "syntax.h"

// using namespace sgs;

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

string removeSpace(const string& input);
void insertBuiltinFunction(std::ofstream &fout);
void translateBasicType(sgs::VarType *stmtVar, std::ofstream &fout);
sgs::VAR_TYPE translateArrayType(sgs::VarType *stmtVar, std::ofstream &fout);
void opSwitchCase(SGSOPERATOR OP, std::ofstream &fout);
void translateOpExp(sgs::Expression *stmtExp, std::ofstream &fout);
void translateLiteralExp(sgs::Expression *stmtExp, std::ofstream &fout);
void translateIdExp(sgs::Expression *stmtExp, std::ofstream &fout);
void translateVisitExp(sgs::Expression *stmtExp, std::ofstream &fout);
void translateCallExp(sgs::Expression *stmtExp, std::ofstream &fout);
void translateAccessExp(sgs::Expression *stmtExp, std::ofstream &fout);
void translateAssignStmt(sgs::Statement *stmtStmt, std::ofstream &fout);
void translateCallStmt(sgs::Statement *stmtStmt, std::ofstream &fout);
void translateBlockStmt(sgs::Statement *stmtStmt, std::ofstream &fout);
void translateIfStmt(sgs::Statement *stmtStmt, std::ofstream &fout);
void translateWhileStmt(sgs::Statement *stmtStmt, std::ofstream &fout);
void translateVarType(sgs::AST *s, enum conditionUseVarType choice, std::ofstream &fout);
void translateExpType(sgs::AST *s, std::ofstream &fout);
void translateStmtType(sgs::AST *s, std::ofstream &fout);
void translateFuncDefType(sgs::AST *s, std::ofstream &fout);
void translateFuncProtoType(sgs::AST *s, std::ofstream &fout);
void translateAST(vector<sgs::AST *>stmts, std::ofstream &fout);
void translateToCPP(vector<sgs::AST *> stmts, const std::string& filename);