#ifndef SYNTAX_H
#define SYNTAX_H
#include "lexeme.h"
#include <cstdio>

enum STATESTEP {
	SS_IDLE
};

struct varNode;
struct classType;
struct classNode;
struct funcType;
struct funcNode;
struct stateSeq;

enum valOp {
	VO_NULL,
	VO_ASSIGN,
	VO_EXE
};
struct rtVal {
	enum valOp op;
	varNode *left, *right;
};
struct stateSeq {
	rtVal act;
	stateSeq *next;
};

enum varType {
	VT_INTEGER,
	VT_CHAR,
	VT_STRING,
	VT_FLOAT,
	VT_BOOL,
	VT_ARRAY,
	VT_CLASS,
	VT_FUNCTION,
	VT_VAR
};
struct varNode {
	enum varType t;
	string name;
	string classType;
	void const *val;
	varNode *next;
	varNode() {}
	varNode(int type, string n, string str = "") : t((enum varType)type), name(n), classType(str), val(NULL) {};
};
struct classType {
	string name;
	vector<varType> varList;
	vector<classType> classList;
};
struct classNode {
	string name;
	vector<varNode> varList;
	vector<classNode> classList;
	classNode() {}
	classNode(classType t, string name) {
		this->name = name;
	}
};
struct funcType {
	string name;
	vector<varNode> varList;
	vector<classNode> classList;
	funcType() {}
	funcType(string name,
		vector<varNode> vars = vector<varNode>(),
		vector<classNode> classes = vector<classNode>()) {
		this->name = name;
		varList = vars;
		classList = classes;
	}
};
struct funcNode {
	funcType declare;
	stateSeq content;
	vector<varNode> localVar;
	funcNode() {}
	funcNode(funcType dec) { declare = dec; }
};

class Syntax {
private:
	vector<tokenPrim> content;
	vector<string> strId;

	enum STATESTEP step;
	unsigned int proc;
public:
	vector<classType> globeClassType;
	vector<funcNode> globeFunc;
	vector<varNode> globeVar;
	stateSeq *output;
	stateSeq *last;

	Syntax();
	Syntax(vector<string> ids, vector<tokenPrim> input);
	~Syntax();
	void prepare();

	static char *opStr(int id);
	static char *valueStr(float value);

	Syntax *input(vector<string> ids, vector<tokenPrim> src);
	stateSeq *parse();

	void parseLibrary(string lib);
	string parseUser();
	varNode *parseValue();
	funcNode parseFuncDec();
	stateSeq parseFuncDef(int funcid);
	varNode *parseParameter(int funcid);
	classType parseClassDec();
	classNode parseClassConstr();

	string findVar();
	int findClass();
	int findFunc();
	int findType();

	static void error(const char *inst, int type);
};

enum SYNTAXERROR {
	SE_UNIQUE,
	SE_EXPDOT,
	SE_EXPCOMMA,
	SE_EXPBRACE,
	SE_REDEF,
	SE_INVALIDTYPE,
	SE_DISACCORD,
	SE_NOID,
	SE_INCOMPLETE,
	SE_UNKNOWN,
	SE_UNSUPPORT
};
class SyntaxException {
private:
	std::string msg;
public:
	SyntaxException(std::string s) {
		msg = s;
	}
	const char *message() { return msg.c_str(); }
};
#endif