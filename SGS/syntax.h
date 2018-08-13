#ifndef SYNTAX_H
#define SYNTAX_H
#include "lexeme.h"
#include <cstdio>
#include <stack>
#include <memory>
#include <utility>

namespace sgs {

	enum AST_TYPE { //AST的类型
		AT_VARDEF,
		AT_CLASS,
		AT_EXP,
		AT_STMT,
		AT_PROTO,
		AT_FUNC
	};
	class AST { //AST类
	public:
		int line;

		AST_TYPE astType;
		AST(int line, AST_TYPE t) : line(line), astType(t) {}
		virtual ~AST() = default;
	};

	class FuncProto;
	class FuncDef;
	enum VAR_TYPE { //变量的类型
		VT_BASIC,
		VT_ARRAY,
		VT_CLASS
	};
	class VarType { //变量类
	private:
		VAR_TYPE varType;
	public:
		VarType(VAR_TYPE t) : varType(t) {}
		VAR_TYPE getVarType() const { return varType; }
		virtual ~VarType() = default;
	};
	enum BASIC_TYPE { //基本变量的类型
		BT_INT,
		BT_FLOAT,
		BT_BOOL,
		BT_CHAR,
		BT_STRING
	};
	class BasicType : public VarType { //基本变量类
	private:
		BASIC_TYPE basicType;
	public:
		BasicType(BASIC_TYPE t) :VarType(VT_BASIC), basicType(t) {}
		BASIC_TYPE getBasicType() const { return basicType; }
	};
	class ArrayType : public VarType { //数组变量类
	private:
		VarType * eleType;
		int length;
	public:
		ArrayType(VarType *t, int l) : VarType(VT_ARRAY), eleType(t), length(l) {}
		VarType *getEleType() const { return eleType; }
		int getLength() const { return length; }
		void setLength(int l) { length = l; }
	};
	class ClassType : public VarType { //类变量类
	private:
		string className;
		vector <std::pair<VarType *, string>> eleList;
		vector <std::pair<sgs::FuncProto *, sgs::FuncDef *>> constructor;
	public:
		explicit ClassType(string n, vector<std::pair<VarType *, string>> list = vector <std::pair<VarType *, string>>()) :
			VarType(VT_CLASS), className(std::move(n)), eleList(std::move(list)) {}
		void pushEle(VarType *t, string n) {
			eleList.emplace_back(t, n);
		}
		void decConstructor(sgs::FuncProto *c) {
			constructor.push_back(std::pair<sgs::FuncProto *, sgs::FuncDef *>(c, NULL));
		}
		std::pair<sgs::FuncProto *, sgs::FuncDef *> latestConstructor() {
			return constructor.back();
		}
		void defConstructor(sgs::FuncDef *c) {
			constructor.back().second = c;
		}
		string getName() const { return className; }
		vector <std::pair<VarType *, string>> getEle() const { return eleList; }
	};
	
	class VarDef : public AST { //变量声明AST
	private:
		VarType * decType;
		string name;
	public:
		VarDef(int line, VarType* t, string n) : AST(line, AT_VARDEF), decType(t), name(std::move(n)) {}
		VarType *getDecType() const { return decType; }
		string getName() const { return name; }
	};
	class ClassDef : public AST { //类声明AST
	private:
		ClassType * decType;
	public:
		ClassDef(int line, ClassType* t) : AST(line, AT_CLASS), decType(t) {}
		ClassType *getDecType() const { return decType; }
	};

