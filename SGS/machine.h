#ifndef MACHINE_H
#define MACHINE_H
#include <windows.h>
#include "syntax.h"

namespace sgs {
	class VarNode;
	class BoolNode; class CharNode; class IntNode; class FloatNode; class StrNode;
	class ArrayNode; class ClassNode;

	VarNode *createVar(VarType *type, string name);

	class VarNode {
	public:
		VarType *type;
		char *name;

		VarNode(VarType *t, string n) : type(t) {
			name = (char *)malloc(n.length() + 1);
			strcpy(name, n.data());
		}
		virtual ~VarNode() {
			free(name);
			delete type;
		}
	};
	class CharNode : public VarNode {
	public:
		char value;

		CharNode(char v, string n) : VarNode(new BasicType(BT_CHAR), n), value(v) {}
		CharNode(string n) : VarNode(new BasicType(BT_CHAR), n), value(0) {}
		virtual ~CharNode() = default;
	};
	class IntNode : public VarNode {
	public:
		int value;

		IntNode(int v, string n) : VarNode(new BasicType(BT_INT), n), value(v) {}
		IntNode(string n) : VarNode(new BasicType(BT_INT), n), value(0) {}
		virtual ~IntNode() = default;
	};
	class FloatNode : public VarNode {
	public:
		float value;

		FloatNode(float v, string n) : VarNode(new BasicType(BT_FLOAT), n), value(v) {}
		FloatNode(string n) : VarNode(new BasicType(BT_FLOAT), n), value(0.f) {}
		virtual ~FloatNode() = default;
	};
	class BoolNode : public VarNode {
	public:
		bool value;

		BoolNode(bool v, string n) : VarNode(new BasicType(BT_BOOL), n), value(v) {}
		BoolNode(string n) : VarNode(new BasicType(BT_BOOL), n), value(false) {}
		virtual ~BoolNode() = default;
	};
	class StrNode : public VarNode {
	public:
		char *value;

		StrNode(const char *v, string n) : VarNode(new BasicType(BT_STRING), n) {
			value = new char[strlen(v) + 1];
			strcpy(value, v);
		}
		StrNode(string n) : VarNode(new BasicType(BT_STRING), n) {
			value = new char[1];
		}
		virtual ~StrNode() = default;
	};
	class ArrayNode : public VarNode {
	public:
		vector<VarNode *> content;

		ArrayNode(VarType *t, int length, string n);
		virtual ~ArrayNode() = default;
		VarNode *operator [](int i) {
			return content[i];
		}

		class ArrayLength : public IntNode {
		public:
			ArrayNode *array;
			ArrayLength(ArrayNode *a, int v, string n) : IntNode(v, n), array(a) {}
			void assign(IntNode *v) {
				array->content.resize(v->value);
				for (auto &e : array->content) {
					e = createVar(((ArrayType *)array->type)->getEleType(), "");
				}
				((ArrayType *)array->type)->setLength(v->value);
			}
		};
		VarNode *attribute(string op) {
			if (op == "length")return new ArrayLength(this, content.size(), "");
			return NULL;
		}
	};
	class ClassNode : public VarNode {
	public:
		vector<VarNode *> content;

		ClassNode(vector <std::pair<VarType *, string>> ele, string cn, string n);
		VarNode *operator [](string m) {
			vector <std::pair<VarType *, string>> elements = ((ClassType *)type)->getEle();
			for (unsigned int i = 0; i < elements.size(); i++) {
				if (elements[i].second == m)return content[i];
			}
			return nullptr;
		}
		virtual ~ClassNode() = default;
	};

	class BreakNote {

	};
	class RedoNote {

	};
	class ReturnNote {

	};

}

#ifndef SGS_DLL

namespace sgs {
	class Symbol {
	public:
		sgs::VarNode *var;
		Symbol *next;

		Symbol(sgs::VarNode *var) : var(var), next(nullptr) {}
	};
	typedef sgs::VarNode *(*SGSFUNC)(int n, VarNode *param[]);

	enum SGSVMERROR {
		VE_DIVBYZERO,
		VE_NOID,
		VE_TYPEMISMATCH,
		VE_BROKEN
	};
	class Machine {
	private:
		vector<sgs::AST *> stmts;

		vector<sgs::ClassType *>classList;
		vector<sgs::FuncDef *>funcList;
		vector<HINSTANCE> dllList;

		Symbol *table[256] = { NULL };
		std::stack<string> stack;

		void initModule();
		void loadDlls();

		void addSymbol(VarNode *var);
		VarNode *findSymbol(string name);
		void removeLocal(string local, bool del = true);

		void step(AST *s);
		void declare(AST *s);
		void structure(AST *s);
		void statement(AST *s);
		void prototype(AST *s);
		void definition(AST *s);

		void assignValue(VarNode *left, VarNode *right);
		VarNode *callFunc(FuncProto *func, vector<Expression *> paras);
		void exeBlock(BlockStmt *block);
		VarNode *getPointer(Expression *e);
		VarNode *expValue(Expression *e);
		VarNode *binCalc(OPERATOR op, Expression *a, Expression *b);
		string getType(IdExp *id);
		bool sameType(VarType *t1, VarType *t2);
		VarNode *arrayElement(Expression *e);
		VarNode *classAttrib(Expression *e);

		int getInt(VarNode *val);
		float getFloat(VarNode *val);
		bool getBool(VarNode *val);
		char getChar(VarNode *val);
		const char *getStr(VarNode *val);
		
		void error(const char *inst, SGSVMERROR type, int line = 0);
	public:
		vector<sgsMsg> msgList;

		Machine();
		~Machine();

		Machine *input(vector<AST *> s,
			vector<ClassType *> c, vector<FuncProto *> f);
		void execute();

	};
}
#endif

#endif