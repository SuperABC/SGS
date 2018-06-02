#define _CRT_SECURE_NO_WARNINGS
#include "machine.h"
#include "printAST.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#define SGS_INTERPRETER_OFF

using std::cin;
using std::cout;
using std::endl;
using std::string;
using namespace sgs;

SgsLex l = SgsLex();
SgsSyntax s = SgsSyntax();
SgsMachine m = SgsMachine();
string removeSpace(string input);
void translateBasicType(sgs::VarType *stmtVar, std::ofstream &fout);
VAR_TYPE translateArrayType(sgs::VarType *stmtVar, std::ofstream &fout);
void translateVarType(sgs::AST *s, enum conditionUseVarType choice, std::ofstream &fout);
void translateExpType(sgs::AST *s, std::ofstream &fout);
void translateStmtType(sgs::AST *s, std::ofstream &fout);
void translateFuncDefType(sgs::AST *s, std::ofstream &fout);
void translateFuncProtoType(sgs::AST *s, std::ofstream &fout);
void translateAST(vector<sgs::AST *>stmts, std::ofstream &fout);
void translateToCPP(vector<sgs::AST *> stmts);
string removeSpace(string input)
{
	int pos = 0;
	string output = input;
	pos = output.find(" ", pos);
	while (pos != -1)
	{
		output.replace(pos, 1, "_");
		pos = output.find(" ", pos);
	}
	return output;
}
void abc(int a[])
{
	
}
void translateBasicType(sgs::VarType *stmtVar, std::ofstream &fout)
{
	sgs::BasicType *basicVar = (sgs::BasicType *)stmtVar;
	switch (basicVar->getBasicType())
	{
	case BASIC_TYPE::BT_INT:    fout << "int " ; break;
	case BASIC_TYPE::BT_FLOAT:  fout << "float "; break;
	case BASIC_TYPE::BT_BOOL:   fout << "bool "; break;
	case BASIC_TYPE::BT_CHAR:   fout << "char "; break;
	case BASIC_TYPE::BT_STRING: fout << "string "; break;
	default:
		break;
	}
	return;
}
VAR_TYPE translateArrayType(sgs::VarType *stmtVar, std::ofstream &fout)
{
	sgs::ArrayType *arrayVar = (sgs::ArrayType *)stmtVar;
	switch (arrayVar->getEleType()->getVarType())
	{
	case VAR_TYPE::VT_BASIC: 
	{
		translateBasicType(arrayVar->getEleType(), fout);
		break;
	}
	case VAR_TYPE::VT_ARRAY: 
	{
		translateBasicType((sgs::ArrayType *)(arrayVar->getEleType())->getVarType(), fout);
		return VAR_TYPE::VT_ARRAY;
	}
	case VAR_TYPE::VT_CLASS: 
	{
		return VAR_TYPE::VT_CLASS;
	}
	default:break;
	}
	return VAR_TYPE::VT_BASIC;
}
void translateVarType(sgs::AST *s, enum conditionUseVarType choice, std::ofstream &fout)
{
	switch (choice)
	{
	case conditionUseVarType::TYPEDEF: //TYPEDEF
	{
		sgs::TypeDef *currentStmt = (sgs::TypeDef *)s;
		std::cout << "newVarName: " + currentStmt->getName() << std::endl;
		switch (currentStmt->getDecType()->getVarType())
		{
		case VAR_TYPE::VT_BASIC: printBasicType(currentStmt->getDecType()); break;
		case VAR_TYPE::VT_ARRAY: printArrayType(currentStmt->getDecType()); break;
		case VAR_TYPE::VT_CLASS: printClassType(currentStmt->getDecType()); break;
		default:break;
		}
		break;
	}
	case conditionUseVarType::CLASS:
	{
		sgs::ClassDef *currentStmt = (sgs::ClassDef *)s;
		std::cout<< "className: " + currentStmt->getDecType()->getName() << std::endl;
		printClassType(currentStmt->getDecType());
		break;
	}
	case conditionUseVarType::EXP:
	{
		sgs::Expression *currentStmt = (sgs::Expression *)s;
		switch (currentStmt->getResType()->getVarType())
		{
		case VAR_TYPE::VT_BASIC: printBasicType(currentStmt->getResType()); break;
		case VAR_TYPE::VT_ARRAY: printArrayType(currentStmt->getResType()); break;
		case VAR_TYPE::VT_CLASS: printClassType(currentStmt->getResType()); break;
		default:break;
		}
		break;
	}
	case conditionUseVarType::FUNC:
	{
		sgs::FuncDef *currentStmt = (sgs::FuncDef *)s;
		break;
	}
	case conditionUseVarType::PROTO:
	{
		sgs::FuncProto *currentStmt = (sgs::FuncProto *)s;
		if (NULL == currentStmt->getReturnType())
		{
			fout << "void ";
			break;
		}
		switch (currentStmt->getReturnType()->getVarType())
		{
		case VAR_TYPE::VT_BASIC: translateBasicType(currentStmt->getReturnType(), fout); break;
		case VAR_TYPE::VT_ARRAY: translateArrayType(currentStmt->getReturnType(), fout); break;
		case VAR_TYPE::VT_CLASS: fout << (((sgs::ClassDef *)currentStmt)->getDecType())->getName(); break;
		default:break;
		}
		break;
	}
	default:
		break;
	}

}
void translateExpType(sgs::AST *s, std::ofstream &fout)
{/*
	sgs::Expression *currentStmt = (sgs::Expression *)s;
	switch (currentStmt->getExpType())
	{
	case EXP_TYPE::ET_OP:
	{
		std::cout << Tab(depth) << "expType: ET_OP" << std::endl;
		depth++;
		printOpExp(currentStmt);
		depth--;
		break;
	}
	case EXP_TYPE::ET_LITERAL:
	{
		std::cout << Tab(depth) << "expType: ET_LITERAL" << std::endl;
		depth++;
		printLiteralExp(currentStmt);
		depth--;
		break;
	}
	case EXP_TYPE::ET_IDENT:
	{
		std::cout << Tab(depth) << "expType: ET_IDENT" << std::endl;
		depth++;
		printIdExp(currentStmt);
		depth--;
		break;
	}
	case EXP_TYPE::ET_VISIT:
	{
		std::cout << Tab(depth) << "expType: ET_VISIT" << std::endl;
		depth++;
		printVisitExp(currentStmt);
		depth--;
		break;
	}
	case EXP_TYPE::ET_CALL:
	{
		std::cout << Tab(depth) << "expType: ET_CALL" << std::endl;
		depth++;
		printCallExp(currentStmt);
		depth--;
		break;
	}
	case EXP_TYPE::ET_ACCESS:
	{
		std::cout << Tab(depth) << "expType: ET_ACCESS" << std::endl;
		depth++;
		printAccessExp(currentStmt);
		depth--;
		break;
	}
	default:break;
	}*/
}
void translateStmtType(sgs::AST *s, std::ofstream &fout)
{/*
	sgs::Statement *currentStmt = (sgs::Statement *)s;
	switch (currentStmt->getStmtType())
	{
	case STMT_TYPE::ST_ASSIGN:
	{
		std::cout << Tab(depth) << "stmtType: ST_ASSIGN" << std::endl;
		depth++;
		printAssignStmt(currentStmt);
		depth--;
		break;
	}
	case STMT_TYPE::ST_CALL:
	{
		std::cout << Tab(depth) << "stmtType: ST_CALL" << std::endl;
		depth++;
		printCallStmt(currentStmt);
		depth--;
		break;
	}
	case STMT_TYPE::ST_IF:
	{
		std::cout << Tab(depth) << "stmtType: ST_IF" << std::endl;
		depth++;
		printIfStmt(currentStmt);
		depth--;
		break;
	}
	case STMT_TYPE::ST_WHILE:
	{
		std::cout << Tab(depth) << "stmtType: ST_WHILE" << std::endl;
		depth++;
		printWhileStmt(currentStmt);
		depth--;
		break;
	}
	case STMT_TYPE::ST_RETURN: std::cout << Tab(depth) << "stmtType: ST_RETURN" << std::endl; break;
	case STMT_TYPE::ST_BREAK: std::cout << Tab(depth) << "stmtType: ST_BREAK" << std::endl; break;
	case STMT_TYPE::ST_CONTINUE: std::cout << Tab(depth) << "stmtType: ST_CONTINUE" << std::endl; break;
	case STMT_TYPE::ST_BLOCK:
	{
		std::cout << Tab(depth) << "stmtType: ST_BLOCK" << std::endl;
		depth++;
		printAssignStmt(currentStmt);
		depth--;
		break;
	}
	default:break;
	}*/
}
void translateFuncDefType(sgs::AST *s, std::ofstream &fout)
{/*
	sgs::FuncDef *currentStmt = (sgs::FuncDef *)s;
	std::cout << Tab(depth) << "proto:" << std::endl;
	depth++;
	dealWithFuncProtoType(currentStmt->getProto());
	depth--;
	std::cout << Tab(depth) << "body:" << std::endl;
	depth++;
	printBlockStmt(currentStmt->getBody());
	depth--;*/
}
void translateFuncProtoType(sgs::AST *s, std::ofstream &fout)
{
	sgs::FuncProto *currentStmt = (sgs::FuncProto *)s;
	translateVarType(currentStmt, conditionUseVarType::PROTO, fout);
	fout << removeSpace(currentStmt->getName()) << '(';
	int count = currentStmt->getParam().size();
	vector <std::pair<VarType *, string>> paramList = currentStmt->getParam();
	if (paramList.size() == 0)
	{
		fout << ");";
	}
	for (int i = 0; i < count; ++i)
	{
		sgs::VarType *currentParameter = paramList[i].first;
		string currentName = paramList[i].second;
		switch (currentParameter->getVarType())
		{
		case VAR_TYPE::VT_BASIC: 
		{
			translateBasicType(currentParameter, fout);
			fout << currentName;
			break;
		}
		case VAR_TYPE::VT_ARRAY: 
		{ 
			VAR_TYPE type = translateArrayType(currentParameter, fout);
			if (VAR_TYPE::VT_BASIC == type)
			{
				fout << currentName << "[]";
			}
			else if (VAR_TYPE::VT_ARRAY == type)
			{
				fout << currentName << "[][]";
			}
			else if (VAR_TYPE::VT_CLASS == type)
			{
				fout << ((sgs::ClassType *)currentParameter)->getName() << ' ';
				fout << currentName << "[]";
			}
		}
		case VAR_TYPE::VT_CLASS: 
		{
			fout << ((sgs::ClassType *)currentParameter)->getName() << ' ';
			break;
		}
		default:break;
		}
		if (i == count - 1)
			fout << ");";
		else
			fout << ", ";
	}
	
}
void translateAST(vector<sgs::AST *>stmts, std::ofstream &fout)
{
	unsigned int loopNum;
	for (loopNum = 0; loopNum < stmts.size(); ++loopNum)
	{
		switch (stmts[loopNum]->astType)
		{
		case AT_TYPEDEF:
		{
			sgs::TypeDef *currentStmt = (sgs::TypeDef *)stmts[loopNum];
			translateVarType(currentStmt, TYPEDEF, fout);
			break;
		}
		case AT_CLASS:
		{
			sgs::ClassDef *currentStmt = (sgs::ClassDef *)stmts[loopNum];
			translateVarType(currentStmt, CLASS, fout);
			std::cout << std::endl;
			break;
		}
		case AT_EXP:
		{
			sgs::Expression *currentStmt = (sgs::Expression *)stmts[loopNum];
			translateExpType(currentStmt, fout);
			translateVarType(currentStmt, EXP, fout);
			break;
		}
		case AT_STMT:
		{
			sgs::Statement *currentStmt = (sgs::Statement *)stmts[loopNum];
			translateStmtType(currentStmt, fout);
			break;
		}
		case AT_FUNC:
		{
			sgs::FuncDef *currentStmt = (sgs::FuncDef *)stmts[loopNum];
			translateFuncDefType(currentStmt, fout);
			break;
		}
		case AT_PROTO:
		{
			sgs::FuncProto *currentStmt = (sgs::FuncProto *)stmts[loopNum];
			translateFuncProtoType(currentStmt, fout);
			break;
		}
		default:
			break;
		}
		fout << std::endl;
	}
	return;
}
void translateToCPP(vector<sgs::AST *> stmts)
{
	std::ofstream fout("output.cpp");
	translateAST(stmts, fout);
	fout.close();
}

int main() 
{
	std::ifstream fin(string("test.sgs"));
	string input, tmp;
	while (!fin.eof()) {
		getline(fin, tmp);
		input += tmp + '\n';
	}
	fin.close();
	l.input(input.c_str())->parse();
	s.input(l.strId, l.output)->parse();
#ifndef SGS_INTERPRETER_OFF
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
	printAST(s.stmts);
	translateToCPP(s.stmts);
#endif
	// system((char *)"pause");
    string temp; 
	cin >> temp;
    return 0;
}