#include <fstream>
#include "generator.h"

using namespace sgs;

void generateCpp(vector<AST *> stmts,
	vector<ClassType *>classList, string file) {
	std::ofstream fout(file);

	fout << "#include <iostream>" << std::endl;
	fout << "#include <fstream>" << std::endl;
	fout << "#include <string>" << std::endl;
	fout << "#include <vector>" << std::endl;
	fout << std::endl;
	fout << "using std::string;" << std::endl;
	fout << "using std::vector;" << std::endl;
	fout << std::endl;

	for (auto ast : stmts) {
		switch (ast->astType) {
		case AT_VARDEF: {
			VarDef *currentStmt = dynamic_cast<VarDef *>(ast);
			fout << cppTab(cppDepth);
			translateVarDec(currentStmt, fout);
			fout << ";" << std::endl;
			break;
		}
		case AT_CLASS: {
			ClassDef *currentStmt = dynamic_cast<ClassDef *>(ast);
			fout << std::endl;
			translateClassDec(currentStmt, fout);
			fout << ";" << std::endl;
			break;
		}
		case AT_PROTO: {
			FuncProto *currentStmt = dynamic_cast<FuncProto *>(ast);
			fout << cppTab(cppDepth);
			translateFuncDec(currentStmt, fout);
			fout << ";" << std::endl;
			break;
		}
		case AT_FUNC: {
			FuncDef *currentStmt = dynamic_cast<FuncDef *>(ast);
			fout << std::endl;
			translateFuncDef(currentStmt, fout);
			break;
		}
		default:
			break;
		}
	}

	fout << "int main() {" << std::endl;
	cppDepth++;

	for (auto ast : stmts) {
		switch (ast->astType) {
		case AT_STMT: {
			Statement *currentStmt = dynamic_cast<Statement *>(ast);
			translateStmtExe(currentStmt, fout);
			break;
		}
		default:
			break;
		}
	}

	fout << cppTab(cppDepth) << "return 0;" << std::endl;
	fout << "}";
	cppDepth--;
	fout.close();
}

string removeSpace(const string& input) {
	string output = input;
	for (auto& x : output) {
		if (x == ' ') x = '_';
	}
	return output;
}

