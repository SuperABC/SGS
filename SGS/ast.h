#pragma once

#include <utility>
#include <vector>
#include <string>
#include "TypeSys.h"
#include <map>
#include <fstream>
#include <llvm/IR/GlobalValue.h>


namespace sgs_backend {
    using namespace llvm;

	using std::vector;
	using std::string;
	using std::map;

	enum AST_TYPE {
		AT_TYPEDEF,
		// AT_EXP,
		AT_GLBVARDEF,
		AT_STMT,
		AT_FUNC,
		AT_PROTO
	};

	class AST {
	public:

        AST(const AST& other) = delete;

	    AST(AST&& other) noexcept
	        : astType(other.astType) {}

        AST& operator=(const AST& other) = delete;

	    AST& operator=(AST&& other) noexcept {
	        if (this == &other)
	            return *this;
	        astType = other.astType;
	        return *this;
	    }

		AST_TYPE astType;
		AST(AST_TYPE t) :astType(t) {}
		virtual ~AST() = default;
	};

	class TypeDef : public AST {
	private:
		SType * decType;
		string name;
	public:
		TypeDef(SType* t, string n) : AST(AT_TYPEDEF), decType(t), name(std::move(n)) {}
		SType *getDecType() const { return decType; }
		string getName() const { return name; }
	};

	enum EXP_TYPE {
		ET_BINOP,
		ET_LITERAL,
		ET_IDENT,
		ET_VISIT,
		ET_CALL,
		ET_ACCESS,
		ET_UNIOP,
		ET_CONSTR,
	};

	class Expression {
	private:
		EXP_TYPE expType;
		SType * resType;
	public:
		virtual ~Expression() = default;
		Expression(EXP_TYPE t, SType* v) : expType(t), resType(v) {}
		EXP_TYPE getExpType() const { return expType; }
		SType *getResType() const { return resType; }
		void setResType(SType *t) { resType = t; }
	};

	enum UNIOP {
		NOT,
		MINUS
	};

	class UniopExp : public Expression {
		UNIOP op;
		Expression* val;
	public:
		UniopExp(UNIOP op ,Expression* val, Context& context): Expression(ET_UNIOP, context.getBoolType()), op(op), val(val) {}
		UNIOP getOp() const { return op; }
		Expression* getVal() const { return val; }
	};

	enum BINOP {
		AND,
		OR,
		ADD,
		SUB,
		MUL,
		DIV,
		GT,
		LT,
        MOD,
        EQ,
	};

	SType* getBinopType(BINOP op, SType* lhs, SType* rhs, Context& context);

	class BinopExp : public Expression {
	private:
		BINOP op;
		Expression *left, *right;
	public:
		BinopExp(BINOP op, Expression* left, Expression* right, Context& context) : 
			Expression(ET_BINOP, getBinopType(op, left->getResType(), right->getResType(), context)), op(op), left(left), right(right) {}
		Expression *getLeft() const { return left; }
		Expression *getRigth() const { return right; }
		BINOP getOp() const { return op; }

	};

	SType* fromBasicType(BasicType tp, Context& context);

	class LiteralExp : public Expression {
		BasicType btype;
	public:
		explicit LiteralExp(BasicType type, Context& context) : Expression(ET_LITERAL, fromBasicType(type, context)), btype(type) {}
		BasicType getBType() const {
			return btype;
		}
	};

	class IntLiteral : public LiteralExp {
		int value;
	public:
		explicit IntLiteral(int value, Context& context) : LiteralExp(BasicType::INTEGER, context), value(value) {}
		int getValue() const { return value; }
	};

	class CharLiteral : public LiteralExp {
		char value;
	public:
		explicit CharLiteral(char value , Context& context) : LiteralExp(BasicType::CHAR, context), value(value) {}
		char getValue() const {
			return value;
		}
	};

	class BoolLiteral : public LiteralExp {
		bool value;
	public:
		explicit BoolLiteral(bool value, Context& context) : LiteralExp(BasicType::BOOLEAN, context), value(value) {}
		bool getValue() const { return value; }
	};

	class FloatLiteral : public LiteralExp {
		float value;
	public:
		explicit FloatLiteral(float value, Context& context) : LiteralExp(BasicType::FLOAT, context), value(value) {}
		float getValue() const { return value; }
	};

