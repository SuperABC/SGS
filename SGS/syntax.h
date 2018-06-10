#ifndef SYNTAX_H
#define SYNTAX_H
#include "lexeme.h"
#include "memory.h"
#include <cstdio>
#include <stack>
#include <memory>
#include <utility>

using std::unique_ptr;

namespace sgs {

	enum AST_TYPE {
		AT_VARDEF,
		AT_CLASS,
		AT_EXP,
		AT_STMT,
		AT_PROTO,
		AT_FUNC
	};
	class AST {
	public:
        virtual ~AST() = default;
		AST_TYPE astType;
		AST(AST_TYPE t) :astType(t) {}
	};

	enum VAR_TYPE {
		VT_BASIC,
		VT_ARRAY,
		VT_CLASS
	};

	class VarType {
	private:
		VAR_TYPE varType;
	public:
        virtual ~VarType() = default;
		VarType(VAR_TYPE t) : varType(t) {}
		VAR_TYPE getVarType() const { return varType; }
	};
	enum BASIC_TYPE {
		BT_INT,
		BT_FLOAT,
		BT_BOOL,
		BT_CHAR,
		BT_STRING
	};
	class BasicType : public VarType {
	private:
		BASIC_TYPE basicType;
	public:
		BasicType(BASIC_TYPE t) :VarType(VT_BASIC), basicType(t) {}
		BASIC_TYPE getBasicType() const { return basicType; }
	};
	class ArrayType : public VarType {
	private:
		VarType * eleType;
		int length;
	public:
		ArrayType(VarType *t, int l) : VarType(VT_ARRAY), eleType(t), length(l) {}
		VarType *getEleType() const { return eleType; }
		int getLength() const { return length; }
	};

	class ClassType : public VarType {
	private:
		string className;
		vector <std::pair<VarType *, string>> eleList;
	public:
		explicit ClassType(string n, vector<std::pair<VarType *, string>> list = vector <std::pair<VarType *, string>>()) :
			VarType(VT_CLASS), className(std::move(n)), eleList(std::move(list)) {}
		void pushEle(VarType *t, string n) {
			eleList.emplace_back(t, n);
		}
		string getName() const { return className; }
		vector <std::pair<VarType *, string>> getEle() const { return eleList; }
	};
	class VarDef : public AST {
	private:
		VarType * decType;
		string name;
	public:
		VarDef(VarType* t, string n) : AST(AT_VARDEF), decType(t), name(std::move(n)) {}
		VarType *getDecType() const { return decType; }
		string getName() const { return name; }
	};

	class ClassDef : public AST {
	private:
		ClassType * decType;
	public:
		ClassDef(ClassType* t) : AST(AT_CLASS), decType(t) {}
		ClassType *getDecType() const { return decType; }
	};