void translateBasicType(VarType *t, std::ofstream &fout) {
	auto* basicVar = dynamic_cast<BasicType *>(t);
	switch (basicVar->getBasicType()) {
	case BT_INT:
		fout << "int";
		break;
	case BT_FLOAT:
		fout << "float";
		break;
	case BT_BOOL:
		fout << "bool";
		break;
	case BT_CHAR:
		fout << "char";
		break;
	case BT_STRING:
		fout << "string";
		break;
	default:
		break;
	}
}
void translateVarType(VarType *t, std::ofstream &fout) {
	switch (t->getVarType()) {
	case VAR_TYPE::VT_BASIC:
		translateBasicType(t, fout);
		break;
	case VAR_TYPE::VT_ARRAY:
		fout << "vector<";
		translateVarType(((ArrayType *)t)->getEleType(), fout);
		fout << ">";
		break;
	case VAR_TYPE::VT_CLASS:
		fout << removeSpace(((ClassType *)t)->getName());
		break;
	default:
		break;
	}
}
void translateOpString(OPERATOR OP, std::ofstream &fout) {
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
		fout << ";";
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
		fout << " >= ";
		break;
	case SGS_OP_GREATER:
		fout << " > ";
		break;
	case SGS_OP_NGREATER:
		fout << " <= ";
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
		fout << "?";
		break;
	case SGS_OP_QUOT:
		fout << "\'";
		break;
	case SGS_OP_DBQUOT:
		fout << "\"";
		break;
	case SGS_OP_CROSS:
		fout << "";
		break;
	default:
		break;
	}
}
void translateOpExp(Expression *s, std::ofstream &fout) {
	OpExp *opExp = (OpExp *)s;
	fout << "(";
	translateExpType(opExp->getLeft(), fout);
	translateOpString(opExp->getOp(), fout);
	translateExpType(opExp->getRight(), fout);
	fout << ")";
}
void translateLiteralExp(Expression *s, std::ofstream &fout) {
	LiteralExp *literalExp = (LiteralExp *)s;
	switch (literalExp->getType()->getVarType()) {
	case VAR_TYPE::VT_BASIC:
		switch (((BasicType *)literalExp->getType())->getBasicType()) {
		case BASIC_TYPE::BT_INT:
			fout << ((IntLiteral *)literalExp)->getValue();
			break;
		case BASIC_TYPE::BT_FLOAT:
			fout << ((FloatLiteral *)literalExp)->getValue();
			break;
		case BASIC_TYPE::BT_BOOL:
			if(((BoolLiteral *)literalExp)->getValue())
				fout << "true";
			else
				fout << "false";
			break;
		case BASIC_TYPE::BT_STRING:
			fout << "\"" << ((StrLiteral *)literalExp)->getValue() << "\"";
			break;
		default:
			break;
		}
		break;
	case VAR_TYPE::VT_ARRAY:
		break;
	case VAR_TYPE::VT_CLASS:
		break;
	default:
		break;
	}
}
void translateIdExp(sgs::Expression *s, std::ofstream &fout) {
	IdExp *idExp = (IdExp *)s;
	fout << removeSpace(idExp->getName());
	return;
}
void translateCallExp(sgs::Expression *s, std::ofstream &fout) {
	CallExp *callExp = (CallExp *)s;
	fout << removeSpace(callExp->getFunction()->getName()) << "(";
	vector <Expression *> param = callExp->getParam();
	for (int i = 0; i < param.size(); i++) {
		if(i > 0)fout << ", ";
		sgs::Expression *currentParameter = param[i];
		translateExpType(currentParameter, fout);
	}
	fout << ")";
}
void translateVisitExp(sgs::Expression *s, std::ofstream &fout) {
	VisitExp *visitExp = (VisitExp *)s;
	translateExpType(visitExp->getArray(), fout);
	fout << "[";
	translateExpType(visitExp->getIndex(), fout);
	fout << "]";
	return;
}
void translateAccessExp(sgs::Expression *s, std::ofstream &fout) {
	AccessExp* accessExp = (AccessExp *)s;
	translateExpType(accessExp->getObject(), fout);
	fout << ".";
	fout << removeSpace(accessExp->getMember());
}
void translateExpType(sgs::AST *s, std::ofstream &fout) {
	Expression *currentExp = (Expression *)s;
	switch (currentExp->getExpType()) {
	case ET_OP:
		translateOpExp(currentExp, fout);
		break;
	case ET_LITERAL:
		translateLiteralExp(currentExp, fout);
		break;
	case ET_IDENT:
		translateIdExp(currentExp, fout);
		break;
	case ET_CALL:
		translateCallExp(currentExp, fout);
		break;
	case ET_VISIT:
		translateVisitExp(currentExp, fout);
		break;
	case ET_ACCESS:
		translateAccessExp(currentExp, fout);
		break;
	default:
		break;
	}
}

void translateAssignStmt(Statement *s, std::ofstream &fout) {
	AssignStmt*assignStmt = (AssignStmt *)s;
	translateExpType(assignStmt->getLeft(), fout);
	fout << " = ";
	translateExpType(assignStmt->getRight(), fout);
	fout << ";";
}
void translateCallStmt(Statement *s, std::ofstream &fout) {
	CallStmt *callStmt = (CallStmt *)s;
	fout << removeSpace(callStmt->getFunction()->getName()) << "(";
	vector <Expression *> param = callStmt->getParam();
	for (unsigned int i = 0; i < param.size(); ++i) {
		if (i > 0)fout << ", ";
		sgs::Expression *currentParam = param[i];
		translateExpType(currentParam, fout);
	}
	fout << ");";
}
void translateIfStmt(Statement *s, std::ofstream &fout) {
	IfStmt *ifStmt = (IfStmt *)s;
	fout << "if(";
	translateExpType(ifStmt->getCond(), fout);
	fout << ") {" << std::endl;
	cppDepth++;
	translateBlockStmt(ifStmt->getTaken(), fout);
	cppDepth--;
	fout << cppTab(cppDepth) << "}" << std::endl;

	if (ifStmt->getUntaken()) {
		fout << cppTab(cppDepth) << "else {" << std::endl;
		cppDepth++;
		translateBlockStmt(ifStmt->getUntaken(), fout);
		cppDepth--;
		fout << cppTab(cppDepth) << "}" << std::endl;
	}
}
void translateWhileStmt(Statement *s, std::ofstream &fout) {
	WhileStmt *whileStmt = (WhileStmt *)s;
	fout << "while(";
	translateExpType(whileStmt->getCondition(), fout);
	fout << ") {" << std::endl;
	cppDepth++;
	translateBlockStmt(whileStmt->getBody(), fout);
	cppDepth--;
	fout << cppTab(cppDepth) << "}" << std::endl;
}
void translateBlockStmt(Statement *s, std::ofstream &fout) {
	BlockStmt *blockStmt = (BlockStmt *)s;
	for (auto ast : blockStmt->getContent()) {
		switch (ast->astType) {
		case AT_VARDEF: {
			VarDef *currentStmt = dynamic_cast<VarDef *>(ast);
			fout << cppTab(cppDepth);
			translateVarDec(currentStmt, fout);
			fout << ";" << std::endl;
			break;
		}
		case AT_STMT: {
			Statement *currentStmt = dynamic_cast<Statement *>(ast);
			translateStmtExe(currentStmt, fout);
			break;
		}
		default:
			break;
		}
	}
}