	enum EXP_TYPE { //表达式类型
		ET_OP,
		ET_LITERAL,
		ET_IDENT,
		ET_VISIT,
		ET_CALL,
		ET_ACCESS
	};
	class Expression : public AST { //表达式AST
	private:
		EXP_TYPE expType;
	public:
		Expression(int line, EXP_TYPE t) : AST(line, AT_EXP), expType(t) {}
		EXP_TYPE getExpType() const { return expType; }
	};
	class OpExp : public Expression { //运算符表达式
	private:
		OPERATOR op;
		Expression *left, *right;
	public:
		explicit OpExp(int line, OPERATOR op) : Expression(line, ET_OP), op(op), left(nullptr), right(nullptr) {}
		void setLeft(Expression *l) { left = l; }
		void setRight(Expression *r) { right = r; }
		Expression *getLeft() const { return left; }
		Expression *getRight() const { return right; }
		OPERATOR getOp() const { return op; }
	};
	class LiteralExp : public Expression { //字面量表达式
		VarType *type;
	public:
		explicit LiteralExp(int line, VarType *type) : Expression(line, ET_LITERAL), type(type) {}
		VarType *getType() const { return type; }
	};
	class IntLiteral : public LiteralExp { //整数字面量表达式
		int value;
	public:
		explicit IntLiteral(int line, int value = 0) : LiteralExp(line, new BasicType(BT_INT)), value(value) {}
		int getValue() const { return value; }
	};
	class CharLiteral : public LiteralExp { //字符字面量表达式
		char value;
	public:
		explicit CharLiteral(int line, char value = '\0') : LiteralExp(line, new BasicType(BT_CHAR)), value(value) {}
		char getValue() const { return value; }
	};
	class FloatLiteral : public LiteralExp { //浮点数字面量表达式
		float value;
	public:
		explicit FloatLiteral(int line, float value = 0) : LiteralExp(line, new BasicType(BT_FLOAT)), value(value) {}
		float getValue() const { return value; }
	};
	class BoolLiteral : public LiteralExp { //布尔字面量表达式
		bool value;
	public:
		explicit BoolLiteral(int line, bool value = false) : LiteralExp(line, new BasicType(BT_BOOL)), value(value) {}
		bool getValue() const { return value; }
	};
	class StrLiteral : public LiteralExp { //字符串字面量表达式
		string value;
	public:
		explicit StrLiteral(int line, string value = "") : LiteralExp(line, new BasicType(BT_STRING)), value(std::move(value)) {}
		string getValue() const { return value; }
	};
	class ArrayLiteral : public LiteralExp { //数组字面量表达式
		vector<Expression *> cont;
	public:
		explicit ArrayLiteral(int line, VarType *t, vector<Expression *> cont) :
			LiteralExp(line, new ArrayType(t, cont.size())), cont(cont) {}
		vector<Expression *> getValue() const { return cont; }
	};
	class ClassLiteral : public LiteralExp { //类字面量表达式
		vector<Expression *> cont;
	public:
		explicit ClassLiteral(int line, string n, vector<std::pair<VarType *, string>> dec,
			vector<Expression *> cont) :
			LiteralExp(line, new ClassType(n, dec)), cont(std::move(cont)) {}
		vector<Expression *> getValue() const { return cont; }
	};
	class IdExp : public Expression { //标识符表达式
	private:
		string name;
	public:
		explicit IdExp(int line, string n) : Expression(line, ET_IDENT), name(std::move(n)) {}
		string getName() const { return name; }
	};
	class AccessExp : public Expression { //类成员表达式
		Expression* object;
		string member;
	public:
		AccessExp(int line, Expression* obj, string member) :
			Expression(line, ET_ACCESS), object(obj), member(std::move(member)) {}
		Expression* getObject() const { return object; }
		string getMember() const { return member; }
	};
	class VisitExp : public Expression { //数组元素表达式
	private:
		Expression * array;
		Expression *index;
	public:

		VisitExp(int line, Expression* array, Expression* index) :
			Expression(line, ET_VISIT), array(array), index(index) {}
		Expression *getArray() const { return array; }
		Expression *getIndex() const { return index; }
	};
	class CallExp : public Expression {  //函数调用表达式
	private:
		FuncProto * function;
		vector <Expression *> paramList;
	public:
		CallExp(int line, FuncProto *f, vector<Expression*> paramList) :
			Expression(line, ET_CALL), function(f), paramList(std::move(paramList)) {}
		void pushParam(Expression *e) { paramList.push_back(e); }
		FuncProto *getFunction() const { return function; };
		const vector <Expression *>& getParam() const { return paramList; };
	};

	enum STMT_TYPE { //语句的类型
		ST_ASSIGN,
		ST_CALL,
		ST_IF,
		ST_WHILE,
		ST_RETURN,
		ST_BREAK,
		ST_CONTINUE,
		ST_BLOCK
	};
	class Statement : public AST { //语句AST
	private:
		STMT_TYPE stmtType;
	public:
		explicit Statement(int line, STMT_TYPE t) : AST(line, AT_STMT), stmtType(t) {}
		STMT_TYPE getStmtType() const { return stmtType; }
	};
	class AssignStmt : public Statement { //赋值语句
	private:
		Expression * left, *right;
	public:
		AssignStmt(int line, Expression* left, Expression* right) : Statement(line, ST_ASSIGN), left(left), right(right) {}
		void setLeft(Expression *l) { left = l; }
		void setRigth(Expression *r) { right = r; }
		Expression *getLeft() const { return left; }
		Expression *getRight() const { return right; }
	};
	class BlockStmt : public Statement { //块语句
	private:
		vector<AST *>content;
	public:
		BlockStmt(int line) : Statement(line, ST_BLOCK) {}
		void pushAST(AST *t) { content.push_back(t); }
		const vector<AST *>& getContent() const { return content; }
	};
	class CallStmt : public Statement { //函数调用语句
	private:
		FuncProto * function;
		vector <Expression *> paramList;
	public:
		explicit CallStmt(int line, FuncProto *f, vector<Expression*> paramList) :
			Statement(line, ST_CALL), function(f), paramList(std::move(paramList)) {}
		void pushParam(Expression *e) { paramList.push_back(e); }
		FuncProto *getFunction() const { return function; };
		const vector <Expression *>& getParam() const { return paramList; };
	};
	class IfStmt : public Statement { //分支语句
	private:
		Expression * condition;
		BlockStmt *taken, *untaken;
	public:
		IfStmt(int line, Expression *b, BlockStmt* taken, BlockStmt* untaken) :
			Statement(line, ST_IF), condition(b), taken(taken), untaken(untaken) {}
		void setTaken(BlockStmt *t) { taken = t; }
		void setUntaken(BlockStmt *u) { untaken = u; }
		BlockStmt *getTaken() const { return taken; }
		BlockStmt *getUntaken() const { return untaken; }
		Expression* getCond() const { return condition; }
	};
	class WhileStmt : public Statement { //循环语句
	private:
		Expression * condition;
		BlockStmt *body;
	public:
		WhileStmt(int line, Expression *c, BlockStmt* body) : Statement(line, ST_WHILE), condition(c), body(body) {}
		void setBody(BlockStmt *b) { body = b; }
		BlockStmt *getBody() const { return body; }
		Expression* getCondition() const { return condition; }
	};
	class ReturnStmt : public Statement { //返回语句
	public:
		ReturnStmt(int line) : Statement(line, ST_RETURN) {}
	};
	class BreakStmt : public Statement { //跳出语句
	public:
		BreakStmt(int line) : Statement(line, ST_BREAK) {}
	};
	class RedoStmt : public Statement { //重做语句
	public:
		RedoStmt(int line) : Statement(line, ST_CONTINUE) {}
	};