	enum EXP_TYPE {
		ET_OP,
		ET_LITERAL,
		ET_IDENT,
		ET_VISIT,
		ET_CALL,
		ET_ACCESS
	};
	class Expression : public AST {
	private:
		EXP_TYPE expType;
	public:
		Expression(EXP_TYPE t) : AST(AT_EXP), expType(t) {}
		EXP_TYPE getExpType() const { return expType; }
	};
	class OpExp : public Expression {
	private:
		SGSOPERATOR op;
		Expression *left, *right;
	public:
		explicit OpExp(SGSOPERATOR op) : Expression(ET_OP), op(op), left(nullptr), right(nullptr) {}
		void setLeft(Expression *l) { left = l; }
		void setRight(Expression *r) { right = r; }
		Expression *getLeft() const { return left; }
		Expression *getRight() const { return right; }
		SGSOPERATOR getOp() const { return op; }
	};
	class LiteralExp : public Expression {
		VarType *type;
	public:
		explicit LiteralExp(VarType *type) : Expression(ET_LITERAL), type(type) {}
		VarType *getType() const { return type; }
	};
	class IntLiteral : public LiteralExp {
		int value;
	public:
		explicit IntLiteral(int value = 0) : LiteralExp(new BasicType(BT_INT)), value(value) {}
		int getValue() const { return value; }
	};
    class CharLiteral : public LiteralExp {
        char value;
    public:
        explicit CharLiteral(char value = '\0') : LiteralExp(new BasicType(BT_CHAR)), value(value) {}
        char getValue() const { return value; }
    };
	class FloatLiteral : public LiteralExp {
		float value;
	public:
		explicit FloatLiteral(float value = 0) : LiteralExp(new BasicType(BT_FLOAT)), value(value) {}
		float getValue() const { return value; }
	};
	class BoolLiteral : public LiteralExp {
		bool value;
	public:
		explicit BoolLiteral(bool value = false) : LiteralExp(new BasicType(BT_BOOL)), value(value) {}
		bool getValue() const { return value; }
	};
	class StrLiteral : public LiteralExp {
		string value;
	public:
		explicit StrLiteral(string value = "") : LiteralExp(new BasicType(BT_STRING)), value(std::move(value)) {}
		string getValue() const { return value; }
	};
	class ArrayLiteral : public LiteralExp {
		vector<Expression *> cont;
	public:
		explicit ArrayLiteral(VarType *t, vector<Expression *> cont) : 
			LiteralExp(new ArrayType(t, cont.size())), cont(cont) {}
		vector<Expression *> getValue() const { return cont; }
	};
	class ClassLiteral : public LiteralExp {
		vector<Expression *> cont;
	public:
		explicit ClassLiteral(string n, vector<std::pair<VarType *, string>> dec,
			vector<Expression *> cont) :
			LiteralExp(new ClassType(n, dec)), cont(std::move(cont)) {}
		vector<Expression *> getValue() const { return cont; }
	};
	class IdExp : public Expression {
	private:
		string name;
	public:
		explicit IdExp(string n) : Expression(ET_IDENT), name(std::move(n)) {}
		string getName() const { return name; }
	};
	class AccessExp : public Expression {
		Expression* object;
		string member;
	public:
		AccessExp(Expression* obj, string member) : Expression(ET_ACCESS), object(obj), member(std::move(member)) {}
		Expression* getObject() const { return object; }
		string getMember() const { return member; }
	};
	class VisitExp : public Expression {
	private:
		Expression *array;
		Expression *index;
	public:

		VisitExp(Expression* array, Expression* index) : Expression(ET_VISIT), array(array), index(index) {}
		Expression *getArray() const { return array; }
		Expression *getIndex() const { return index; }
	};
    class FuncProto;
	class CallExp : public Expression {
	private:
		FuncProto * function;
		vector <Expression *> paramList;
	public:
		CallExp(FuncProto *f, vector<Expression*> paramList) :
			Expression(ET_CALL), function(f), paramList(std::move(paramList)) {}
		void pushParam(Expression *e) { paramList.push_back(e); }
		FuncProto *getFunction() const { return function; };
		const vector <Expression *>& getParam() const { return paramList; };
	};

