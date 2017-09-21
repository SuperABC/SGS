#include "syntax.h"
#pragma warning(disable:4996)

Syntax::Syntax(){
	step = SS_IDLE;
	proc = 0;

	output = stateSeq();
	last = &output;
}
Syntax::Syntax(vector<string>ids, vector<tokenPrim>input) {
	content = input;
	strId = ids;
	step = SS_IDLE;
	proc = 0;
}
Syntax::~Syntax() {

}

char *Syntax::opStr(int id) {
	switch (id) {
	case OP_PLUS: return "+";
	case OP_PLUSPLUS: return "++";
	case OP_EQPLUS: return "+=";
	case OP_MINUS: return "-";
	case OP_MINUSMINUS: return "--";
	case OP_EQMINUS: return "-=";
	case OP_MULTY: return "*";
	case OP_EQMULTY: return "*=";
	case OP_DIVIDE: return "/";
	case OP_EQDIVIDE: return "/=";
	case OP_AND: return "&";
	case OP_ANDAND: return "&&";
	case OP_EQAND: return "&=";
	case OP_OR: return "|";
	case OP_OROR: return "||";
	case OP_EQOR: return "|=";
	case OP_NOR: return "^";
	case OP_EQNOR: return "^=";
	case OP_INVERSE: return "~";
	case OP_EQINVERSE: return "~=";
	case OP_LBRACE: return "(";
	case OP_RBRACE: return ")";
	case OP_LPARENTHESIS: return "[";
	case OP_RPARENTHESIS: return "]";
	case OP_LBRAKET: return "{";
	case OP_RBRAKET: return "}";
	case OP_SEMI: return ";";
	case OP_COMMA: return ",";
	case OP_DOT: return ".";
	case OP_SMALLER: return "<";
	case OP_NSMALLER: return ">=";
	case OP_GREATER: return ">";
	case OP_NGREATER: return "<=";
	case OP_NOT: return "!";
	case OP_NOTEQ: return "!=";
	case OP_EQUAL: return "=";
	case OP_QUERY: return "?";
	case OP_QUOT: return "\'";
	case OP_DBQUOT: return "\"";
	default: return "";
	}
}
char *Syntax::valueStr(float value) {
	char *rt = (char *)malloc(20);
	sprintf(rt, "%f", value);
	return rt;
}

Syntax *Syntax::input(vector<string> ids, vector<tokenPrim> src) {
	content = src;
	
	for (auto i : ids) {
		unsigned int j = 0;
		for (; j < strId.size(); j++) {
			if (i == strId[j])break;
		}
		if (j == strId.size())strId.push_back(i);
	}
	content = src;
	return this;
}
stateSeq Syntax::parse() {
	stateSeq *tmp = &output;
	while (proc < content.size()) {
		if (content[proc].type == TT_SYS&&content[proc].id == ID_USE) {
			proc++;
			if (content[proc].type == TT_SYS&&content[proc].id == ID_LIBRARY) {
				proc++;
				parseLibrary(strId[content[proc].id]);

				proc++;
				if (content[proc].type == TT_OP&&content[proc].id != OP_DOT)
					error("Use library", SE_EXPDOT);
				proc++;
			}
			else {
				error("Use", SE_UNIQUE);
				proc++;
			}
			continue;
		}
		if (content[proc].type == TT_SYS&&content[proc].id == ID_NEW) {
			if (content[proc].type == TT_SYS&&content[proc].id == ID_CLASS) {
				proc++;
				globeClassType.push_back(parseClassDec());
			}
			else if (content[proc].type == TT_SYS&&content[proc].id == ID_FUNCTION) {
				proc++;
				globeFunc.push_back(parseFuncDec());
			}
			continue;
		}
		if (content[proc].type == TT_SYS&&content[proc].id == ID_LET) {
			proc++;
			string newVar;
			int varType;
			if (content[proc].type == TT_USER) {
				int classIdx;
				string valueType;
				if (classIdx = findClass() >= 0) {

				}
				else if ((valueType = findVar()) == "") {

				}
				else
					error(strId[content[proc].id].c_str(), SE_NOID);
			}
			else if (content[proc].type == TT_SYS) {
				if ((varType = findType()) != -1) {
					proc++;
					if ((newVar = findVar()) == "")
						error(strId[content[proc].id].c_str(), SE_REDEF);
					else {
						globeVar.push_back(varNode(varType, newVar));
					}
				}
				else
					error(strId[content[proc].id].c_str(), SE_UNIQUE);
			}

			if (content[proc].type == TT_OP&&content[proc].id == OP_DOT)
				continue;
			else if (content[proc].type == TT_SYS&&content[proc].id == ID_BE) {
				proc++;
				rtVal st = rtVal();
				st.op = VO_ASSIGN;
				st.left = new varNode(varType, newVar);
				st.right = parseValue();
				last->act = st;
				last = last->next;
			}
			else
				error("", SE_INCOMPLETE);
			continue;
		}
		if (content[proc].type == TT_SYS&&content[proc].id == ID_START) {
			proc++;
			int funcIdx;
			if (funcIdx = findFunc() >= 0) {
				globeFunc[funcIdx].content = parseFuncDef();
			}
			else
				error(strId[content[proc].id].c_str(), SE_NOID);
			continue;
		}
		if (content[proc].type == TT_OP&&content[proc].id == OP_DOT) {
			proc++;
			continue;
		}
	}
	return output;
}

