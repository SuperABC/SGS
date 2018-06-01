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
void dealWithExpType(sgs::AST *s);
void dealWithStmtType(sgs::AST *s);
void dealWithFuncDefType(sgs::AST *s);
void dealWithFuncProtoType(sgs::AST *s);
void testTool(vector<sgs::AST *> stmts);

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
	std::cout << "decType: VT_ARRAY" << std::endl;
	std::cout << "length:  " << arrayVar->getLength() << std::endl;
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
	std::cout << "decType: VT_CLASS" << std::endl;
	std::cout << "className:  " << classVar->getName() << std::endl;
	std::cout << "classMember: " << std::endl;
	int count = classVar->getEle().size();
	vector <std::pair<VarType *, string>> classElements = classVar->getEle();
	for (int i = 0; i < count; ++i)
	{
		sgs::VarType *currentMember = classElements[i].first;
		std::cout << "No." << (i + 1) << " Member:" << std::endl;
		std::cout << "memberName:" + classElements[i].second << std::endl;
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
void opSwitchCase(SGSOPERATOR OP)
{
	switch (OP)
	{
	case SGS_OP_PLUS:
		std::cout << "PLUS" << std::endl;
		break;
	case SGS_OP_PLUSPLUS:
		std::cout << "PLUSPLUS" << std::endl;
		break;
	case SGS_OP_EQPLUS:
		std::cout << "EQPLUS" << std::endl;
		break;
	case SGS_OP_MINUS:
		std::cout << "MINUS" << std::endl;
		break;
	case SGS_OP_MINUSMINUS:
		std::cout << "MINUSMINUS" << std::endl;
		break;
	case SGS_OP_EQMINUS:
		std::cout << "EQMINUS" << std::endl;
		break;
	case SGS_OP_NEG:
		std::cout << "NEG" << std::endl;
		break;
	case SGS_OP_MULTY:
		std::cout << "MULTY" << std::endl;
		break;
	case SGS_OP_EQMULTY:
		std::cout << "EQMULTY" << std::endl;
		break;
	case SGS_OP_DIVIDE:
		std::cout << "DIVIDE" << std::endl;
		break;
	case SGS_OP_EQDIVIDE:
		std::cout << "EQDIVIDE" << std::endl;
		break;
	case SGS_OP_MOD:
		std::cout << "MOD" << std::endl;
		break;
	case SGS_OP_EQMOD:
		std::cout << "EQMOD" << std::endl;
		break;
	case SGS_OP_AND:
		std::cout << "AND" << std::endl;
		break;
	case SGS_OP_ANDAND:
		std::cout << "ANDAND" << std::endl;
		break;
	case SGS_OP_EQAND:
		std::cout << "EQAND" << std::endl;
		break;
	case SGS_OP_OR:
		std::cout << "OR" << std::endl;
		break;
	case SGS_OP_OROR:
		std::cout << "OROR" << std::endl;
		break;
	case SGS_OP_EQOR:
		std::cout << "EQOR" << std::endl;
		break;
	case SGS_OP_NOR:
		std::cout << "NOR" << std::endl;
		break;
	case SGS_OP_EQNOR:
		std::cout << "EQNOR" << std::endl;
		break;
	case SGS_OP_INVERSE:
		std::cout << "INVERSE" << std::endl;
		break;
	case SGS_OP_EQINVERSE:
		std::cout << "EQINVERSE" << std::endl;
		break;
	case SGS_OP_LBRACE:
		std::cout << "LBRACE" << std::endl;
		break;
	case SGS_OP_RBRACE:
		std::cout << "RBRACE" << std::endl;
		break;
	case SGS_OP_LPARENTHESIS:
		std::cout << "LPARENTHESIS" << std::endl;
		break;
	case SGS_OP_RPARENTHESIS:
		std::cout << "RPARENTHESIS" << std::endl;
		break;
	case SGS_OP_LBRAKET:
		std::cout << "LBRAKET" << std::endl;
		break;
	case SGS_OP_RBRAKET:
		std::cout << "RBRAKET" << std::endl;
		break;
	case SGS_OP_SEMI:
		std::cout << "SEMI" << std::endl;
		break;
	case SGS_OP_COMMA:
		std::cout << "COMMA" << std::endl;
		break;
	case SGS_OP_DOT:
		std::cout << "DOT" << std::endl;
		break;
	case SGS_OP_SMALLER:
		std::cout << "SMALLER" << std::endl;
		break;
	case SGS_OP_NSMALLER:
		std::cout << "NSMALLER" << std::endl;
		break;
	case SGS_OP_GREATER:
		std::cout << "GREATER" << std::endl;
		break;
	case SGS_OP_NGREATER:
		std::cout << "NGREATER" << std::endl;
		break;
	case SGS_OP_NOT:
		std::cout << "NOT" << std::endl;
		break;
	case SGS_OP_NOTEQ:
		std::cout << "NOTEQ" << std::endl;
		break;
	case SGS_OP_EQUAL:
		std::cout << "EQUAL" << std::endl;
		break;
	case SGS_OP_QUERY:
		std::cout << "QUERY" << std::endl;
		break;
	case SGS_OP_QUOT:
		std::cout << "QUOT" << std::endl;
		break;
	case SGS_OP_DBQUOT:
		std::cout << "DBQUOT" << std::endl;
		break;
	case SGS_OP_CROSS:
		std::cout << "CROSS" << std::endl;
		break;
	default:
		break;
	}
}
void printOpExp(sgs::Expression *stmtExp)
{
	sgs::OpExp *opExp = (sgs::OpExp *)stmtExp;
	std::cout << "opExpOperator:"  << std::endl;
	opSwitchCase(opExp->getOp());
	std::cout << "leftExp:" << std::endl;
	dealWithExpType(opExp->getLeft());
	std::cout << "RightExp:" << std::endl;
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
			std::cout << "No. " << i << " content" << std::endl;
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
	sgs::CallExp *callExp = (sgs::CallExp *)stmtExp;
	std::cout << "function" << std::endl;
	dealWithFuncProtoType(callExp->getFunction());
	int count = callExp->getParam().size();
	vector <Expression *> paramList = callExp->getParam();
	for (int i = 0; i < count; ++i)
	{
		sgs::Expression *currentParameter = paramList[i];
		std::cout << "No." << (i + 1) << " parameter:" << std::endl;
		dealWithExpType(currentParameter);
	}
}
void printAccessExp(sgs::Expression *stmtExp)
{
	sgs::AccessExp *accessExp = (sgs::AccessExp *)stmtExp;
	std::cout << "object:" << std::endl;
	dealWithExpType(accessExp->getObject());
	std::cout << "member" << std::endl;
	std::cout << accessExp->getMember() << std::endl;
}
void printAssignStmt(sgs::Statement *stmtStmt)
{
	sgs::AssignStmt *AssignStmt = (sgs::AssignStmt *)stmtStmt;
	std::cout << "Left Expression:" << std::endl;
	dealWithExpType(AssignStmt->getLeft());
	std::cout << "Right Expression:" << std::endl;
	dealWithExpType(AssignStmt->getRight());
}
void printCallStmt(sgs::Statement *stmtStmt)
{
	sgs::CallStmt *callStmt = (sgs::CallStmt *)stmtStmt;
	std::cout << "function" << std::endl;
	dealWithFuncProtoType(callStmt->getFunction());
	int count = callStmt->getParam().size();
	vector <Expression *> paramList = callStmt->getParam();
	for (int i = 0; i < count; ++i)
	{
		sgs::Expression *currentParameter = paramList[i];
		std::cout << "No." << (i + 1) << " parameter:" << std::endl;
		dealWithExpType(currentParameter);
	}
}
void printBlockStmt(sgs::Statement *stmtStmt)
{
	sgs::BlockStmt *blockStmt = (sgs::BlockStmt *)stmtStmt;
	std::cout << "block content" << std::endl;
	testTool(blockStmt->getContent());
}
void printIfStmt(sgs::Statement *stmtStmt)
{
	sgs::IfStmt *IfStmt = (sgs::IfStmt *)stmtStmt;
	std::cout << "If condition:" << std::endl;
	dealWithExpType(IfStmt->getCond());
	std::cout << "taken:" << std::endl;
	printBlockStmt(IfStmt->getTaken());
	std::cout << "untaken:" << std::endl;
	printBlockStmt(IfStmt->getUntaken());
}
void printWhileStmt(sgs::Statement *stmtStmt)
{
	sgs::WhileStmt *whileStmt = (sgs::WhileStmt *)stmtStmt;
	std::cout << "condition:" << std::endl;
	dealWithExpType(whileStmt->getCondition());
	std::cout << "body" << std::endl;
	printBlockStmt(whileStmt->getBody());
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
		std::cout << "className: " + currentStmt->getDecType()->getName() << std::endl;
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
			std::cout << "return type is void" << std::endl;
			break;
		}
		switch (currentStmt->getReturnType()->getVarType())
		{
		case VAR_TYPE::VT_BASIC: printBasicType(currentStmt->getReturnType()); break;
		case VAR_TYPE::VT_ARRAY: printArrayType(currentStmt->getReturnType()); break;
		case VAR_TYPE::VT_CLASS: printClassType(currentStmt->getReturnType()); break;
		default:break;
		}
		break;
	}
	default:
		break;
	}
	
}//not complete
void dealWithExpType(sgs::AST *s)
{
	sgs::Expression *currentStmt = (sgs::Expression *)s;
	switch (currentStmt->getExpType())
	{
	case EXP_TYPE::ET_OP: 
	{
		std::cout << "expType: ET_OP" << std::endl;
		printOpExp(currentStmt);
		break;
	}
	case EXP_TYPE::ET_LITERAL:
	{
		std::cout << "expType: ET_LITERAL" << std::endl;
		printLiteralExp(currentStmt);
		break;
	}
	case EXP_TYPE::ET_IDENT:
	{
		std::cout << "expType: ET_IDENT" << std::endl;
		printIdExp(currentStmt);
		break;
	}
	case EXP_TYPE::ET_VISIT:
	{
		std::cout << "expType: ET_VISIT" << std::endl;
		printVisitExp(currentStmt);
		break;
	}
	case EXP_TYPE::ET_CALL:
	{
		std::cout << "expType: ET_CALL" << std::endl;
		printCallExp(currentStmt);
		break;
	}
	case EXP_TYPE::ET_ACCESS:
	{
		std::cout << "expType: ET_ACCESS" << std::endl;
		printAccessExp(currentStmt);
		break;
	}
	default:break;
	}
}
void dealWithStmtType(sgs::AST *s)
{
	sgs::Statement *currentStmt = (sgs::Statement *)s;
	switch (currentStmt->getStmtType())
	{
	case STMT_TYPE::ST_ASSIGN: 
	{
		std::cout << "stmtType: ST_ASSIGN" << std::endl;
		printAssignStmt(currentStmt);
		break;
	}
	case STMT_TYPE::ST_CALL: 
	{
		std::cout << "stmtType: ST_CALL" << std::endl; 
		printCallStmt(currentStmt);
		break; 
	}
	case STMT_TYPE::ST_IF: 
	{
		std::cout << "stmtType: ST_IF" << std::endl;
		printIfStmt(currentStmt);
		break;
	}
	case STMT_TYPE::ST_WHILE: 
	{
		std::cout << "stmtType: ST_WHILE" << std::endl;
		printWhileStmt(currentStmt);
		break;
	}
	case STMT_TYPE::ST_RETURN: std::cout << "stmtType: ST_RETURN" << std::endl; break;
	case STMT_TYPE::ST_BREAK: std::cout << "stmtType: ST_BREAK" << std::endl; break;
	case STMT_TYPE::ST_CONTINUE: std::cout << "stmtType: ST_CONTINUE" << std::endl; break;
	case STMT_TYPE::ST_BLOCK: 
	{
		std::cout << "stmtType: ST_BLOCK" << std::endl; 
		printAssignStmt(currentStmt);
		break;
	}
	default:break;
	}
}
void dealWithFuncDefType(sgs::AST *s)
{
	sgs::FuncDef *currentStmt = (sgs::FuncDef *)s;
	std::cout << "proto:" << std::endl;
	dealWithFuncProtoType(currentStmt->getProto());
	std::cout << "body:" << std::endl;
	printBlockStmt(currentStmt->getBody());
}
void dealWithFuncProtoType(sgs::AST *s)
{
	sgs::FuncProto *currentStmt = (sgs::FuncProto *)s;
	std::cout << "name:" << std::endl;
	std::cout << currentStmt->getName() << std::endl;
	std::cout << "parameters:" << std::endl;
	int count = currentStmt->getParam().size();
	vector <std::pair<VarType *, string>> paramList = currentStmt->getParam();
	for (int i = 0; i < count; ++i)
	{
		sgs::VarType *currentParameter = paramList[i].first;
		std::cout << "No." << (i + 1) << " parameter:" << std::endl;
		std::cout << "parameterName:" + paramList[i].second << std::endl;
		switch (currentParameter->getVarType())
		{
		case VAR_TYPE::VT_BASIC: printBasicType(currentParameter); break;
		case VAR_TYPE::VT_ARRAY: printArrayType(currentParameter); break;
		case VAR_TYPE::VT_CLASS: printClassType(currentParameter); break;
		default:break;
		}
	}
	std::cout << "return value type:" << std::endl;
	dealWithVarType(currentStmt, conditionUseVarType::PROTO);
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
			std::cout << std::endl;
			break;
		}
		case AT_CLASS:
		{
			sgs::ClassDef *currentStmt = (sgs::ClassDef *)stmts[loopNum];
			std::cout << "astType: AT_CLASS" << std::endl;
			dealWithVarType(currentStmt, CLASS);
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
			dealWithStmtType(currentStmt);
			std::cout << std::endl;
			break;
		}
		case AT_FUNC:
		{
			sgs::FuncDef *currentStmt = (sgs::FuncDef *)stmts[loopNum];
			std::cout << "astType: AT_FUNC" << std::endl;
			dealWithFuncDefType(currentStmt);
			std::cout << std::endl;
			break;
		}
		case AT_PROTO:
		{
			sgs::FuncProto *currentStmt = (sgs::FuncProto *)stmts[loopNum];
			std::cout << "astType: AT_PROTO" << std::endl; 
			dealWithFuncProtoType(currentStmt);
			std::cout << std::endl;
			break;
		}
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
	//dealWithExpType(((sgs::AssignStmt*)s.stmts[11])->getRight());
	//dealWithExpType(((sgs::AssignStmt*)s.stmts[6])->getLeft());
	system("pause");
}