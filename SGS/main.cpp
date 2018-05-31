#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include "machine.h"

#define SGS_INTERPRETER

using std::cin;
using std::cout;
using std::endl;
using namespace sgs;

SgsLex l = SgsLex();
SgsSyntax s = SgsSyntax();
SgsMachine m = SgsMachine();
void printBasicType(sgs::VarType *stmtVar);
void printArrayType(sgs::VarType *stmtVar);
void printClassType(sgs::VarType *stmtVar);
void dealWithExpType(sgs::AST *s);
void printBasicType(sgs::VarType *stmtVar)
{
	sgs::BasicType *basicVar = (sgs::BasicType *)stmtVar;
	switch (basicVar->getBasicType())
	{
	case BASIC_TYPE::BT_INT:    std::cout << "basicType: BT_INT" << std::endl; break;
	case BASIC_TYPE::BT_FLOAT:  std::cout << "basicType: BT_FLOAT" << std::endl; break;
	case BASIC_TYPE::BT_BOOL:   std::cout << "basicType: BT_BOOL" << std::endl; break;
	case BASIC_TYPE::BT_CHAR:   std::cout << "basicType: BT_CHAR" << std::endl; break;
	case BASIC_TYPE::BT_STRING: std::cout << "basicType: BT_STRING" << std::endl; break;
	default:
		break;
	}
	return;
}
void printArrayType(sgs::VarType *stmtVar)
{
	sgs::ArrayType *arrayVar = (sgs::ArrayType *)stmtVar;
	std::cout << "	decType: VT_ARRAY" << std::endl;
	std::cout << "	length:  " << arrayVar->getLength() << std::endl;
	std::cout << "	";
	switch (arrayVar->getEleType()->getVarType())
	{
	case VAR_TYPE::VT_BASIC: printBasicType(arrayVar->getEleType()); break;
	case VAR_TYPE::VT_ARRAY: printArrayType(arrayVar->getEleType()); break;
	case VAR_TYPE::VT_CLASS: printClassType(arrayVar->getEleType()); break;
	default:break;
	}
	return;
}
void printClassType(sgs::VarType *stmtVar)
{
	sgs::ClassType *classVar = (sgs::ClassType *)stmtVar;
	std::cout << "	decType: VT_CLASS" << std::endl;
	std::cout << "	className:  " << classVar->getName() << std::endl;
	std::cout << "	classMember: " << std::endl;
	int count = classVar->getEle().size();
	vector <std::pair<VarType *, string>> classElements = classVar->getEle();
	for (int i = 0; i < count; ++i)
	{
		sgs::VarType *currentMember = classElements[i].first;
		std::cout << "	    No." << (i + 1) << " Member:" << std::endl;
		std::cout << "	        memberName:" + classElements[i].second << std::endl;
		std::cout << "		";
		switch (currentMember->getVarType())
		{
		case VAR_TYPE::VT_BASIC: printBasicType(currentMember); break;
		case VAR_TYPE::VT_ARRAY: printArrayType(currentMember); break;
		case VAR_TYPE::VT_CLASS: printClassType(currentMember); break;
		default:break;
		}
	}
	return;
}
void printOpExp(sgs::Expression *stmtExp)
{
	sgs::OpExp *opExp = (sgs::OpExp *)stmtExp;
	std::cout << "opExpOperator:" + opExp->getOp() << std::endl;
	std::cout << "    leftExp:" << std::endl;
	std::cout << "        ";
	dealWithExpType(opExp->getLeft());
	std::cout << "    RightExp:" << std::endl;
	std::cout << "        ";
	dealWithExpType(opExp->getRight());
}
void printLiteralExp(sgs::Expression *stmtExp)
{
	sgs::LiteralExp *literalExp = (sgs::LiteralExp *)stmtExp;
	switch (literalExp->getType()->getVarType())
	{
	case VAR_TYPE::VT_BASIC:
	{
		sgs::BasicType *basicLiteralExp = (sgs::BasicType *)literalExp->getType();
		switch (basicLiteralExp->getBasicType())
		{
		case BASIC_TYPE::BT_INT:
		{
			sgs::IntLiteral *intLiteralExp = (sgs::IntLiteral *)literalExp;
			std::cout << "int value:" << intLiteralExp->getValue() << std::endl;
			break;
		}
		case BASIC_TYPE::BT_FLOAT:
		{
			sgs::FloatLiteral *floatLiteralExp = (sgs::FloatLiteral *)literalExp;
			std::cout << "float value:" << floatLiteralExp->getValue() << std::endl;
			break;
		}
		case BASIC_TYPE::BT_BOOL:
		{
			sgs::BoolLiteral *boolLiteralExp = (sgs::BoolLiteral *)literalExp;
			std::cout << "Bool value:" << boolLiteralExp->getValue() << std::endl;
			break;
		}
		case BASIC_TYPE::BT_STRING:
		{
			sgs::StrLiteral *strLiteralExp = (sgs::StrLiteral *)literalExp;
			std::cout << "String value:" << strLiteralExp->getValue() << std::endl;
			break;
		}
		default:
			break;
		}
		break;
	}
	case VAR_TYPE::VT_ARRAY:
	{
		sgs::ArrayLiteral *arrayLiteralExp = (sgs::ArrayLiteral *)literalExp;
		std::cout << "Array value:"  << std::endl;
		vector<Expression *>arrayConent = arrayLiteralExp->getValue();
		int count = arrayConent.size();
		for (int i = 0; i < count; ++i)
		{
			std::cout << "content " << i << std::endl;
			dealWithExpType(arrayConent[i]);
		}
		break;
	}
	case VAR_TYPE::VT_CLASS:
	{
		sgs::ClassLiteral *classLiteralExp = (sgs::ClassLiteral *)literalExp;
		std::cout << "Class value:" << std::endl;
		vector<Expression *>classConent = classLiteralExp->getValue();
		int count = classConent.size();
		for (int i = 0; i < count; ++i)
		{
			std::cout << "content " << i << std::endl;
			dealWithExpType(classConent[i]);
		}
		break;
	}
	default:
		break;
	}
}
void printIdExp(sgs::Expression *stmtExp)
{
	sgs::IdExp *idExp = (sgs::IdExp *)stmtExp;
	std::cout << "name:" << idExp->getName() << endl;
	return;
}
void printVisitExp(sgs::Expression *stmtExp)
{
	sgs::VisitExp *visitExp = (sgs::VisitExp *)stmtExp;
	std::cout << "array name: " << std::endl;
	dealWithExpType(visitExp->getArray());
	std::cout << "index: " << std::endl;
	dealWithExpType(visitExp->getIndex());
	return;
}
void printCallExp(sgs::Expression *stmtExp)
{

}
void printAccessExp(sgs::Expression *stmtExp)
{
	sgs::AccessExp *accessExp = (sgs::AccessExp *)stmtExp;
	std::cout << "object:" << std::endl;
	dealWithExpType(accessExp->getObject());
	std::cout << "member" << std::endl;
	std::cout << accessExp->getMember() << std::endl;
}
enum conditionUseVarType {
	TYPEDEF,
	CLASS,
	EXP,
	FUNC,
	PROTO
};
void dealWithVarType(sgs::AST *s, enum conditionUseVarType choice)
{
	//first, do type conversion
	switch (choice)
	{
	case conditionUseVarType::TYPEDEF: //TYPEDEF
	{
		sgs::TypeDef *currentStmt = (sgs::TypeDef *)s;
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
		printClassType(currentStmt->getDecType());
		break; 
	}
	case conditionUseVarType::EXP: 
	{
		sgs::Expression *currentStmt = (sgs::Expression *)s;
		//std::cout << "	expRes:" << std::endl;
		switch (currentStmt->getResType()->getVarType())
		{
		case VAR_TYPE::VT_BASIC: printBasicType(currentStmt->getResType()); break;
		case VAR_TYPE::VT_ARRAY: printArrayType(currentStmt->getResType()); break;
		case VAR_TYPE::VT_CLASS: printClassType(currentStmt->getResType()); break;
		default:break;
		}
		break; 
	}
	case 3: {break; }
	case 4: {break; }
	default:
		break;
	}
	
}
void dealWithExpType(sgs::AST *s)
{
	sgs::Expression *currentStmt = (sgs::Expression *)s;
	switch (currentStmt->getExpType())
	{
	case 0: 
		std::cout << "expType: ET_OP" << std::endl; 
		std::cout << "    ";
		printOpExp(currentStmt);
		break;
	case 1: 
		std::cout << "expType: ET_LITERAL" << std::endl;
		std::cout << "    ";
		printLiteralExp(currentStmt);
		break;
	case 2: 
		std::cout << "expType: ET_IDENT" << std::endl;
		std::cout << "    ";
		printIdExp(currentStmt);
		break;
	case 3: 
		std::cout << "expType: ET_VISIT" << std::endl;
		std::cout << "    ";
		printVisitExp(currentStmt);
		break;
	case 4: 
		std::cout << "expType: ET_CALL" << std::endl; 
		break;
	case 5: 
		std::cout << "expType: ET_ACCESS" << std::endl;
		std::cout << "    ";
		printAccessExp(currentStmt);
		break;
	default:break;
	}
}
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
			dealWithVarType(currentStmt, TYPEDEF);
			std::cout << "newVarName: " + currentStmt->getName() << std::endl;
			std::cout << std::endl;
			break;
		}
		case AT_CLASS:
		{
			sgs::ClassDef *currentStmt = (sgs::ClassDef *)stmts[loopNum];
			std::cout << "astType: AT_CLASS" << std::endl;
			dealWithVarType(currentStmt, CLASS);
			std::cout << "className: " + currentStmt->getDecType()->getName() << std::endl;
			std::cout << std::endl;
			break;
		}
		case AT_EXP:
		{
			sgs::Expression *currentStmt = (sgs::Expression *)stmts[loopNum];
			std::cout << "astType: AT_EXP" << std::endl;
			dealWithExpType(currentStmt);
			dealWithVarType(currentStmt, EXP);
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
#ifndef SGS_INTERPRETER
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
	//(sgs::IdExp *)(((sgs::AssignStmt*)s.stmts[5])->getLeft());
	dealWithExpType(((sgs::AssignStmt*)s.stmts[11])->getRight());
	//dealWithExpType(((sgs::AssignStmt*)s.stmts[6])->getLeft());
	system("pause");
}