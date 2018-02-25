#ifndef LEXEME_H
#define LEXEME_H
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>

#define _CRT_SECURE_NO_WARNINGS
#define SYS_ID_NUM 37

using std::vector;
using std::string;

enum TOKENTYPE {
	TT_USER,
	TT_DATA,
	TT_OP,
	TT_SYS,
};
enum CONSTTYPE {
	CT_INT,
	CT_FLOAT,
	CT_BOOL,
	CT_CHAR,
	CT_STRING
};

enum IDENTIFIER {
	ID_INTEGER,
	ID_CHAR,
	ID_STRING,
	ID_FLOAT,
	ID_BOOL,
	ID_ARRAY,
	ID_CLASS,
	ID_FUNCTION,
	ID_NONE,

	ID_LET,
	ID_BE,
	ID_NEW,
	ID_WITH,
	ID_START,
	ID_END,
	ID_IF,
	ID_THEN,
	ID_ELSE,
	ID_LOOP,
	ID_WHEN,
	ID_OF,
	ID_IS,
	ID_ARE,
	ID_SMALLER,
	ID_LARGER,
	ID_THAN,
	ID_USE,
	ID_LIBRARY,
	ID_RESULT,
	ID_QUIT,
	ID_AND,
	ID_OR,
	ID_NOT,
	ID_XOR,
	ID_BREAK,
	ID_REDO,
	ID_COMMENT
};
enum OPERATOR {
	OP_PLUS,
	OP_PLUSPLUS,
	OP_EQPLUS,
	OP_MINUS,
	OP_MINUSMINUS,
	OP_EQMINUS,
	OP_NEG,
	OP_MULTY,
	OP_EQMULTY,
	OP_DIVIDE,
	OP_EQDIVIDE,
	OP_MOD,
	OP_EQMOD,
	OP_AND,
	OP_ANDAND,
	OP_EQAND,
	OP_OR,
	OP_OROR,
	OP_EQOR,
	OP_NOR,
	OP_EQNOR,
	OP_INVERSE,
	OP_EQINVERSE,
	OP_LBRACE,
	OP_RBRACE,
	OP_LPARENTHESIS,
	OP_RPARENTHESIS,
	OP_LBRAKET,
	OP_RBRAKET,
	OP_SEMI,
	OP_COMMA,
	OP_DOT,
	OP_SMALLER,
	OP_NSMALLER,
	OP_GREATER,
	OP_NGREATER,
	OP_NOT,
	OP_NOTEQ,
	OP_EQUAL,
	OP_QUERY,
	OP_QUOT,
	OP_DBQUOT,
	OP_CROSS
};

typedef struct _hn {
	enum TOKENTYPE type;
	std::string name;
	int id;
	struct _hn *next;
}hashNode;
typedef struct {
	enum TOKENTYPE type;
	int id;
	float value;
	char *s;
}tokenPrim;

class Lexeme {
private:
	string content;
	int idNum;

	hashNode *list[256];

	int negPos = 0;
public:
	vector<tokenPrim> output;
	vector<string> strId;

	Lexeme(const char *input = NULL);
	~Lexeme();

	int preserve(char *str);
	void prepare();
	Lexeme *input(const char *str);
	std::vector<tokenPrim>parse();
	int hash(string s);
	const char *get();
	static void error(const char *word, int type);
};

enum LEXEMEERROR {
	LE_ILLEGAL,
	LE_TOOLONG,
	LE_INCOMPLETE
};
class LexemeException {
private:
	std::string msg;
public:
	LexemeException(std::string s) {
		msg = s;
	}
	const char *message() { return msg.c_str(); }
};
#endif