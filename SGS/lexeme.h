#ifndef LEXEME_H
#define LEXEME_H
#define _CRT_SECURE_NO_WARNINGS

#include "message.h"
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>


using std::vector;
using std::string;

namespace sgs {

	enum TOKENTYPE {
		SGS_TT_USER,
		SGS_TT_DATA,
		SGS_TT_OP,
		SGS_TT_SYS,
	};

	enum IDENTIFIER {
		SGS_ID_BOOL,
		SGS_ID_CHAR,
		SGS_ID_INTEGER,
		SGS_ID_FLOAT,
		SGS_ID_STRING,
		SGS_ID_ARRAY,
		SGS_ID_CLASS,
		SGS_ID_FUNCTION,

		SGS_ID_LET,
		SGS_ID_BE,
		SGS_ID_NEW,
		SGS_ID_START,
		SGS_ID_END,
		SGS_ID_WITH,
		SGS_ID_IF,
		SGS_ID_THEN,
		SGS_ID_ELSE,
		SGS_ID_LOOP,
		SGS_ID_WHEN,
		SGS_ID_UNTIL,
		SGS_ID_OF,
		SGS_ID_AND,
		SGS_ID_OR,
		SGS_ID_IS,
		SGS_ID_ARE,
		SGS_ID_SMALLER,
		SGS_ID_GREATER,
		SGS_ID_THAN,
		SGS_ID_USE,
		SGS_ID_LIBRARY,
		SGS_ID_RESULT,
		SGS_ID_QUIT,
		SGS_ID_NOT,
		SGS_ID_XOR,
		SGS_ID_BREAK,
		SGS_ID_REDO,
		SGS_ID_FALSE,
		SGS_ID_TRUE,
		SGS_ID_ITSELF,
		SGS_ID_RETURN,
		SGS_ID_ITS,
		SGS_ID_LAMBDA,
		SGS_ID_EMBED,
		SGS_ID_NULL,
		SGS_ID_COMMENT
	};
	enum OPERATOR {
		SGS_OP_PLUS,
		SGS_OP_PLUSPLUS,
		SGS_OP_EQPLUS,
		SGS_OP_MINUS,
		SGS_OP_MINUSMINUS,
		SGS_OP_EQMINUS,
		SGS_OP_NEG,
		SGS_OP_MULTY,
		SGS_OP_EQMULTY,
		SGS_OP_DIVIDE,
		SGS_OP_EQDIVIDE,
		SGS_OP_MOD,
		SGS_OP_EQMOD,
		SGS_OP_AND,
		SGS_OP_ANDAND,
		SGS_OP_EQAND,
		SGS_OP_OR,
		SGS_OP_OROR,
		SGS_OP_EQOR,
		SGS_OP_NOR,
		SGS_OP_EQNOR,
		SGS_OP_INVERSE,
		SGS_OP_EQINVERSE,
		SGS_OP_LBRACE,
		SGS_OP_RBRACE,
		SGS_OP_LPARENTHESIS,
		SGS_OP_RPARENTHESIS,
		SGS_OP_LBRAKET,
		SGS_OP_RBRAKET,
		SGS_OP_SEMI,
		SGS_OP_COMMA,
		SGS_OP_DOT,
		SGS_OP_SMALLER,
		SGS_OP_NSMALLER,
		SGS_OP_GREATER,
		SGS_OP_NGREATER,
		SGS_OP_NOT,
		SGS_OP_NOTEQ,
		SGS_OP_EQUAL,
		SGS_OP_QUERY,
		SGS_OP_QUOT,
		SGS_OP_DBQUOT,
		SGS_OP_CROSS
	};
	enum LITERALTYPE {
		SGS_CT_BOOL,
		SGS_CT_CHAR,
		SGS_CT_INTEGER,
		SGS_CT_FLOAT,
		SGS_CT_STRING,
	};

	struct HashNode {
		enum TOKENTYPE type;
		std::string name;
		int id;
		HashNode *next;
	};

	struct TokenPrim {
		enum TOKENTYPE type;
		int id;
		union {
			bool bvalue;
			char cvalue;
			int ivalue;
			float fvalue;
			char *svalue;
			void *ovalue = nullptr;
		};

		int line;
		int start, end;
	};

	enum LEXEMEERROR {
		SGS_LE_ILLEGAL,
		SGS_LE_TOOLONG,
		SGS_LE_INCOMPLETE,
		SGS_LE_EXPOP
	};
	class Lexeme {
	private:
		string content;
		int idNum;

		HashNode *list[256];

		int tmpLine = 1;
	public:
		vector<TokenPrim> output;
		vector<string> strId;

		vector<sgsMsg> msgList;

		Lexeme(const char *input = nullptr);
		virtual ~Lexeme();

		int preserve(const char *str);
		void prepare();
		int hash(string str);

		Lexeme *input(const char *str);
		std::vector<TokenPrim>parse();

		const char *get() const;

		void error(const char *word, LEXEMEERROR type);
	};

}

#endif