void translateVarDec(AST *s, std::ofstream &fout) {
	VarDef *currentStmt = (VarDef *)s;
	string currentName = removeSpace(currentStmt->getName());

	translateVarType(currentStmt->getDecType(), fout);
	fout << " " << currentName;
}
void translateClassDec(AST *s, std::ofstream &fout) {
	ClassDef* classVar = (ClassDef *)s;
	fout << cppTab(cppDepth) << "class " <<
		classVar->getDecType()->getName() << " {" << std::endl;
	fout << cppTab(cppDepth) << "public:" << std::endl;
	cppDepth++;

	vector <std::pair<VarType *, string>> elements = classVar->getDecType()->getEle();
	for (unsigned int i = 0; i < elements.size(); i++) {
		fout << cppTab(cppDepth);
		translateVarType(elements[i].first, fout);
		fout << " " << removeSpace(elements[i].second);
		fout << ";" << std::endl;
	}

	cppDepth--;
	fout << "}";
}
void translateFuncDec(AST *s, std::ofstream &fout) {
	FuncProto* currentStmt = (FuncProto *)s;
	if (currentStmt->getReturnType() == NULL) {
		fout << "void";
	}
	else {
		translateVarType(currentStmt->getReturnType(), fout);
	}
	fout << " ";
	fout << removeSpace(currentStmt->getName());
	fout << "(";

	vector <std::pair<VarType *, string>> param = currentStmt->getParam();
	for (unsigned int i = 0; i < param.size(); i++) {
		if (i > 0)fout << ", ";
		translateVarType(param[i].first, fout);
		fout << " " << removeSpace(param[i].second);
	}

	fout << ")";
}
void translateFuncDef(AST *s, std::ofstream &fout) {

}
void translateStmtExe(AST *s, std::ofstream &fout) {
	Statement*currentStmt = (Statement *)s;
	fout << cppTab(cppDepth);
	switch (currentStmt->getStmtType()) {
	case STMT_TYPE::ST_ASSIGN:
		translateAssignStmt(currentStmt, fout);
		break;
	case STMT_TYPE::ST_CALL:
		translateCallStmt(currentStmt, fout);
		break;
	case STMT_TYPE::ST_IF:
		translateIfStmt(currentStmt, fout);
		break;
	case STMT_TYPE::ST_WHILE:
		translateWhileStmt(currentStmt, fout);
		break;
	case STMT_TYPE::ST_RETURN:
		fout << cppTab(cppDepth) << "return result;";
		break;
	case STMT_TYPE::ST_BREAK:
		fout << cppTab(cppDepth) << "break;";
		break;
	case STMT_TYPE::ST_CONTINUE:
		fout << cppTab(cppDepth) << "continue;";
		break;
	case STMT_TYPE::ST_BLOCK:
		translateBlockStmt(currentStmt, fout);
		break;
	default:
		break;
	}
	fout << std::endl;
}