void Syntax::parseLibrary(string lib) {

}
varNode *Syntax::parseValue() {
	varNode *ret = new varNode();
	ret->name = "";
	if (content[proc].type != TT_DATA)
		error("", SE_INCOMPLETE);
	if (content[proc].s != NULL) {
		ret->t = VT_STRING;
		ret->val = malloc(sizeof(content[proc].s) + 1);
		strcpy((char *)ret->val, content[proc++].s);
	}
	else {
		ret->t = VT_FLOAT;
		ret->val = new float(content[proc++].value);
	}
	return ret;
}
funcNode Syntax::parseFuncDec() {
	funcNode ret = funcNode();
	return ret;
}
stateSeq Syntax::parseFuncDef() {
	stateSeq ret = stateSeq();
	return ret;
}
classType Syntax::parseClassDec() {
	classType ret = classType();
	return ret;
}
classNode Syntax::parseClassConstr() {
	classNode ret = classNode();
	return ret;
}
string Syntax::findVar() {
	string tmp;
	while (content[proc].type == TT_USER) {
		tmp += strId[content[proc++].id];
		tmp += " ";
	}
	tmp.pop_back();
	for (auto n : globeVar) {
		if (n.name == tmp) {
			return "";
		}
	}
	return tmp;
}
int Syntax::findClass() {
	string tmp;
	int preProc = proc;
	while (content[proc].type == TT_USER) {
		tmp += strId[content[proc++].id];
		tmp += " ";
	}
	tmp.pop_back();
	for (unsigned int i = 0; i < globeClassType.size(); i++) {
		if (globeClassType[i].name == tmp)return i;
	}
	proc = preProc;
	return -1;
}
int Syntax::findFunc() {
	string tmp;
	int preProc = proc;
	while (content[proc].type == TT_USER) {
		tmp += strId[content[proc++].id];
		tmp += " ";
	}
	tmp.pop_back();
	for (unsigned int i = 0; i < globeFunc.size(); i++) {
		if (globeFunc[i].declare.name == tmp)return i;
	}
	proc = preProc;
	return -1;
}
int Syntax::findType() {
	if (content[proc].type == TT_SYS) {
		if (content[proc].id >= VT_INTEGER&&content[proc].id <= VT_ARRAY)
			return content[proc].id;
	}
	return -1;
}

void Syntax::error(const char *inst, int type) {
	switch (type) {
	case SE_UNIQUE:
		throw new SyntaxException(std::string(inst) + "无此用法。");
	case SE_EXPDOT:
		throw new SyntaxException(std::string(inst) + "缺少句号。");
	case SE_EXPCOMMA:
		throw new SyntaxException(std::string(inst) + "缺少逗号。");
	case SE_EXPBRACE:
		throw new SyntaxException(std::string(inst) + "括号不完整。");
	case SE_REDEF:
		throw new SyntaxException(std::string(inst) + "重定义。");
	case SE_INVALIDTYPE:
		throw new SyntaxException(std::string(inst) + "操作对象类型错误。");
	case SE_NOID:
		throw new SyntaxException(std::string(inst) + "未定义。");
	case SE_INCOMPLETE:
		throw new SyntaxException(std::string(inst) + "语句不完整。");
	case SE_UNKNOWN:
		throw new SyntaxException(std::string(inst) + "未知错误。");
	case SE_UNSUPPORT:
		throw new SyntaxException(std::string(inst) + "暂不支持。");
	}
}
