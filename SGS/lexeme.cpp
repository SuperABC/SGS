#define _CRT_SECURE_NO_WARNINGS
#include "lexeme.h"
#include <cstring>

using namespace sgs;

Lexeme::Lexeme(const char *input) {
    for (auto& i : list) {
        i = nullptr;
    }
    idNum = 0;
    prepare();

    if (input == nullptr)return;
    content = std::string(input);
}
Lexeme::~Lexeme() {
    int j = 0, ids = 256;
    while (ids--) {
        HashNode *tmp = list[j];
        while (tmp) {
            list[j] = list[j]->next;
            delete tmp;
            tmp = list[j];
        }
        j++;
    }
}

int Lexeme::preserve(const char *str) {
    int value = 0;
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        value += str[i];
    }
    value %= 256;
    HashNode *tmp = list[value];
    while (tmp != nullptr && strcmp(tmp->name.data(), str) != 0) {
        tmp = tmp->next;
    }
    if (tmp == nullptr) {
        tmp = new HashNode();
        tmp->id = idNum++;
        tmp->type = SGS_TT_SYS;
        tmp->name = string(str);
        tmp->next = list[value];
        list[value] = tmp;
        strId.emplace_back(str);
    }
    return tmp->id;
}
void Lexeme::prepare() {
    preserve("bool");
    preserve("char");
    preserve("integer");
    preserve("float");
    preserve("string");
    preserve("array");
    preserve("class");
    preserve("function");

    preserve("let");
    preserve("be");
    preserve("new");
    preserve("start");
    preserve("end");
    preserve("with");
    preserve("if");
    preserve("then");
    preserve("else");
    preserve("loop");
    preserve("when");
	preserve("until");
    preserve("of");
    preserve("and");
    preserve("or");
    preserve("is");
    preserve("are");
    preserve("smaller");
    preserve("greater");
    preserve("than");
    preserve("use");
    preserve("library");
    preserve("result");
    preserve("quit");
    preserve("not");
    preserve("xor");
    preserve("break");
    preserve("redo");
    preserve("false");
    preserve("true");
    preserve("itself");
    preserve("return");
	preserve("its");
	preserve("lambda");
	preserve("embed");
    preserve("null");
    preserve("comment");
}
int Lexeme::hash(string str) {
    int value = 0;
    int len = str.length();
    for (int i = 0; i < len; i++) {
        value += str[i];
    }
    value %= 256;
    HashNode *tmp = list[value];
    while (tmp != nullptr && str != tmp->name) {
        tmp = tmp->next;
    }
    if (tmp == nullptr) {
        tmp = new HashNode();
        tmp->id = idNum++;
        tmp->type = SGS_TT_USER;
        tmp->name = std::string(str);
        tmp->next = list[value];
        list[value] = tmp;
        strId.push_back(str);
    }
    return tmp->id;
}

