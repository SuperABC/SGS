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
void translateClassType(sgs::VarType *stmtVar, std::ofstream &fout) {
	sgs::ClassType *classVar = (sgs::ClassType *)stmtVar;
	fout << "class " << classVar->getName() << "{" << std::endl;
	fout << "public:" << std::endl;
	int count = classVar->getEle().size();
	vector <std::pair<VarType *, string>> classElements = classVar->getEle();
	for (int i = 0; i < count; ++i) {
		sgs::VarType *currentMember = classElements[i].first;
		switch (currentMember->getVarType()) {
		case VAR_TYPE::VT_BASIC: translateBasicType(currentMember, fout); break;
		case VAR_TYPE::VT_ARRAY:
		{
			VAR_TYPE type = translateArrayType(currentMember, fout);
			string currentName = classElements[i].second;
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
				fout << ((sgs::ClassType *)currentMember)->getName() << ' ';
				fout << currentName << "[]";
			}
			break;
		}
		case VAR_TYPE::VT_CLASS: translateClassType(currentMember, fout); break;
		default:break;
		}
		fout << classElements[i].second << ";" << std::endl;
	}
	fout << "}" << std::endl;
	return;
}
void translateOpSwitchCase(SGSOPERATOR OP, std::ofstream &fout) {
	switch (OP) {
	case SGS_OP_PLUS:
		fout << " + ";
		break;
	case SGS_OP_PLUSPLUS:
		fout << "++";
		break;
	case SGS_OP_EQPLUS:
		fout << " += ";
		break;
	case SGS_OP_MINUS:
		fout << " - ";
		break;
	case SGS_OP_MINUSMINUS:
		fout << "--";
		break;
	case SGS_OP_EQMINUS:
		fout << " -= ";
		break;
	case SGS_OP_NEG:
		fout << "-";
		break;
	case SGS_OP_MULTY:
		fout << " * ";
		break;
	case SGS_OP_EQMULTY:
		fout << " *= ";
		break;
	case SGS_OP_DIVIDE:
		fout << " / ";
		break;
	case SGS_OP_EQDIVIDE:
		fout << " /= ";
		break;
	case SGS_OP_MOD:
		fout << " % ";
		break;
	case SGS_OP_EQMOD:
		fout << " %= ";
		break;
	case SGS_OP_AND:
		fout << " & ";
		break;
	case SGS_OP_ANDAND:
		fout << " && ";
		break;
	case SGS_OP_EQAND:
		fout << " &= ";
		break;
	case SGS_OP_OR:
		fout << " | ";
		break;
	case SGS_OP_OROR:
		fout << " || ";
		break;
	case SGS_OP_EQOR:
		fout << " |= ";
		break;
	case SGS_OP_NOR:
		fout << " ^ ";
		break;
	case SGS_OP_EQNOR:
		fout << " ^= ";
		break;
	case SGS_OP_INVERSE:
		fout << "~";
		break;
	case SGS_OP_EQINVERSE:
		fout << " ~= ";
		break;
	case SGS_OP_LBRACE:
		fout << "{";
		break;
	case SGS_OP_RBRACE:
		fout << "}";
		break;
	case SGS_OP_LPARENTHESIS:
		fout << "(";
		break;
	case SGS_OP_RPARENTHESIS:
		fout << ")";
		break;
	case SGS_OP_LBRAKET:
		fout << "[";
		break;
	case SGS_OP_RBRAKET:
		fout << "]";
		break;
	case SGS_OP_SEMI:
		fout << "SEMI";
		break;
	case SGS_OP_COMMA:
		fout << ",";
		break;
	case SGS_OP_DOT:
		fout << ".";
		break;
	case SGS_OP_SMALLER:
		fout << " < ";
		break;
	case SGS_OP_NSMALLER:
		fout << " <= ";
		break;
	case SGS_OP_GREATER:
		fout << " > ";
		break;
	case SGS_OP_NGREATER:
		fout << " >= ";
		break;
	case SGS_OP_NOT:
		fout << " !";
		break;
	case SGS_OP_NOTEQ:
		fout << " != ";
		break;
	case SGS_OP_EQUAL:
		fout << " == ";
		break;
	case SGS_OP_QUERY:
		fout << "QUERY";
		break;
	case SGS_OP_QUOT:
		fout << "\'";
		break;
	case SGS_OP_DBQUOT:
		fout << "\"";
		break;
	case SGS_OP_CROSS:
		fout << "CROSS";
		break;
	default:
		break;
	}
}
void translateOpExp(sgs::Expression *stmtExp, std::ofstream &fout) {
	sgs::OpExp *opExp = (sgs::OpExp *)stmtExp;
	fout << "(";
	translateExpType(opExp->getLeft(), fout);
	translateOpSwitchCase(opExp->getOp(), fout);
	translateExpType(opExp->getRight(), fout);
	fout << ")";
}
void translateLiteralExp(sgs::Expression *stmtExp, std::ofstream &fout) {
	sgs::LiteralExp *literalExp = (sgs::LiteralExp *)stmtExp;
	switch (literalExp->getType()->getVarType()) {
	case VAR_TYPE::VT_BASIC:
	{
		sgs::BasicType *basicLiteralExp = (sgs::BasicType *)literalExp->getType();
		switch (basicLiteralExp->getBasicType()) {
		case BASIC_TYPE::BT_INT:
		{
			sgs::IntLiteral *intLiteralExp = (sgs::IntLiteral *)literalExp;
			fout << intLiteralExp->getValue();
			break;
		}
		case BASIC_TYPE::BT_FLOAT:
		{
			sgs::FloatLiteral *floatLiteralExp = (sgs::FloatLiteral *)literalExp;
			fout << floatLiteralExp->getValue();
			break;
		}
		case BASIC_TYPE::BT_BOOL:
		{
			sgs::BoolLiteral *boolLiteralExp = (sgs::BoolLiteral *)literalExp;
			if (boolLiteralExp->getValue() == true)
				fout << "true";
			else
				fout << "false";
			break;
		}
		case BASIC_TYPE::BT_STRING:
		{
			sgs::StrLiteral *strLiteralExp = (sgs::StrLiteral *)literalExp;
			fout << "\"" << strLiteralExp->getValue() << "\"";
			break;
		}
		default:
			break;
		}
		break;
	}
	case VAR_TYPE::VT_ARRAY:
	{
		/*sgs::ArrayLiteral *arrayLiteralExp = (sgs::ArrayLiteral *)literalExp;
		std::cout << Tab(depth) << "Array value:" << std::endl;
		depth++;
		vector<Expression *>arrayConent = arrayLiteralExp->getValue();
		int count = arrayConent.size();
		for (int i = 0; i < count; ++i) {
			std::cout << Tab(depth) << "No. " << i << " content" << std::endl;
			depth++; dealWithExpType(arrayConent[i]); depth--;
		}
		depth--;
		break;*/
	}
	case VAR_TYPE::VT_CLASS:
	{
		sgs::ClassLiteral *classLiteralExp = (sgs::ClassLiteral *)literalExp;
		vector<Expression *>classConent = classLiteralExp->getValue();
		int count = classConent.size();
		fout << "(";
		for (int i = 0; i < count; ++i) {
			translateExpType(classConent[i], fout);
			if (i != count - 1)
				fout << ", ";
			else
				fout << ")";
		}
		break;
	}
	default:
		break;
	}
}
void translateIdExp(sgs::Expression *stmtExp, std::ofstream &fout) {
	sgs::IdExp *idExp = (sgs::IdExp *)stmtExp;
	fout << removeSpace(idExp->getName());
	return;
}
void translateVisitExp(sgs::Expression *stmtExp, std::ofstream &fout) {
	sgs::VisitExp *visitExp = (sgs::VisitExp *)stmtExp;
	translateExpType(visitExp->getArray(), fout);
	fout << "[";
	translateExpType(visitExp->getIndex(), fout);
	fout << "]";
	return;
}
void translateCallExp(sgs::Expression *stmtExp, std::ofstream &fout) {
	sgs::CallExp *callExp = (sgs::CallExp *)stmtExp;
	fout << removeSpace(callExp->getFunction()->getName()) << "(";
	int count = callExp->getParam().size();
	vector <Expression *> paramList = callExp->getParam();
	if (count == 0)
	{
		fout << ")";
		return;
	}
	for (int i = 0; i < count; ++i) {
		sgs::Expression *currentParameter = paramList[i];
		translateExpType(currentParameter, fout);
		if (i == count - 1)
		{
			fout << ")";
		}
		else
		{
			fout << ", ";
		}
	}
}
void translateAccessExp(sgs::Expression *stmtExp, std::ofstream &fout) {
	sgs::AccessExp *accessExp = (sgs::AccessExp *)stmtExp;
	translateExpType(accessExp->getObject(), fout);
	fout << ".";
	fout << removeSpace(accessExp->getMember());
}
void translateAssignStmt(sgs::Statement *stmtStmt, std::ofstream &fout) {
	sgs::AssignStmt *AssignStmt = (sgs::AssignStmt *)stmtStmt;
	translateExpType(AssignStmt->getLeft(), fout);
	fout << " = ";
	translateExpType(AssignStmt->getRight(), fout);
	fout << ";" << std::endl;
}
void translateCallStmt(sgs::Statement *stmtStmt, std::ofstream &fout) {
	sgs::CallStmt *callStmt = (sgs::CallStmt *)stmtStmt;
	fout << removeSpace(callStmt->getFunction()->getName()) << "(";
	int count = callStmt->getParam().size();
	vector <Expression *> paramList = callStmt->getParam();
	for (int i = 0; i < count; ++i) {
		sgs::Expression *currentParameter = paramList[i];
		translateExpType(currentParameter, fout);
		if (i == count - 1)
			fout << ");" << std::endl;
		else
			fout << ", ";
	}
}
void translateBlockStmt(sgs::Statement *stmtStmt, std::ofstream &fout) {
	sgs::BlockStmt *blockStmt = (sgs::BlockStmt *)stmtStmt;
	translateAST(blockStmt->getContent(), fout);
}
void translateIfStmt(sgs::Statement *stmtStmt, std::ofstream &fout) {
	sgs::IfStmt *IfStmt = (sgs::IfStmt *)stmtStmt;
	fout << "if(";
	translateExpType(IfStmt->getCond(), fout);
	fout << "){" << std::endl;
	translateBlockStmt(IfStmt->getTaken(), fout);
	fout << "}" << std::endl;
	fout << "else{" << std::endl;
	translateBlockStmt(IfStmt->getUntaken(), fout);
	fout << "}" << std::endl;
}
void translateWhileStmt(sgs::Statement *stmtStmt, std::ofstream &fout) {
	sgs::WhileStmt *whileStmt = (sgs::WhileStmt *)stmtStmt;
	fout << "while(";
	translateExpType(whileStmt->getCondition(), fout);
	fout << "){" << std::endl;
	translateBlockStmt(whileStmt->getBody(), fout);
	fout << "}" << std::endl;
}
void translateVarType(sgs::AST *s, enum conditionUseVarType choice, std::ofstream &fout)
{
	switch (choice)
	{
	case conditionUseVarType::TYPEDEF: //TYPEDEF
	{
		sgs::TypeDef *currentStmt = (sgs::TypeDef *)s;
		string currentName = removeSpace(currentStmt->getName());
		switch (currentStmt->getDecType()->getVarType())
		{
		case VAR_TYPE::VT_BASIC: 
		{
			translateBasicType(currentStmt->getDecType(), fout);
			fout << currentName;
			break;
		}
		case VAR_TYPE::VT_ARRAY:
		{
			VAR_TYPE type = translateArrayType(currentStmt->getDecType(), fout);
			if (VAR_TYPE::VT_BASIC == type)
			{
				fout << currentName << "[" << ((sgs::ArrayType *)currentStmt->getDecType())->getLength() << "]";
			}
			else if (VAR_TYPE::VT_ARRAY == type)
			{
				fout << currentName << "[][]";
			}
			else if (VAR_TYPE::VT_CLASS == type)
			{
				fout << ((sgs::ClassType *)currentStmt->getDecType())->getName() << ' ';
				fout << currentName << "[" << ((sgs::ArrayType *)currentStmt->getDecType())->getLength() << "]";
			}
			break;
		}
		case VAR_TYPE::VT_CLASS: 
		{
			fout << removeSpace(((sgs::ClassType *)currentStmt->getDecType())->getName()) << ' ' << currentName;
			break;
		}
		default:break;
		}
		fout  << ";" << std::endl;
		break;
		}
	case conditionUseVarType::CLASS:
	{
		sgs::ClassDef *currentStmt = (sgs::ClassDef *)s;
		translateClassType(currentStmt->getDecType(), fout);
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
		if (NULL == currentStmt->getProto()->getReturnType())
		{
			fout << "void ";
			break;
		}
		switch (currentStmt->getProto()->getReturnType()->getVarType())
		{
		case VAR_TYPE::VT_BASIC: translateBasicType(currentStmt->getProto()->getReturnType(), fout); break;
		case VAR_TYPE::VT_ARRAY: translateArrayType(currentStmt->getProto()->getReturnType(), fout); break;
		case VAR_TYPE::VT_CLASS: fout << (((sgs::ClassDef *)currentStmt)->getDecType())->getName(); break;
		default:break;
		}
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
{
	sgs::Expression *currentStmt = (sgs::Expression *)s;
	switch (currentStmt->getExpType())
	{
	case EXP_TYPE::ET_OP:
	{
		translateOpExp(currentStmt, fout);
		break;
	}
	case EXP_TYPE::ET_LITERAL:
	{
		translateLiteralExp(currentStmt, fout);
		break;
	}
	case EXP_TYPE::ET_IDENT:
	{
		translateIdExp(currentStmt, fout);
		break;
	}
	case EXP_TYPE::ET_VISIT:
	{
		translateVisitExp(currentStmt, fout);
		break;
	}
	case EXP_TYPE::ET_CALL:
	{
		translateCallExp(currentStmt, fout);
		break;
	}
	case EXP_TYPE::ET_ACCESS:
	{
		translateAccessExp(currentStmt, fout);
		break;
	}
	default:break;
	}
}
void translateStmtType(sgs::AST *s, std::ofstream &fout)
{
	sgs::Statement *currentStmt = (sgs::Statement *)s;
	switch (currentStmt->getStmtType())
	{
	case STMT_TYPE::ST_ASSIGN:
	{
		translateAssignStmt(currentStmt, fout);
		break;
	}
	case STMT_TYPE::ST_CALL:
	{
		translateCallStmt(currentStmt, fout);
		break;
	}
	case STMT_TYPE::ST_IF:
	{
		translateIfStmt(currentStmt, fout);
		break;
	}
	case STMT_TYPE::ST_WHILE:
	{
		translateWhileStmt(currentStmt, fout);
		break;
	}
	case STMT_TYPE::ST_RETURN: fout << "return result;" << std::endl; break;
	case STMT_TYPE::ST_BREAK: fout  << "break;" << std::endl; break;
	case STMT_TYPE::ST_CONTINUE: fout << "continue;" << std::endl; break;
	case STMT_TYPE::ST_BLOCK:
	{
		translateAssignStmt(currentStmt, fout);
		break;
	}
	default:break;
	}
}
void translateFuncDefType(sgs::AST *s, std::ofstream &fout)
{
	sgs::FuncDef *currentStmt = (sgs::FuncDef *)s;
	translateFuncProtoType(currentStmt->getProto(), fout);
	fout << "{" << std::endl;
	translateVarType(s, FUNC, fout);
	fout << "result;" << std::endl;
	translateBlockStmt(currentStmt->getBody(), fout);
	fout << "}" << std::endl;
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
		fout << ")";
		return;
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
			fout << ")";
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
			break;
		}
		case AT_EXP:
		{
			sgs::Expression *currentStmt = (sgs::Expression *)stmts[loopNum];
			translateExpType(currentStmt, fout);
			//translateVarType(currentStmt, EXP, fout);
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
			fout << ";" << std::endl;
			break;
		}
		default:
			break;
		}
	}
	return;
}
void translateToCPP(vector<sgs::AST *> stmts)
{
	std::ofstream fout("output.cpp");
	fout << "#include <iostream>" << std::endl;
	fout << "#include <string>" << std::endl;
	fout << "using std::string;" << std::endl;
	unsigned int loopNum;
	for (loopNum = 0; loopNum < stmts.size(); ++loopNum)
	{
		switch (stmts[loopNum]->astType)
		{
		case AT_CLASS:
		{
			sgs::ClassDef *currentStmt = (sgs::ClassDef *)stmts[loopNum];
			translateVarType(currentStmt, CLASS, fout);
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
			fout << ";" << std::endl;
			break;
		}
		default:
			break;
		}
	}
	fout << "int main(){" << std::endl;
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
		case AT_EXP:
		{
			sgs::Expression *currentStmt = (sgs::Expression *)stmts[loopNum];
			translateExpType(currentStmt, fout);
			//translateVarType(currentStmt, EXP, fout);
			break;
		}
		case AT_STMT:
		{
			sgs::Statement *currentStmt = (sgs::Statement *)stmts[loopNum];
			translateStmtType(currentStmt, fout);
			break;
		}
		default:
			break;
		}
	}
	fout << "return 0;" << std::endl;
	fout << "}";
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