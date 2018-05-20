#ifndef SYNTAX_H
#define SYNTAX_H
#include "lexeme.h"
#include "memory.h"
#include <cstdio>
#include <stack>
#include <memory>

using std::unique_ptr;

namespace sgs {
	enum ASTTYPE {
		AT_STMT,
		AT_EXP
	};
	class AST {
	public:
		ASTTYPE type;
	};

	enum StmtType {
		ST_DEC,
		ST_ASSIGN,
		ST_CALL,
		ST_IF,
		ST_WHILE,
		ST_RETURN,
		ST_BREAK,
		ST_CONTINUE,
		ST_BLOCK
	};
	class Statement : public AST {
	public:
		StmtType type;
	};
	class DecStmt : public Statement {

	};
	class AssignStmt : public Statement {

	};
	class CallStmt : public Statement {

	};
	class IfStmt : public Statement {

	};
	class WhileStmt : public Statement {

	};
	class ReturnStmt : public Statement {

	};
	class BreakStmt : public Statement {

	};
	class ContinueStmt : public Statement {

	};
	class BlockStmt : public Statement {

	};

	enum EXPTYPE {
		ET_OP,
		ET_LETERAL,
		ET_ID
	};
	class Expression : public AST {
	public:
		EXPTYPE type;
	};
	class OpExp : public Expression {

	};
	class LiteralExp : public Expression {

	};
	class IdExp : public Expression {

	};

	enum DECTYPE {
		DT_BASIC,
		DT_ARRAY,
		DT_CLASS
	};
	class VarDec {
		DECTYPE type;
	};
	enum BASICTYPE {
		PT_INT,
		PT_FLOAT,
		PT_BOOL,
		PT_CHAR,
		PT_STRING
	};
	class BasicDec : public VarDec {
		BASICTYPE type;
	};
	class ArrayDec : public VarDec {
		VarDec *eleType;
		int length;
	};
	class ClassDec : public VarDec {
		string name;
		vector <std::pair<VarDec *, string>> eleList;
	};
	class FuncProto {
		VarDec *returnType;
		string name;
		vector <std::pair<VarDec *, string>> eleList;

		bool def = false;
	};
	class FuncDef : FuncProto {
		vector<AST> stmts;
	};

}

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
class SgsSyntax {
private:
	vector<sgsTokenPrim> content;
	vector<string> strId;

	SgsMemory synMem;

	unsigned int proc;
	int func = -1;

	void prepare();

	void skipLine();
	void parseLib(string lib);
	sgs::Expression *parseExp();
	sgs::FuncProto *parseFuncDec();
	sgs::FuncDef *parseFuncDef(int funcid);
	vector<sgs::AST *> parseBlock();
	sgs::ClassDec *parseClassDec();

	string parseUser();
	int findVar();
	int findType();
	int findClass();
	int findFunc();
	int findArray();

	void clearMem();

	static bool compare(int op1, int op2);

public:
	vector<sgs::AST *> stmts;
	vector<sgs::FuncProto *> funcs;

	vector<sgsMsg> msgList;

	SgsSyntax();
	SgsSyntax(vector<string> &ids, vector<sgsTokenPrim> &input);
	~SgsSyntax();

	SgsSyntax *input(vector<string> &ids, vector<sgsTokenPrim> &src);
	void parse();

	void error(const char *word, SGSYNTAXERROR type);
};

#endif