	enum STMT_TYPE {
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
	private:
		STMT_TYPE stmtType;
	public:
		explicit Statement(STMT_TYPE t) : AST(AT_STMT), stmtType(t) {}
		STMT_TYPE getStmtType() const { return stmtType; }
	};
	class AssignStmt : public Statement {
	private:
		Expression * left, *right;
	public:
		AssignStmt(Expression* left, Expression* right) : Statement(ST_ASSIGN), left(left), right(right) {}
		void setLeft(Expression *l) { left = l; }
		void setRigth(Expression *r) { right = r; }
		Expression *getLeft() const { return left; }
		Expression *getRight() const { return right; }
	};
	class BlockStmt : public Statement {    
	private:
		vector<AST *>content;
	public:
		BlockStmt() : Statement(ST_BLOCK) {}
		void pushAST(AST *t) { content.push_back(t); }
		const vector<AST *>& getContent() const { return content; }
	};
	class CallStmt : public Statement {
	private:
		FuncProto * function;
		vector <Expression *> paramList;
	public:
		explicit CallStmt(FuncProto *f, vector<Expression*> paramList) :
			Statement(ST_CALL), function(f), paramList(std::move(paramList)) {}
		void pushParam(Expression *e) { paramList.push_back(e); }
		FuncProto *getFunction() const { return function; };
		const vector <Expression *>& getParam() const { return paramList; };
	};
	class IfStmt : public Statement {
	private:
		Expression * condition;
		BlockStmt *taken, *untaken;
	public:
		IfStmt(Expression *b, BlockStmt* taken, BlockStmt* untaken) :
			Statement(ST_IF), condition(b), taken(taken), untaken(untaken) {}
		void setTaken(BlockStmt *t) { taken = t; }
		void setUntaken(BlockStmt *u) { untaken = u; }
		BlockStmt *getTaken() const { return taken; }
		BlockStmt *getUntaken() const { return untaken; }
		Expression* getCond() const { return condition; }
	};
	class WhileStmt : public Statement {
	private:
		Expression * condition;
		BlockStmt *body;
	public:
		WhileStmt(Expression *c, BlockStmt* body) : Statement(ST_WHILE), condition(c), body(body) {}
		void setBody(BlockStmt *b) { body = b; }
		BlockStmt *getBody() const { return body; }
		Expression* getCondition() const { return condition; }
	};
	class ReturnStmt : public Statement {
	public:
		ReturnStmt() : Statement(ST_RETURN) {}
	};
	class BreakStmt : public Statement {
	public:
		BreakStmt() : Statement(ST_BREAK) {}
	};
	class ContinueStmt : public Statement {
	public:
		ContinueStmt() : Statement(ST_CONTINUE) {}
	};

	class FuncProto : public AST {
	private:
		VarType * returnType;
		string name;
		vector <std::pair<VarType *, string>> paramList;
	public:
		FuncProto(VarType *ret, string n, vector<std::pair<VarType *, string>> list = vector<std::pair<VarType *, string>>()) :
			AST(AT_PROTO), returnType(ret), name(std::move(n)), paramList(std::move(list)) {}
		void pushParam(VarType *t, string n) {
			paramList.emplace_back(t, n);
		}
		VarType *getReturnType() const { return returnType; };
		string getName() const { return name; }
		const vector <std::pair<VarType *, string>>& getParam() const { return paramList; };
	};

	class FuncDef : public AST {
	private:
		FuncProto * proto;
		BlockStmt* body;
	public:
		FuncDef(FuncProto* p) : AST(AT_FUNC), proto(p), body(nullptr) {}
		FuncProto *getProto() const { return proto; }
		void setBody(BlockStmt *b) { body = b; }
		BlockStmt *getBody() const { return body; }
	};

	inline LiteralExp* getLiteral(int value = 0) {
		return new IntLiteral(value);
	}
	inline LiteralExp* getLiteral(float value = 0.f) {
		return new FloatLiteral(value);
	}
	inline LiteralExp* getLiteral(bool value = false) {
		return new BoolLiteral(value);
	}
	inline LiteralExp* getLiteral(char *value = nullptr) {
		return new StrLiteral(value);
	}
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
	vector<SgsTokenPrim> content;
	vector<string> strId;

	SgsMemory synMem;

	unsigned int proc;
	int func = -1;

	void prepare();
	void skipLine();

	void parseLib(string lib);
	sgs::Expression *parseExp();
	sgs::Expression *parseVar();
	sgs::ClassDef *parseClassDec();
	sgs::ClassLiteral *parseClassConst(int classid);
	sgs::FuncProto *parseFuncDec();
	sgs::FuncDef *parseFuncDef(int funcid);
	vector<sgs::Expression *>parseParam(int funcid);
	sgs::BlockStmt *parseBlock(bool untaken = false);

	string parseUser(string guide = "");
	int parseUser(vector<string> guides);
	int findClass();
	int findFunc();

	void clearMem();

	static bool compare(int op1, int op2);

public:
	vector<sgs::AST *> stmts;

	vector<sgs::ClassType *>classList;
	vector<sgs::FuncProto *>funcList;

	vector<sgsMsg> msgList;

	SgsSyntax();
	SgsSyntax(vector<string> &ids, vector<SgsTokenPrim> &input);
	~SgsSyntax();

	SgsSyntax *input(vector<string> &ids, vector<SgsTokenPrim> &src);
	void parse();

	void error(const char *word, SGSYNTAXERROR type);
};

#endif