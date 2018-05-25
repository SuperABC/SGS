#define _CRT_SECURE_NO_WARNINGS
#include "machine.h"

using namespace sgs;

string nameReform(string input) {
	string ret;
	for (unsigned int i = 0; i < input.length(); i++) {
		if (input[i] != ' ')ret.push_back(input[i]);
		else {
			ret.push_back(input[++i] & ~0x20);
		}
	}
	return ret;
}

SgsMachine::SgsMachine() {
	initModule();
}
SgsMachine::~SgsMachine() {

}
void SgsMachine::initModule() {
	loadDlls();
}
void SgsMachine::loadDlls() {
	dllList.push_back(LoadLibrary("Function.dll"));
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
void SgsMachine::removeLocal(vector<string> local, bool del) {
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
					if(del)delete tmp;
					break;
				}
				else {
					table[index] = tmp->next;
					if (del)delete tmp;
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
	VarNode *tmp = NULL;
	switch (dec->getDecType()->getVarType()) {
	case sgs::VT_BASIC:
		switch (((BasicType *)dec->getDecType())->getBasicType()) {
		case BT_INT:
			tmp = new IntNode(0, dec->getName());
			break;
		case BT_FLOAT:
			tmp = new FloatNode(0.f, dec->getName());
			break;
		case BT_BOOL:
			tmp = new BoolNode(false, dec->getName());
			break;
		case BT_STRING:
			tmp = new StrNode("", dec->getName());
			break;
		}
		break;
	case sgs::VT_ARRAY:
		tmp = new ArrayNode(((ArrayType *)dec->getDecType())->getEleType(),
			((ArrayType *)dec->getDecType())->getLength(), "");
		break;
	case sgs::VT_CLASS:
		tmp = new ClassNode(((ClassType *)dec->getDecType())->getEle(),
			((ClassType *)dec->getDecType())->getName(), "");
		break;
	}
	addSymbol(tmp);
}
void SgsMachine::structure(AST *s) {

}
void SgsMachine::statement(AST *s) { //suspend.
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
	switch (left->type->getVarType()) {
	case sgs::VT_BASIC:
		switch (((BasicType *)left->type)->getBasicType()) {
		case BT_INT:
			if (((BasicType *)right->type)->getBasicType() == BT_INT)
				((IntNode *)left)->value = (int)((IntNode *)right)->value;
			else if(((BasicType *)right->type)->getBasicType() == BT_FLOAT)
				((IntNode *)left)->value = (int)((FloatNode *)right)->value;
			else if (((BasicType *)right->type)->getBasicType() == BT_BOOL)
				((IntNode *)left)->value = (int)((BoolNode *)right)->value;
			else error("", VE_TYPEMISMATCH);
			break;
		case BT_FLOAT:
			if (((BasicType *)right->type)->getBasicType() == BT_INT)
				((FloatNode *)left)->value = (float)((IntNode *)right)->value;
			else if (((BasicType *)right->type)->getBasicType() == BT_FLOAT)
				((FloatNode *)left)->value = (float)((FloatNode *)right)->value;
			else if (((BasicType *)right->type)->getBasicType() == BT_BOOL)
				((FloatNode *)left)->value = (float)((BoolNode *)right)->value;
			else error("", VE_TYPEMISMATCH);
			break;
		case BT_BOOL:
			if (((BasicType *)right->type)->getBasicType() == BT_INT)
				((BoolNode *)left)->value = (bool)((IntNode *)right)->value;
			else if (((BasicType *)right->type)->getBasicType() == BT_FLOAT)
				((BoolNode *)left)->value = (bool)((FloatNode *)right)->value;
			else if (((BasicType *)right->type)->getBasicType() == BT_BOOL)
				((BoolNode *)left)->value = (bool)((BoolNode *)right)->value;
			else error("", VE_TYPEMISMATCH);
			break;
		case BT_STRING:
			if (((BasicType *)right->type)->getBasicType() == BT_INT) {
				char *buffer = (char *)malloc((int)log10(((IntNode *)right)->value) + 2);
				sprintf(buffer, "%d", ((IntNode *)right)->value);
				((StrNode *)left)->value = buffer;
			}
			else if (((BasicType *)right->type)->getBasicType() == BT_FLOAT) {
				char *buffer = (char *)malloc((int)log10(((FloatNode *)right)->value) + 2);
				sprintf(buffer, "%f", ((FloatNode *)right)->value);
				((StrNode *)left)->value = buffer;
			}
			else if (((BasicType *)right->type)->getBasicType() == BT_BOOL) {
				if (((BoolNode *)right)->value)((StrNode *)left)->value = "true";
				else ((StrNode *)left)->value = "false";
			}
			else if (((BasicType *)right->type)->getBasicType() == BT_STRING) {
				((StrNode *)left)->value = ((StrNode *)right)->value;
			}
			else error("", VE_TYPEMISMATCH);
			break;
		}
		break;
	case sgs::VT_ARRAY:
		if (right->type->getVarType() == sgs::VT_ARRAY &&
			((ArrayType *)left->type)->getEleType() == ((ArrayType *)right->type)->getEleType())
			((ArrayNode *)left)->content = ((ArrayNode *)right)->content;
		else error("", VE_TYPEMISMATCH);
		break;
	case sgs::VT_CLASS:
		if (right->type->getVarType() == VT_CLASS &&
			((ClassType *)left->type)->getName() == ((ClassType *)right->type)->getName())
			((ClassNode *)left)->content = ((ClassNode *)right)->content;
		else error("", VE_TYPEMISMATCH);
		break;
	}
}
VarNode *SgsMachine::callFunc(FuncProto *func, vector<Expression *> paras) {
	string name = func->getName();
	SGSFUNC tmp = NULL;
	for (auto func : funcList) {
		if (name == func.first->getName()) {
			if (func.second) {
				addSymbol(new VarNode(func.first->getReturnType(), "result"));
				for (unsigned int i = 0; i < paras.size(); i++) {
					VarNode *tmp = new VarNode(
						func.first->getParam()[i].first, func.first->getParam()[i].second);
					assignValue(tmp, expValue(paras[i]));
					addSymbol(tmp);
				}
				vector<string> local = exeBlock(func.second->getBody());
				VarNode *res = findSymbol("result");
				removeLocal(local);
				return res;
			}
		}
	}
	for (auto dll : dllList) {
		if (tmp = (SGSFUNC)GetProcAddress(dll, nameReform(name).data())) {
			vector<VarNode *> list;
			for (auto exp : paras)list.push_back(expValue(exp));
			return tmp(list);
		}
	}
	return NULL;
}
vector<string> SgsMachine::exeBlock(BlockStmt *block) { //suspend.
	vector<string> local;
	return local;
}
VarNode *SgsMachine::getPointer(Expression *e) {
	switch (e->getExpType()) {
	case ET_IDENT:
		return findSymbol(((IdExp *)e)->getName());
	case ET_VISIT:
		return arrayElement(e);
	case ET_ACCESS:
		return classAttrib(e);
	default:
		return NULL;
	}
}
VarNode *SgsMachine::expValue(Expression *e) {
	switch (e->getExpType()) {
	case ET_OP:
		return binCalc(((OpExp *)e)->getLeft(), ((OpExp *)e)->getRight());
	case ET_LITERAL:
		switch (((LiteralExp *)e)->getType()->getVarType()) {
		case sgs::VT_BASIC:
			switch (((BasicType *)((LiteralExp *)e)->getType())->getBasicType()) {
			case BT_INT:
				return new IntNode(((IntLiteral *)e)->getValue(), "");
			case BT_FLOAT:
				return new FloatNode(((FloatLiteral *)e)->getValue(), "");
			case BT_BOOL:
				return new BoolNode(((BoolLiteral *)e)->getValue(), "");
			case BT_STRING:
				return new StrNode(((StrLiteral *)e)->getValue().data(), "");
			default:
				return NULL;
			}
		case sgs::VT_ARRAY:
			return new ArrayNode(((ArrayType *)((ArrayLiteral *)e)->getType())->getEleType(), 
				((ArrayType *)((ArrayLiteral *)e)->getType())->getLength(), "");
		case sgs::VT_CLASS:
			return new ClassNode(((ClassType *)((ClassLiteral *)e)->getType())->getEle(),
				((ClassType *)((ClassLiteral *)e)->getType())->getName(), "");
		default:
			return NULL;
		}
	case ET_CALL:
		return callFunc(((CallStmt *)e)->getFunction(), ((CallStmt *)e)->getParam());
	case ET_IDENT:
		return findSymbol(((IdExp *)e)->getName());
	case ET_VISIT:
		return arrayElement(e);
	case ET_ACCESS:
		return classAttrib(e);
	default:
		return NULL;
	}
}
VarNode *SgsMachine::binCalc(Expression *a, Expression *b) {
	return NULL;
}
VarNode *SgsMachine::arrayElement(Expression *e) { //suspend.
	return NULL;
}
VarNode *SgsMachine::classAttrib(Expression *e) { //suspend.
	return NULL;
}

int SgsMachine::getInt(VarNode *val) {
	return ((IntNode *)val)->value;
}
float SgsMachine::getFloat(VarNode *val) {
	return ((FloatNode *)val)->value;
}
bool SgsMachine::getBool(VarNode *val) {
	return ((BoolNode *)val)->value;
}
const char *SgsMachine::getStr(VarNode *val) {
	return ((StrNode *)val)->value.data();
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