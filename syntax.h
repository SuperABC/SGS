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
struct stateSeq;

enum valOp {
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
	VT_FLOAT,
	VT_BOOL,
	VT_CHAR,
	VT_STRING,
	VT_ARRAY,
	VT_CLASS,
	VT_FUNCTION
};
struct varNode {
	enum varType t;
	string name;
	string classType;
	void *val;
	varNode() {}
	varNode(int type, string n, string str = "") : t((enum varType)type), name(n), classType(str), val(NULL) {};
};
struct classType {
	string name;
	vector<varType> valList;
	vector<classType> classList;
	vector<funcType> funcList;
};
struct classNode {
	string name;
	vector<varNode> valList;
	vector<classNode> classList;
	vector<funcType> funcList;
};
struct funcType {
	string name;
	vector<varType> valList;
	vector<classType> classList;
	vector<funcType> funcList;
};
struct funcNode {
	funcType declare;
	stateSeq content;
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
	stateSeq output;
	stateSeq *last;

	Syntax();
	Syntax(vector<string> ids, vector<tokenPrim> input);
	~Syntax();

	static char *opStr(int id);
	static char *valueStr(float value);

	Syntax *input(vector<string> ids, vector<tokenPrim> src);
	stateSeq parse();

	void parseLibrary(string lib);
	varNode *parseValue();
	funcNode parseFuncDec();
	stateSeq parseFuncDef();
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