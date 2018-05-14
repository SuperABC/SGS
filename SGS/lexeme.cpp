#define _CRT_SECURE_NO_WARNINGS
#include "lexeme.h"

SgsLex::SgsLex(const char *input) {
	for (int i = 0; i < 256; i++) {
		list[i] = NULL;
	}
	idNum = 0;
	prepare();

	if (input == NULL)return;
	content = std::string(input);
}
SgsLex::~SgsLex() {
	int i = 0, ids = 256;
	while (ids--) {
		sgsHashNode *tmp = list[i];
		while (tmp) {
			list[i] = list[i]->next;
			delete tmp;
			tmp = list[i];
		}
		i++;
	}
	for (unsigned int i = 0; i < output.size(); i++) {
		if (output[i].s)delete output[i].s;
	}
}

int SgsLex::preserve(const char *str) {
	int value = 0;
	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		value += str[i];
	}
	value %= 256;
	sgsHashNode *tmp = list[value];
	while (tmp != NULL && strcmp(tmp->name.data(), str) != 0) {
		tmp = tmp->next;
	}
	if (tmp == NULL) {
		tmp = new sgsHashNode();
		tmp->id = idNum++;
		tmp->type = SGS_TT_SYS;
		tmp->name = string(str);
		tmp->next = list[value];
		list[value] = tmp;
		strId.push_back(string(str));
	}
	return tmp->id;
}
void SgsLex::prepare() {
	preserve("integer");
	preserve("char");
	preserve("string");
	preserve("float");
	preserve("bool");
	preserve("array");
	preserve("class");
	preserve("function");
	preserve("none");
	preserve("let");
	preserve("be");
	preserve("new");
	preserve("with");
	preserve("start");
	preserve("end");
	preserve("if");
	preserve("then");
	preserve("else");
	preserve("loop");
	preserve("when");
	preserve("of");
	preserve("is");
	preserve("are");
	preserve("smaller");
	preserve("larger");
	preserve("than");
	preserve("use");
	preserve("library");
	preserve("result");
	preserve("quit");
	preserve("and");
	preserve("or");
	preserve("not");
	preserve("xor");
	preserve("break");
	preserve("redo");
	preserve("false");
	preserve("true");
	preserve("itself");
	preserve("return");
	preserve("null");
	preserve("comment");
}
int SgsLex::hash(string str) {
	int value = 0;
	int len = str.length();
	for (int i = 0; i < len; i++) {
		value += str[i];
	}
	value %= 256;
	sgsHashNode *tmp = list[value];
	while (tmp != NULL && str != tmp->name) {
		tmp = tmp->next;
	}
	if (tmp == NULL) {
		tmp = new sgsHashNode();
		tmp->id = idNum++;
		tmp->type = SGS_TT_USER;
		tmp->name = std::string(str);
		tmp->next = list[value];
		list[value] = tmp;
		strId.push_back(str);
	}
	return tmp->id;
}

