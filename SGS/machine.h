#ifndef MACHINE_H
#define MACHINE_H
#include "syntax.h"

namespace sgs {
	class VarNode {
	public:
		VarType *type;
		string name;
	};
	class IntNode : public VarNode {
	public:
		int value;
	};
	class FloatNode : public VarNode {
	public:
		float value;
	};
	class BoolNode : public VarNode {
	public:
		bool value;
	};
	class StrNode : public VarNode {
	public:
		string value;
	};
	class ArrayNode : public VarNode {
	public:
		vector<VarNode *> content;
	};
	class ClassNode : public VarNode {
	public:
		vector<VarNode *> content;
	};
}

class Symbol {
public:
	sgs::VarNode *var;
	Symbol *next;

	Symbol(sgs::VarNode *var) : var(var) {}
};

enum SGSVMERROR {
	VE_DIVBYZERO,
	VE_TYPEMISMATCH,
	VE_BROKEN
};
class SgsMachine {
private:
	vector<sgs::AST *> stmts;

	vector<sgs::ClassType *>classList;
	vector<std::pair<sgs::FuncProto *, sgs::FuncDef *>>funcList;

	Symbol *table[256];

	SgsMemory macMem;

	void *env;
	void initModule();
	void loadDlls();

	void addSymbol(sgs::VarNode *var);
	sgs::VarNode *findSymbol(string name);
	void removeLocal(vector<string> local);

	void step(sgs::AST *s);
	void declare(sgs::AST *s);
	void structure(sgs::AST *s);
	void statement(sgs::AST *s);
	void prototype(sgs::AST *s);
	void definition(sgs::AST *s);

	void assignValue(sgs::VarNode *left, sgs::VarNode *right);
	sgs::VarNode *callFunc(sgs::FuncProto *func, vector<sgs::Expression *> paras);
	void exeBlock(sgs::BlockStmt *block);
	sgs::VarNode *getPointer(sgs::Expression *e);
	sgs::VarNode *expValue(sgs::Expression *e);
	sgs::VarNode *arrayElement(sgs::Expression *e);
	sgs::VarNode *classAttrib(sgs::Expression *e);

	sgs::IntLiteral *getInt(sgs::VarNode *val);
	sgs::FloatLiteral *getFloat(sgs::VarNode *val);
	sgs::BoolLiteral *getBool(sgs::VarNode *val);
	sgs::StrLiteral *getStr(sgs::VarNode *val);
public:
	vector<sgsMsg> msgList;

	SgsMachine();
	~SgsMachine();

	SgsMachine *input(vector<sgs::AST *> s,
		vector<sgs::ClassType *> c, vector<sgs::FuncProto *> f);
	void execute();
	void environment(void *env);

	void clearMem();
	void error(const char *inst, SGSVMERROR type);
};

#endif