	class FuncProto : public AST { //函数声明AST
	private:
		VarType * returnType;
		string name;
		vector <std::pair<VarType *, string>> paramList;
	public:
		FuncProto(int line, VarType *ret, string n, vector<std::pair<VarType *, string>> list = vector<std::pair<VarType *, string>>()) :
			AST(line, AT_PROTO), returnType(ret), name(std::move(n)), paramList(std::move(list)) {}
		void pushParam(VarType *t, string n) {
			paramList.emplace_back(t, n);
		}
		VarType *getReturnType() const { return returnType; };
		string getName() const { return name; }
		const vector <std::pair<VarType *, string>>& getParam() const { return paramList; };
	};
	class FuncDef : public AST { //函数定义AST
	private:
		FuncProto * proto;
		BlockStmt* body;
	public:
		FuncDef(int line, FuncProto* p) : AST(line, AT_FUNC), proto(p), body(nullptr) {}
		FuncProto *getProto() const { return proto; }
		void setBody(BlockStmt *b) { body = b; }
		BlockStmt *getBody() const { return body; }
	};

	inline LiteralExp* getLiteral(int line, int value = 0) {
		return new IntLiteral(line, value);
	}
	inline LiteralExp* getLiteral(int line, float value = 0.f) {
		return new FloatLiteral(line, value);
	}
	inline LiteralExp* getLiteral(int line, bool value = false) {
		return new BoolLiteral(line, value);
	}
	inline LiteralExp* getLiteral(int line, char *value = nullptr) {
		return new StrLiteral(line, value);
	}

	enum SYNTAXERROR {
		SGS_SE_NOLIB,
		SGS_SE_USELIB,
		SGS_SE_LIBNAME,
		SGS_SE_AFTERNEW,
		SGS_SE_EMPTYCLASS,
		SGS_SE_EXPTYPE,
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
	class Syntax {
	private:
		vector<TokenPrim> content;
		vector<string> strId;

		unsigned int proc;
		int func = -1;

		//语法解析准备
		void prepare();
		//跳过当前行
		void skipLine();

		//检测proc越界
		bool checkBoarder();
		//读取库文件
		void parseLib(string lib, int line);
		//读取右值表达式
		sgs::Expression *parseExp();
		//读取左值表达式
		sgs::Expression *parseVar();
		//读取类声明
		sgs::ClassDef *parseClassDec();
		//读取类字面量
		sgs::ClassLiteral *parseClassLiteral(int classid);
		//读取函数声明
		sgs::FuncProto *parseFuncDec();
		//读取函数定义
		sgs::FuncDef *parseFuncDef(int funcid);
		//读取构造器定义
		sgs::FuncDef *parseConstructorDef(int classid);
		//读取函数实参
		vector<sgs::Expression *>parseParam(int funcid);
		//读取构造器实参
		vector<sgs::Expression *>parseAttrib(int classid);
		//读取语句块
		sgs::BlockStmt *parseBlock(bool untaken = false);

		string parseUser(string guide = "");
		int parseUser(vector<string> guides);
		int findClass();
		int findFunc();

		static bool compare(int op1, int op2);

	public:
		vector<sgs::AST *> stmts;

		vector<sgs::ClassType *>classList;
		vector<sgs::FuncProto *>funcList;

		vector<sgsMsg> msgList;

		Syntax();
		Syntax(vector<string> &ids, vector<TokenPrim> &input);
		~Syntax();

		Syntax *input(vector<string> &ids, vector<TokenPrim> &src);
		void parse();

		void error(const char *word, SYNTAXERROR type, int line);
	};
}

#endif