	// class StrLiteral : public LiteralExp {
	// 	string value;
	// public:
	// 	explicit StrLiteral(string value = "") : LiteralExp(BT_STRING), value(std::move(value)) {}
	// 	string getValue() const { return value; }
	// };

	class ConstString : public Expression {
		string str;
	public:
		ConstString(string str) : Expression(ET_CONSTR, nullptr), str(std::move(str)) {}
		const string& getStr() const {
			return str;
		}
	};

	inline LiteralExp* getLiteral(int value, Context& context) {
		return new IntLiteral(value, context);
	}

	inline LiteralExp* getLiteral(bool value, Context& context) {
		return new BoolLiteral(value, context);
	}

	inline LiteralExp* getLiteral(double value, Context& context) {
		return new FloatLiteral(value, context);
	}

	inline LiteralExp* getLiteral(char value, Context& context) {
		return new CharLiteral(value, context);
	}

	class IdExp : public Expression {
		string name;
	public:
		explicit IdExp(string n, SType* type) : Expression(ET_IDENT, type), name(std::move(n)) {}
		string getName() const {
			return name;
		}
	};

	inline SType* getAccessType(SType* tp, const string& str) {
		return dynamic_cast<STupleType*>(tp)->getElemType(str);
	}

	class AccessExp : public Expression {
		Expression* object;
		string member;
	public:
		AccessExp(Expression* obj, const string& member) : 
			Expression(ET_ACCESS, getAccessType(obj->getResType(), member)), 
			object(obj),  
			member(member) {}
		Expression* getObject() const { return object; }
		string getMember() const { return member; }
	};

	class VisitExp : public Expression {
	private:
		Expression * array;
		Expression * index;
	public:

		VisitExp(Expression* array, Expression* index) : Expression(ET_VISIT, (dynamic_cast<SArrayType*>(array->getResType())->getElementType())), array(array), index(index) {}
		Expression *getArray() const { return array; }
		Expression *getIndex() const { return index; }
	};


	enum STMT_TYPE {
		ST_ASSIGN,
		// ST_CALL,
		ST_IF,
		ST_WHILE,
		ST_RETURN,
		ST_BREAK,
		ST_CONTINUE,
		ST_BLOCK,
		ST_EXP,
		ST_VARDEF
	};

	class Statement : public AST {
		STMT_TYPE stmtType;
	public:
		explicit Statement(STMT_TYPE t) : AST(AT_STMT), stmtType(t) {}
		STMT_TYPE getStmtType() const { return stmtType; }
	};

	class ExpStmt : public Statement {
		Expression* exp;
	public:
		explicit ExpStmt(Expression* exp) : Statement(ST_EXP), exp(exp) {}
		Expression* getExp() const { return exp; }
	};

	class AssignStmt : public Statement {
		Expression * left, *right;
	public:
		AssignStmt(Expression* left, Expression* right) : Statement(ST_ASSIGN), left(left), right(right) {}
		Expression *getLeft() const { return left; }
		Expression *getRigth() const { return right; }
	};

	class BlockStmt : public Statement {
		vector<Statement *> content;
	public:
		BlockStmt(vector<Statement*> content) : Statement(ST_BLOCK), content(std::move(content)) {}
		vector<Statement *>& getContent() { return content; }
	};

	class GlobalVarDef : public AST {
		string name;
		SType* type;
		LiteralExp* initValue;

	public:
		GlobalVarDef(string name, SType* type, LiteralExp* initValue = nullptr)
			: AST(AT_GLBVARDEF),
			  name(std::move(name)),
			  type(type),
			  initValue(initValue) {}

		const string& getName() const { return name; }
		SType* getType() const { return type; }
		LiteralExp* getInitValue() const { return initValue; }
	};

