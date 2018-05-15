#ifndef SYNTAX_H
#define SYNTAX_H
#include "lexeme.h"
#include "memory.h"
#include <cstdio>
#include <stack>

class stateSeq;
class stateNode;

class expNode;
class varDec;
class varNode;
class primDec;
class primNode;
class classDec;
class classNode;
class arrayDec;
class arrayNode;
class funcDec;
class funcNode;

enum stateType {
	VO_NULL,
	VO_ASSIGN,
	VO_CALL,
	VO_IF,
	VO_WHILE
};
class stateSeq {
private:
	enum stateType op;
	void *left, *right;
public:
	varNode *leftValue() {
		if (op != VO_ASSIGN)return NULL;
		return (varNode *)left;
	}
	varNode *rightValue() {
		if (op != VO_ASSIGN)return NULL;
		return (varNode *)right;
	}
	funcDec *funcName() {
		if (op != VO_CALL)return NULL;
		return (funcDec *)left;
	}
	classNode *funcPara() {
		if (op != VO_CALL)return NULL;
		return (classNode *)right;
	}
	varNode *ifCondition() {
		if (op == VO_IF)return NULL;
		return (varNode *)left;
	}
	stateNode *ifTaken() {
		if (op == VO_IF)return NULL;
		return (stateNode *)((expNode *)right)->left;
	}
	stateNode *ifUntaken() {
		if (op == VO_IF)return NULL;
		return (stateNode *)((expNode *)right)->right;
	}
	varNode *loopCondition() {
		if (op == VO_WHILE)return NULL;
		return (varNode *)left;
	}
	stateNode *loopBlock() {
		if (op == VO_WHILE)return NULL;
		return (stateNode *)right;
	}
};
class stateNode : public expNode {
	vector<stateSeq> statements;
};

class expNode {
public:
	SGSOPERATOR op;
	expNode *left, *right;
};
enum decType {
	SGS_DT_INTEGER,
	SGS_DT_CHAR,
	SGS_DT_STRING,
	SGS_DT_FLOAT,
	SGS_DT_BOOL,
	SGS_DT_EXP,
	SGS_DT_CLASS,
	SGS_DT_ARRAY,
	SGS_DT_FUNCTION,
	SGS_DT_NULL
};
class varDec {
public:
	decType type;
	string name;
};
class varNode : public expNode {
public:
	varDec declaration;
	string name;
};
class primDec :public varDec {

};
class primNode : public varNode {
	void *value;
};
class classDec : public varDec {
	vector<varDec> varList;
};
class classNode : public varNode {
public:
	vector<varNode> varList;
	classNode() {}
	classNode(classDec t, string name) {
		this->declaration = t;
		this->name = name;
	}
};
class arrayDec : public varDec {
public:
	varDec type;
};
class arrayNode : public varNode {
	vector<varNode> content;
};
class funcDec : public varDec {
public:
	vector<varNode> varList;
	funcDec() {}
	funcDec(string name, vector<varNode> vars = vector<varNode>()) {
		this->name = name;
		varList = vars;
	}
};
class funcNode : public varNode {
public:
	vector<stateSeq> content;
	vector<varNode> localVar;
	funcNode() {}
	funcNode(funcDec dec) { declaration = dec; }
};

class SgsSyntax {
private:
	vector<sgsTokenPrim> content;
	vector<string> strId;

	SgsMemory synMem;

	unsigned int proc;
	int func = -1;

	void prepare();

	void parseLib(string lib);
	expNode *parseExp();
	funcNode *parseFuncDec();
	stateNode *parseFuncDef(int funcid);
	classNode *parseParam(int funcid);
	stateNode *parseBlock();
	classDec *parseClassDec();

	int findType();
	int findVar();
	int findClass();
	int findFunc();
	int findArray();

	void clearMem();

	static bool compare(int op1, int op2);

public:
	vector<varDec *> globeDec;
	vector<varNode *> globeVar;

	stateNode *output;

	SgsSyntax();
	SgsSyntax(vector<string> &ids, vector<sgsTokenPrim> &input);
	~SgsSyntax();

	SgsSyntax *input(vector<string> &ids, vector<sgsTokenPrim> &src);
	stateNode *parse();

	static void error(const char *inst, int type);
};

enum SGSYNTAXERROR {
	SGS_SE_EXPOSE,
	SGS_SE_UNIQUE,
	SGS_SE_EXPDOT,
	SGS_SE_EXPCOMMA,
	SGS_SE_EXPBRACE,
	SGS_SE_REDEF,
	SGS_SE_INVALIDTYPE,
	SGS_SE_DISACCORD,
	SGS_SE_NOID,
	SGS_SE_INCOMPLETE,
	SGS_SE_UNKNOWN,
	SGS_SE_UNSUPPORT
};
class SGSyntaxException {
private:
	std::string msg;
public:
	SGSyntaxException(std::string s) {
		msg = s;
	}
	const char *message() { return msg.data(); }
};
#endif