SgsLex *SgsLex::input(const char *str) {
	content = std::string(str);
	return this;
}
vector<sgsTokenPrim> SgsLex::parse() {
	output.clear();

	int len = content.length();
	for (int i = 0; i < len; i++) {
		sgsTokenPrim node;
		if (content[i] == ' ' || content[i] == '\t')continue;
		else if (content[i] >= '0' && content[i] <= '9') {
			node.type = SGS_TT_DATA;
			node.start = i;

			int t = i;
			while (content[t] >= '0'&&content[t] <= '9') {
				t++;
			}
			if (content[t] == '.' && ++t != content.size() && content[t] >= '0'&&content[t] <= '9') {
				node.value = float(atof(content.data() + i));
				node.id = CT_FLOAT;
			}
			else {
				node.value = float(atoi(content.data() + i));
				node.id = CT_INT;
			}

			i++;
			bool point = false;
			while (content[i] >= '0' && content[i] <= '9' || content[i] == '.') {
				if (content[i] == '.') {
					if (point)break;
					else point = true;
				}
				i++;
			}
			i--;
			if (content[i] == '.')i--;

			node.end = i + 1;
			output.push_back(node);
			continue;
		}
		else if (content[i] >= 'A'&&content[i] <= 'Z' || content[i] >= 'a'&&content[i] <= 'z' || content[i] == '_') {
			node.type = SGS_TT_SYS;
			node.start = i;
			node.value = 0;
			char str[32];
			int j = 0;
			while (content[i] >= 'A'&&content[i] <= 'Z' || content[i] >= 'a'&&content[i] <= 'z' ||
				content[i] >= '0'&&content[i] <= '9' || content[i] == '_') {
				str[j++] = content[i++];
				if (j == 31) {
					str[j] = '\0';
					error(str, SGS_LE_TOOLONG);
				}
			}
			str[j] = '\0';
			node.id = hash(str);
			if (node.id > SGS_ID_COMMENT) {
				node.type = SGS_TT_USER;
			}

			node.end = i;
			output.push_back(node);
			i--;
			continue;
		}
		else {
			node.type = SGS_TT_OP;
			node.start = i;
			node.value = 0;
			std::string str;
			char *a = new char[32];
			switch (content[i]) {
			case '+':
				if (content[i + 1] == '+') {
					node.id = SGS_OP_PLUSPLUS;
					i++;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
				else if (content[i + 1] == '=') {
					node.id = SGS_OP_EQPLUS;
					i++;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
				else {
					node.id = SGS_OP_PLUS;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
			case '-':
				if (content[i + 1] == '-') {
					node.id = SGS_OP_MINUSMINUS;
					i++;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
				else if (content[i + 1] == '=') {
					node.id = SGS_OP_EQMINUS;
					i++;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
				else {
					node.id = SGS_OP_MINUS;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
			case '*':
				if (content[i + 1] == '=') {
					node.id = SGS_OP_EQMULTY;
					i++;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
				else {
					node.id = SGS_OP_MULTY;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
			case '/':
				if (content[i + 1] == '=') {
					node.id = SGS_OP_EQDIVIDE;
					i++;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
				else {
					node.id = SGS_OP_DIVIDE;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
			case '%':
				if (content[i + 1] == '=') {
					node.id = SGS_OP_EQMOD;
					i++;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
				else {
					node.id = SGS_OP_MOD;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
			case '&':
				if (content[i + 1] == '&') {
					node.id = SGS_OP_ANDAND;
					i++;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
				else if (content[i + 1] == '=') {
					node.id = SGS_OP_EQAND;
					i++;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
				else {
					node.id = SGS_OP_AND;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
			case '|':
				if (content[i + 1] == '|') {
					node.id = SGS_OP_OROR;
					i++;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
				else if (content[i + 1] == '=') {
					node.id = SGS_OP_EQOR;
					i++;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
				else {
					node.id = SGS_OP_OR;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
			case '^':
				if (content[i + 1] == '=') {
					node.id = SGS_OP_EQNOR;
					i++;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
				else {
					node.id = SGS_OP_NOR;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
			case '~':
				if (content[i + 1] == '=') {
					node.id = SGS_OP_EQINVERSE;
					i++;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
				else {
					node.id = SGS_OP_INVERSE;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
			case '(':
				node.id = SGS_OP_LBRACE;
				node.end = i + 1;
				output.push_back(node);
				continue;
			case ')':
				node.id = SGS_OP_RBRACE;
				node.end = i + 1;
				output.push_back(node);
				continue;
			case '[':
				node.id = SGS_OP_LPARENTHESIS;
				node.end = i + 1;
				output.push_back(node);
				continue;
			case ']':
				node.id = SGS_OP_RPARENTHESIS;
				node.end = i + 1;
				output.push_back(node);
				continue;
			case '{':
				node.id = SGS_OP_LBRAKET;
				node.end = i + 1;
				output.push_back(node);
				continue;
			case '}':
				node.id = SGS_OP_RBRAKET;
				node.end = i + 1;
				output.push_back(node);
				continue;
			case '>':
				if (content[i + 1] == '=') {
					node.id = SGS_OP_NSMALLER;
					i++;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
				else {
					node.id = SGS_OP_GREATER;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
			case '<':
				if (content[i + 1] == '=') {
					node.id = SGS_OP_NGREATER;
					i++;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
				else {
					node.id = SGS_OP_SMALLER;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
			case ';':
				node.id = SGS_OP_SEMI;
				node.end = i + 1;
				output.push_back(node);
				continue;
			case ',':
				node.id = SGS_OP_COMMA;
				node.end = i + 1;
				output.push_back(node);
				continue;
			case '.':
				node.id = SGS_OP_DOT;
				node.end = i + 1;
				output.push_back(node);
				continue;
			case  '!':
				if (content[i + 1] == '=') {
					node.id = SGS_OP_NOTEQ;
					i++;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
				else {
					node.id = SGS_OP_NOT;
					node.end = i + 1;
					output.push_back(node);
					continue;
				}
			case '=':
				node.id = SGS_OP_EQUAL;
				node.end = i + 1;
				output.push_back(node);
				continue;
			case '?':
				node.id = SGS_OP_QUERY;
				node.end = i + 1;
				output.push_back(node);
				continue;
			case '\"':
				node.type = SGS_TT_DATA;
				i++;
				while (content[i] != '\"'&& i < (int)content.length()) {
					str.push_back(content[i++]);
				}
				if (content[i] != '\"')
					error("", SGS_LE_INCOMPLETE);
				strcpy(a, str.data());
				node.id = CT_STRING;
				node.s = a;
				node.end = i + 1;
				output.push_back(node);
				continue;
			case '\'':
				if (content[++i] == 's') {
					node.type = SGS_TT_SYS;
					node.id = SGS_ID_OF;
					node.end = i + 1;
					output.push_back(node);
				}
				else error("\'s", SGS_LE_EXPOP);
				continue;
			case '#':
				while (++i <  (int)content.size() && content[i] != '\n');
				continue;
			}
		}
	}
	return output;
}

const char *SgsLex::get() {
	return content.data();
}

void SgsLex::error(const char *word, int type) {
	switch (type) {
	case SGS_LE_ILLEGAL:
		throw new SGSLexemeException(string("非法的标识符") + word);
	case SGS_LE_TOOLONG:
		throw new SGSLexemeException(string("标识符太长：") + word);
	case SGS_LE_INCOMPLETE:
		throw new SGSLexemeException(string("结构不完整") + word);
	case SGS_LE_EXPOP:
		throw new SGSLexemeException(string("符号错误") + word);
	}
}
