#ifndef MACHINE_H
#define MACHINE_H
#include <windows.h>
#include "syntax.h"

namespace sgs {
	class VarNode {
	public:
		VarType *type;
		string name;

		VarNode(VarType *t, string n) : type(t), name(n) {}
	};
	class IntNode : public VarNode {
	public:
		int value;

		IntNode(int v, string n) : VarNode(new BasicType(BT_INT), n), value(v) {}
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

#ifndef SGS_DLL
class Symbol {
public:
	sgs::VarNode *var;
	Symbol *next;

	Symbol(sgs::VarNode *var) : var(var) {}
};

typedef sgs::VarNode *(*SGSFUNC)(vector<sgs::VarNode *> param);

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
	vector<HINSTANCE> dllList;

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

#endif