Lexeme *Lexeme::input(const char *str) {
    content = std::string(str);
	output.clear();
    return this;
}
vector<TokenPrim> Lexeme::parse() {
    output.clear();

    int len = content.length();
    for (int i = 0; i < len; i++) {
        TokenPrim node;
        if (content[i] == ' ' || content[i] == '\t')continue;
        else if (content[i] == '\n') {
            tmpLine++;
        }
		else if (content[i] >= '0' && content[i] <= '9') {
            node.type = SGS_TT_DATA;
            node.start = i;

            int t = i;
            while (content[t] >= '0'&&content[t] <= '9') {
                t++;
            }
            if (content[t] == '.' && ++t != content.size() && content[t] >= '0'&&content[t] <= '9') {
                node.fvalue = float(atof(content.data() + i));
                node.id = SGS_CT_FLOAT;
            }
			else {
                node.ivalue = atoi(content.data() + i);
                node.id = SGS_CT_INTEGER;
            }

            i++;
            bool point = false;
            while ((content[i] >= '0' && content[i] <= '9') || content[i] == '.') {
                if (content[i] == '.') {
                    if (point)break;
                    else point = true;
                }
                i++;
            }
            i--;
            if (content[i] == '.')i--;

            node.end = i + 1;
            node.line = tmpLine;
            output.push_back(node);
            continue;
        }
		else if ((content[i] >= 'A'&&content[i] <= 'Z') || (content[i] >= 'a'&&content[i] <= 'z') ||
			content[i] == '_' || content[i] == '$' || content[i] == '@') {
            node.type = SGS_TT_SYS;
            node.start = i;
            char str[32];
            int j = 0;
            while ((content[i] >= 'A'&&content[i] <= 'Z') || (content[i] >= 'a'&&content[i] <= 'z') ||
                (content[i] >= '0'&&content[i] <= '9') || content[i] == '_' || content[i] == '$' || content[i] == '@') {
                str[j++] = content[i++];
                if (j == 31) {
                    error(str, SGS_LE_TOOLONG);
                    while ((content[i] >= 'A'&&content[i] <= 'Z') || (content[i] >= 'a'&&content[i] <= 'z') ||
                        (content[i] >= '0'&&content[i] <= '9') || content[i] == '_')i++;
                }
            }
            str[j] = '\0';
            node.id = hash(str);
            if (node.id > SGS_ID_COMMENT) {
                node.type = SGS_TT_USER;
            }
			else {
				if (node.id == SGS_ID_TRUE) {
					node.type = SGS_TT_DATA;
					node.id = SGS_CT_BOOL;
					node.bvalue = true;
				}
				if (node.id == SGS_ID_FALSE) {
					node.type = SGS_TT_DATA;
					node.id = SGS_CT_BOOL;
					node.bvalue = false;
				}
			}
			i--;

            node.end = i + 1;
            node.line = tmpLine;
            output.push_back(node);
            continue;
        }
		else {
            node.type = SGS_TT_OP;
            node.start = i;
            node.id = 0;
            std::string str;
            switch (content[i]) {
            case '+':
                if (content[i + 1] == '+') {
                    node.id = SGS_OP_PLUSPLUS;
                    i++;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
				else if (content[i + 1] == '=') {
                    node.id = SGS_OP_EQPLUS;
                    i++;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
				else {
                    node.id = SGS_OP_PLUS;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
            case '-':
                if (content[i + 1] == '-') {
                    node.id = SGS_OP_MINUSMINUS;
                    i++;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
				else if (content[i + 1] == '=') {
                    node.id = SGS_OP_EQMINUS;
                    i++;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
				else {
                    node.id = SGS_OP_MINUS;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
            case '*':
                if (content[i + 1] == '=') {
                    node.id = SGS_OP_EQMULTY;
                    i++;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
				else {
                    node.id = SGS_OP_MULTY;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
            case '/':
                if (content[i + 1] == '=') {
                    node.id = SGS_OP_EQDIVIDE;
                    i++;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
				else {
                    node.id = SGS_OP_DIVIDE;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
            case '%':
                if (content[i + 1] == '=') {
                    node.id = SGS_OP_EQMOD;
                    i++;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
				else {
                    node.id = SGS_OP_MOD;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
            case '&':
                if (content[i + 1] == '&') {
                    node.id = SGS_OP_ANDAND;
                    i++;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
				else if (content[i + 1] == '=') {
                    node.id = SGS_OP_EQAND;
                    i++;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
				else {
                    node.id = SGS_OP_AND;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
            case '|':
                if (content[i + 1] == '|') {
                    node.id = SGS_OP_OROR;
                    i++;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
				else if (content[i + 1] == '=') {
                    node.id = SGS_OP_EQOR;
                    i++;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
				else {
                    node.id = SGS_OP_OR;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
            case '^':
                if (content[i + 1] == '=') {
                    node.id = SGS_OP_EQNOR;
                    i++;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
				else {
                    node.id = SGS_OP_NOR;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
            case '~':
                if (content[i + 1] == '=') {
                    node.id = SGS_OP_EQINVERSE;
                    i++;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
				else {
                    node.id = SGS_OP_INVERSE;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
            case '(':
                node.id = SGS_OP_LBRACE;
                node.end = i + 1;
                node.line = tmpLine;
                output.push_back(node);
                continue;
            case ')':
                node.id = SGS_OP_RBRACE;
                node.end = i + 1;
                node.line = tmpLine;
                output.push_back(node);
                continue;
            case '[':
                node.id = SGS_OP_LPARENTHESIS;
                node.end = i + 1;
                node.line = tmpLine;
                output.push_back(node);
                continue;
            case ']':
                node.id = SGS_OP_RPARENTHESIS;
                node.end = i + 1;
                node.line = tmpLine;
                output.push_back(node);
                continue;
            case '{':
                node.id = SGS_OP_LBRAKET;
                node.end = i + 1;
                node.line = tmpLine;
                output.push_back(node);
                continue;
            case '}':
                node.id = SGS_OP_RBRAKET;
                node.end = i + 1;
                node.line = tmpLine;
                output.push_back(node);
                continue;
            case '>':
                if (content[i + 1] == '=') {
                    node.id = SGS_OP_NSMALLER;
                    i++;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
				else {
                    node.id = SGS_OP_GREATER;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
            case '<':
                if (content[i + 1] == '=') {
                    node.id = SGS_OP_NGREATER;
                    i++;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
				else {
                    node.id = SGS_OP_SMALLER;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
            case ';':
                node.id = SGS_OP_SEMI;
                node.end = i + 1;
                node.line = tmpLine;
                output.push_back(node);
                continue;
            case ',':
                node.id = SGS_OP_COMMA;
                node.end = i + 1;
                node.line = tmpLine;
                output.push_back(node);
                continue;
            case '.':
                node.id = SGS_OP_DOT;
                node.end = i + 1;
                node.line = tmpLine;
                output.push_back(node);
                continue;
            case  '!':
                if (content[i + 1] == '=') {
                    node.id = SGS_OP_NOTEQ;
                    i++;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
				else {
                    node.id = SGS_OP_NOT;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                    continue;
                }
            case '=':
                node.id = SGS_OP_EQUAL;
                node.end = i + 1;
                node.line = tmpLine;
                output.push_back(node);
                continue;
            case '?':
                node.id = SGS_OP_QUERY;
                node.end = i + 1;
                node.line = tmpLine;
                output.push_back(node);
                continue;
            case '\"': {
                node.type = SGS_TT_DATA;
                i++;
                while (content[i] != '\"'&& content[i] != '\n') {
                    str.push_back(content[i++]);
                }
                if (content[i] != '\"')
                    error("", SGS_LE_INCOMPLETE);
                char *a = new char[str.length() + 1];
                strcpy(a, str.data());
                node.id = SGS_CT_STRING;
                node.svalue = a;
                node.end = i + 1;
                node.line = tmpLine;
                output.push_back(node);
                continue;
            }
            case '\'':
                if (content[++i] == 's') {
                    node.type = SGS_TT_SYS;
                    node.id = SGS_ID_OF;
                    node.end = i + 1;
                    node.line = tmpLine;
                    output.push_back(node);
                }
				else {
                    error("\'s", SGS_LE_EXPOP);
					while (content[++i] != '\n');
					i--;
                }
                continue;
            case '#':
                while (content[++i] != '\n');
				i--;
            default:
				break;
            }
        }
    }
    return output;
}

const char *Lexeme::get() const {
    return content.data();
}

void Lexeme::error(const char *word, LEXEMEERROR type) {
    switch (type) {
    case SGS_LE_ILLEGAL:
        msgList.emplace_back(string("Illegal identifier: ") + word + ".\n", MT_ERROR);
    case SGS_LE_TOOLONG:
        msgList.emplace_back(word + string("Identifier length overflow, cut at position 31.\n"), MT_WARNING);
    case SGS_LE_INCOMPLETE:
        msgList.emplace_back(word + string("Incomplete type.\n"), MT_ERROR);
    case SGS_LE_EXPOP:
        msgList.emplace_back(word + string("Operator error.\n"), MT_ERROR);
    }
}