	class FuncProto : public AST {
	private:
		SType * returnType;
		string name;
		vector <std::pair<SType *, string>> paramList;
	public:
		FuncProto(SType *ret, string funcName, vector<pair<SType*, string>> paramList) : AST(AT_PROTO), returnType(ret), name(std::move(funcName)), paramList(std::move(paramList)) {}
		void pushParam(SType *t, string n) {
			paramList.emplace_back(t, n);
		}
		FunctionType* getLLVMType(LLVMContext& context, const map<string, Type*>& typeReference) const {
			vector<Type*> res;
			for (const auto& x : paramList) {
				res.push_back(getParamType(x.first, context, typeReference));
			}
			return FunctionType::get(returnType->toLLVMType(context, typeReference), res, false);
		}

		SType *getReturnType() const { return returnType; };
		string getName() const { return name; }
		const vector <std::pair<SType *, string>>& getParam() const { return paramList; };
	};

	class FuncDef : public AST {
	private:
		FuncProto * proto;
		BlockStmt* body;
	public:
		FuncDef(FuncProto* p, BlockStmt* body) : AST(AT_FUNC), proto(p), body(body) {}
		FuncProto *getProto() const { return proto; }
		void setBody(BlockStmt *b) { body = b; }
		BlockStmt *getBody() const { return body; }
	};

	// class CallStmt : public Statement {
	// private:
	// 	FuncDef * function;
	// 	vector <Expression *> paramList;
	// public:
	// 	explicit CallStmt(FuncDef *f, vector<Expression*> paramList) :
	// 		Statement(ST_CALL), function(f), paramList(std::move(paramList)) {
	// 	}
	// 	void pushParam(Expression *e) { paramList.push_back(e); }
	// 	FuncDef *getFunction() const { return function; };
	// 	const vector <Expression *>& getParam() const { return paramList; };
	// };

	// inline SType* getFuncResType(SType*)

	class CallExp : public Expression {
	private:
		string caller;
		vector <Expression *> paramList;
	public:
		CallExp(string f, vector<Expression*> paramList, SType* type) :
			Expression(ET_CALL, type), caller(std::move(f)), paramList(std::move(paramList)) {
		}
		void pushParam(Expression *e) { paramList.push_back(e); }
		string getFunction() const { return caller; };
		const vector <Expression *>& getParam() const { return paramList; };
	};

	class IfStmt : public Statement {
		Expression * condition;
		Statement *taken, *untaken;
	public:
		IfStmt(Expression *b, Statement* taken, Statement* untaken) :
			Statement(ST_IF), condition(b), taken(taken), untaken(untaken) {
		}
		void setTaken(Statement *t) { taken = t; }
		void setUntaken(Statement *u) { untaken = u; }
		Statement *getPass() const { return taken; }
		Statement *getFail() const { return untaken; }
		Expression* getCond() const { return condition; }
	};

	class WhileStmt : public Statement {
	private:
		Expression * condition;
		BlockStmt * body;
	public:
		WhileStmt(Expression *cond, BlockStmt* body) : Statement(ST_WHILE), condition(cond), body(body) {}
		BlockStmt *getBody() const { return body; }
		Expression* getCond() const { return condition; }
	};

	class VarDefStmt : public Statement {
		Expression* initValue;
		SType* varType;
		string name;
	public:

		VarDefStmt(SType* varType, Expression* initValue, string name)
			: Statement(ST_VARDEF),
			  initValue(initValue),
			  varType(varType),
			  name(std::move(name)) {}

		Expression* getInitValue() const { return initValue; }
		SType* getVarType() const { return varType; }
		string getName() const { return name; }
	};

	class ReturnStmt : public Statement {
		Expression* exp;
	public:
		explicit ReturnStmt(Expression* exp) : Statement(ST_RETURN), exp(exp) {}
		Expression* getRetVal() const { return exp; }
	};

	class BreakStmt : public Statement {
	public:
		BreakStmt() : Statement(ST_BREAK) {}
	};

	class ContinueStmt : public Statement {
	public:
		ContinueStmt() : Statement(ST_CONTINUE) {}
	};

	using Content = vector<AST*>;

	string printType(SType* type, const string& name);
	string printExpression(Expression* expr);
	string printStatement(Statement* stmt);
	string printAST(AST* ast);
	static int printCounter;

    void recursiveDeleteExpression(Expression* exp);

    void recursiveDeleteStatement(Statement* stmt);

    void recursiveDeleteAST(AST* ast);

    void recursiveDelete(const Content& content);

	void printContentInDot(const Content& content, const string& filename);
}
