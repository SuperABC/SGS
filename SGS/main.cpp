#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include "machine.h"

//#define SGS_COMPILE

using std::cin;
using std::cout;
using std::endl;
using namespace sgs;

SgsLex l = SgsLex();
SgsSyntax s = SgsSyntax();
SgsMachine m = SgsMachine();

void testTool(vector<sgs::AST *> stmts)
{
	unsigned int loopNum;
	for (loopNum = 0; loopNum < stmts.size(); ++loopNum)
	{
		std::cout << "No." << loopNum << " statement is parsed" << std::endl;
		std::cout << "--------------result---------------" << std::endl;
		switch (stmts[loopNum]->astType)
		{
		case AT_TYPEDEF:
		{
			sgs::TypeDef *currentStmt = (sgs::TypeDef *)stmts[loopNum];
			std::cout << "astType: AT_TYPEDEF" << std::endl;
			switch (currentStmt->getDecType()->getVarType())
			{
			case 0: std::cout << "decType: VT_BASIC" << std::endl; break;
			case 1: std::cout << "decType: VT_ARRAY" << std::endl; break;
			case 2: std::cout << "decType: VT_CLASS" << std::endl; break;
			default:break;
			}
			std::cout << "name:    " + currentStmt->getName() << std::endl;
			std::cout << std::endl;
			break;
		}
		case AT_CLASS:
		{
			sgs::ClassDef *currentStmt = (sgs::ClassDef *)stmts[loopNum];
			std::cout << "astType: AT_CLASS" << std::endl;
			switch (currentStmt->getDecType()->getVarType())
			{
			case 0: std::cout << "decType: VT_BASIC" << std::endl; break;
			case 1: std::cout << "decType: VT_ARRAY" << std::endl; break;
			case 2: std::cout << "decType: VT_CLASS" << std::endl; break;
			default:break;
			}
			std::cout << "className: " + currentStmt->getDecType()->getName() << std::endl;
			std::cout << "eleList" << std::endl;
			//std::cout << currentStmt->getDecType()->getEle() << std::endl;
			std::cout << std::endl;
			break;
		}
		case AT_EXP:
		{
			sgs::Expression *currentStmt = (sgs::Expression *)stmts[loopNum];
			std::cout << "astType: AT_EXP" << std::endl;
			switch (currentStmt->getExpType())
			{
			case 0: std::cout << "expType: ET_OP" << std::endl; break;
			case 1: std::cout << "expType: ET_LITERAL" << std::endl; break;
			case 2: std::cout << "expType: ET_IDENT" << std::endl; break;
			case 3: std::cout << "expType: ET_VISIT" << std::endl; break;
			case 4: std::cout << "expType: ET_CALL" << std::endl; break;
			case 5: std::cout << "expType: ET_ACCESS" << std::endl; break;
			default:break;
			}
			switch (currentStmt->getResType()->getVarType())
			{
			case 0: std::cout << "decType: VT_BASIC" << std::endl; break;
			case 1: std::cout << "decType: VT_ARRAY" << std::endl; break;
			case 2: std::cout << "decType: VT_CLASS" << std::endl; break;
			default:break;
			}
			std::cout << std::endl;
			break;
		}
		case AT_STMT:
		{
			sgs::Statement *currentStmt = (sgs::Statement *)stmts[loopNum];
			std::cout << "astType: AT_STMT" << std::endl;
			switch (currentStmt->getStmtType())
			{
			case 0: std::cout << "stmtType: ST_ASSIGN" << std::endl; break;
			case 1: std::cout << "stmtType: ST_CALL" << std::endl; break;
			case 2: std::cout << "stmtType: ST_IF" << std::endl; break;
			case 3: std::cout << "stmtType: ST_WHILE" << std::endl; break;
			case 4: std::cout << "stmtType: ST_RETURN" << std::endl; break;
			case 5: std::cout << "stmtType: ST_BREAK" << std::endl; break;
			case 6: std::cout << "stmtType: ST_CONTINUE" << std::endl; break;
			case 7: std::cout << "stmtType: ST_BLOCK" << std::endl; break;
			default:break;
			}
			std::cout << std::endl;
			break;
		}
		case AT_FUNC:
		{
			sgs::FuncDef *currentStmt = (sgs::FuncDef *)stmts[loopNum];
			std::cout << "astType: AT_FUNC" << std::endl;
			std::cout << "USE VS To CHECK THE VALUE OF paramList" << std::endl;
			std::cout << "USE VS To CHECK THE VALUE OF body" << std::endl;
			switch (currentStmt->getProto()->getReturnType()->getVarType())
			{
			case 0: std::cout << "returnType: VT_BASIC" << std::endl; break;
			case 1: std::cout << "returnType: VT_ARRAY" << std::endl; break;
			case 2: std::cout << "returnType: VT_CLASS" << std::endl; break;
			default:break;
			}
			std::cout << "name:    " + currentStmt->getProto()->getName() << std::endl;
			std::cout << std::endl;
			break;
		}
		case AT_PROTO:
			break;
		default:

			break;
		}
	}
	return;
}
void main() {
	std::ifstream fin("test.sgs");

	string input, tmp;
	while (!fin.eof()) {
		getline(fin, tmp);
		input += tmp + '\n';
	}

	l.input(input.c_str())->parse();
	s.input(l.strId, l.output)->parse();
#ifndef SGS_COMPILE
	bool success = true;
	if (s.msgList.size()) {
		for (auto msg : s.msgList) {
			std::cout << msg.getMsg();
			if (msg.getLevel() == MT_ERROR)success = false;
		}
	}
	if (success) {
		m.input(s.stmts, s.classList, s.funcList)->execute();
		for (auto msg : m.msgList)std::cout << msg.getMsg();
	}
#else
	testTool(s.stmts);
#endif

	system("pause");
}