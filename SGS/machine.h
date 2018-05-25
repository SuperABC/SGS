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

		FloatNode(float v, string n) : VarNode(new BasicType(BT_FLOAT), n), value(v) {}
	};
	class BoolNode : public VarNode {
	public:
		bool value;

		BoolNode(bool v, string n) : VarNode(new BasicType(BT_BOOL), n), value(v) {}
	};
	class StrNode : public VarNode {
	public:
		string value;

		StrNode(const char *v, string n) : VarNode(new BasicType(BT_STRING), n), value(v) {}
	};
	class ArrayNode : public VarNode {
	public:
		vector<VarNode *> content;

		ArrayNode(VarType *t, int length, string n) : 
			VarNode(new ArrayType(t,length), n), content(vector<VarNode *>(length)) {}
	};
	class ClassNode : public VarNode {
	public:
		vector<VarNode *> content;

		ClassNode(vector <std::pair<VarType *, string>> ele, string cn, string n) :
			VarNode(new ClassType(cn, ele), n), content(vector<VarNode *>(ele.size())) {}
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

	Symbol *table[256] = { NULL };
	std::stack<string> stack;

	SgsMemory macMem;

	void *env;
	void initModule();
	void loadDlls();

	void addSymbol(sgs::VarNode *var);
	sgs::VarNode *findSymbol(string name);
	void removeLocal(string local, bool del = true);

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
	sgs::VarNode *binCalc(sgs::Expression *a, sgs::Expression *b);
	sgs::VarNode *arrayElement(sgs::Expression *e);
	sgs::VarNode *classAttrib(sgs::Expression *e);

	int getInt(sgs::VarNode *val);
	float getFloat(sgs::VarNode *val);
	bool getBool(sgs::VarNode *val);
	const char *getStr(sgs::VarNode *val);
public:
	vector<sgsMsg> msgList;

	SgsMachine();
	~SgsMachine();

	SgsMachine *input(vector<sgs::AST *> s,
		vector<sgs::ClassType *> c, vector<sgs::FuncProto *> f);
	void execute();
	sgs::VarNode *execute(sgs::BlockStmt *block);
	void environment(void *env);

	void clearMem();
	void error(const char *inst, SGSVMERROR type);
};
#endif

#endif