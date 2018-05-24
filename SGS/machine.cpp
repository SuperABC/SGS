#define _CRT_SECURE_NO_WARNINGS
#include "machine.h"

using namespace sgs;

SgsMachine::SgsMachine() {

}
SgsMachine::~SgsMachine() {

}
void SgsMachine::initModule() {

}
void SgsMachine::loadDlls() {

}
void SgsMachine::addSymbol(VarNode *var) {
	string name = var->name;
	int index = 0;
	for (auto c : name)index += c;
	index = index % 256;
	Symbol *symbol = new Symbol(var);
	symbol->next = table[index];
	table[index] = symbol;
}
VarNode *SgsMachine::findSymbol(string name) {
	int index = 0;
	for (auto c : name)index += c;
	index = index % 256;
	Symbol *tmp = table[index];
	while (tmp) {
		if (tmp->var->name == name) {
			return tmp->var;
		}
	}
	return NULL;
}
void SgsMachine::removeLocal(vector<string> local) {
	for (auto s : local) {
		int index = 0;
		for (auto c : s)index += c;
		index = index % 256;
		Symbol *tmp = table[index];
		Symbol *pre = NULL;
		while (tmp) {
			if (tmp->var->name == s) {
				if (pre) {
					pre->next = tmp->next;
					delete tmp;
					break;
				}
				else {
					table[index] = tmp->next;
					delete tmp;
					break;
				}
			}
			else {
				pre = tmp;
				tmp = tmp->next;
			}
		}
	}
}

SgsMachine *SgsMachine::input(vector<AST *> s,
	vector<ClassType *> c, vector<FuncProto *> f) {
	stmts = s;
	classList = c;
	for (auto d : f) {
		funcList.push_back(std::pair<sgs::FuncProto *, sgs::FuncDef * >(d, NULL));
	}
	return this;
}
void SgsMachine::environment(void *env) {

}
void SgsMachine::execute() {
	for (auto s : stmts)step(s);
}
void SgsMachine::step(AST *s) {
	switch (s->astType) {
	case AT_TYPEDEF:
		declare(s);
		break;
	case AT_CLASS:
		structure(s);
		break;
	case AT_STMT:
		statement(s);
		break;
	case AT_PROTO:
		prototype(s);
		break;
	case AT_FUNC:
		definition(s);
		break;
	default:
		break;
	}
}
void SgsMachine::declare(AST *s) {
	TypeDef *dec = (TypeDef *)s;
	VarNode *tmp = new VarNode();
	tmp->type = dec->getDecType();
	tmp->name = dec->getName();
	addSymbol(tmp);
}
void SgsMachine::structure(AST *s) {

}
void SgsMachine::statement(AST *s) {
	Statement *stmt = (Statement *)s;
	switch (stmt->getStmtType()) {
	case ST_ASSIGN: {
		AssignStmt *assign = (AssignStmt *)stmt;
		assignValue(getPointer(assign->getLeft()), expValue(assign->getRight()));
		break;
	}
	case ST_CALL: {
		CallStmt *call = (CallStmt *)stmt;
		callFunc(call->getFunction(), call->getParam());
		break;
	}
	case ST_IF: {
		IfStmt *branch = (IfStmt *)stmt;
		if (getBool(expValue(branch->getCond()))) {
			exeBlock(branch->getTaken());
		}
		else {
			exeBlock(branch->getUntaken());
		}
		break;
	}
	case ST_WHILE: {
		WhileStmt *loop = (WhileStmt *)stmt;
		while (getBool(expValue(loop->getCondition()))) {
			exeBlock(loop->getBody());
		}
		break;
	}
	case ST_RETURN:
		break;
	case ST_BREAK:
		break;
	case ST_CONTINUE:
		break;
	default:
		break;
	}
}
void SgsMachine::prototype(AST *s) {

}
void SgsMachine::definition(AST *s) {
	FuncDef *def = (FuncDef *)s;
	for (auto &dec : funcList) {
		if (def->getProto()->getName() == dec.first->getName())
			dec.second = def;
	}
}

void SgsMachine::assignValue(VarNode *left, VarNode *right) {

}
VarNode *SgsMachine::callFunc(FuncProto *func, vector<Expression *> paras) {

}
void SgsMachine::exeBlock(BlockStmt *block) {

}
VarNode *SgsMachine::getPointer(Expression *e) {
	switch (e->getExpType()) {
	case ET_IDENT:
		return findSymbol(((IdExp *)e)->getName());
	case ET_VISIT:
		return arrayElement(e);
	case ET_ACCESS:
		return classAttrib(e);
		break;
	default:
		break;
	}
}
VarNode *SgsMachine::expValue(Expression *e) {

}
VarNode *SgsMachine::arrayElement(Expression *e) {

}
VarNode *SgsMachine::classAttrib(Expression *e) {

}

IntLiteral *SgsMachine::getInt(VarNode *val) {

}
FloatLiteral *SgsMachine::getFloat(VarNode *val) {

}
BoolLiteral *SgsMachine::getBool(VarNode *val) {

}
StrLiteral *SgsMachine::getStr(VarNode *val) {

}

void SgsMachine::clearMem() {
	macMem.clear();
}
void SgsMachine::error(const char *inst, SGSVMERROR type) {
	switch (type) {
	case VE_DIVBYZERO:
		msgList.push_back(sgsMsg(string("除数为零。"), MT_ERROR));
		break;
	case VE_TYPEMISMATCH:
		msgList.push_back(sgsMsg(string("无法进行类型转换。"), MT_ERROR));
		break;
	case VE_BROKEN:
		msgList.push_back(sgsMsg(string("虚拟机被破坏。"), MT_ERROR));
		break;
	}
}