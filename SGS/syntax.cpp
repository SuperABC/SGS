#define _CRT_SECURE_NO_WARNINGS
#include "syntax.h"

SgsSyntax::SgsSyntax() {
	proc = 0;

	prepare();
}
SgsSyntax::SgsSyntax(vector<string> &ids, vector<sgsTokenPrim> &input) {
	proc = 0;

	content = input;
	strId = ids;

	prepare();
}
SgsSyntax::~SgsSyntax() {

}
void SgsSyntax::prepare() {

}

SgsSyntax *SgsSyntax::input(vector<string> &ids, vector<sgsTokenPrim> &src) {
	content = src;

	for (auto i : ids) {
		unsigned int j = 0;
		for (; j < strId.size(); j++) {
			if (i == strId[j])break;
		}
		if (j == strId.size())strId.push_back(i);
	}
	return this;
}
stateNode *SgsSyntax::parse() {
	proc = 0;

	while (proc < content.size()) {
		if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_USE) {
			continue;
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_NEW) {
			continue;
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LET) {
			continue;
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_START) {
			continue;
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_IF) {
			continue;
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LOOP) {
			continue;
		}
		else if (content[proc].type == SGS_TT_USER) {
			continue;
		}
		else if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_DOT) {
			proc++;
			continue;
		}
	}

	return output;
}

void SgsSyntax::parseLib(string lib) {

}
expNode *SgsSyntax::parseExp() {
	return NULL;
}
funcNode *SgsSyntax::parseFuncDec() {
	return NULL;
}
stateNode *SgsSyntax::parseFuncDef(int funcid) {
	return NULL;
}
classNode *SgsSyntax::parseParam(int funcid) {
	return NULL;
}
classDec *SgsSyntax::parseClassDec() {
	return NULL;
}
stateNode *SgsSyntax::parseBlock() {
	return NULL;
}
int SgsSyntax::findType() {
	return -1;
}
int SgsSyntax::findVar() {
	return -1;
}
int SgsSyntax::findClass() {
	return -1;
}
int SgsSyntax::findFunc() {
	return -1;
}
int SgsSyntax::findArray() {
	return -1;
}

void SgsSyntax::clearMem() {
	synMem.clear();
}

bool SgsSyntax::compare(int op1, int op2) {//op1<=op2
	int prio1, prio2;
	switch (op1) {
	case SGS_OP_LBRACE:
		prio1 = 0;
		break;
	case SGS_OP_OROR:
		prio1 = 1;
		break;
	case SGS_OP_ANDAND:
		prio1 = 2;
		break;
	case SGS_OP_OR:
		prio1 = 3;
		break;
	case SGS_OP_NOR:
		prio1 = 4;
		break;
	case SGS_OP_AND:
		prio1 = 5;
		break;
	case SGS_OP_EQUAL:
	case SGS_OP_NOTEQ:
		prio1 = 6;
		break;
	case SGS_OP_GREATER:
	case SGS_OP_NGREATER:
	case SGS_OP_SMALLER:
	case SGS_OP_NSMALLER:
		prio1 = 7;
		break;
	case SGS_OP_PLUS:
	case SGS_OP_MINUS:
		prio1 = 8;
		break;
	case SGS_OP_MULTY:
	case SGS_OP_DIVIDE:
	case SGS_OP_MOD:
		prio1 = 9;
		break;
	case SGS_OP_NOT:
	case SGS_OP_INVERSE:
	case SGS_OP_NEG:
		prio1 = 10;
		break;
	}
	switch (op2) {
	case SGS_OP_LBRACE:
		prio2 = 0;
		break;
	case SGS_OP_OROR:
		prio2 = 1;
		break;
	case SGS_OP_ANDAND:
		prio2 = 2;
		break;
	case SGS_OP_OR:
		prio2 = 3;
		break;
	case SGS_OP_NOR:
		prio2 = 4;
		break;
	case SGS_OP_AND:
		prio2 = 5;
		break;
	case SGS_OP_EQUAL:
	case SGS_OP_NOTEQ:
		prio2 = 6;
		break;
	case SGS_OP_GREATER:
	case SGS_OP_NGREATER:
	case SGS_OP_SMALLER:
	case SGS_OP_NSMALLER:
		prio2 = 7;
		break;
	case SGS_OP_PLUS:
	case SGS_OP_MINUS:
		prio2 = 8;
		break;
	case SGS_OP_MULTY:
	case SGS_OP_DIVIDE:
	case SGS_OP_MOD:
		prio2 = 9;
		break;
	case SGS_OP_NOT:
	case SGS_OP_INVERSE:
	case SGS_OP_NEG:
		prio2 = 10;
		break;
	}
	return prio1 <= prio2;
}
void SgsSyntax::error(const char *inst, int type) {
	switch (type) {
	case SGS_SE_EXPOSE:
		throw new SGSyntaxException(std::string(inst) + "语句块不封闭。");
	case SGS_SE_UNIQUE:
		throw new SGSyntaxException(std::string(inst) + "无此用法。");
	case SGS_SE_EXPDOT:
		throw new SGSyntaxException(std::string(inst) + "缺少句号。");
	case SGS_SE_EXPCOMMA:
		throw new SGSyntaxException(std::string(inst) + "缺少逗号。");
	case SGS_SE_EXPBRACE:
		throw new SGSyntaxException(std::string(inst) + "括号不完整。");
	case SGS_SE_REDEF:
		throw new SGSyntaxException(std::string(inst) + "重定义。");
	case SGS_SE_INVALIDTYPE:
		throw new SGSyntaxException(std::string(inst) + "操作对象类型错误。");
	case SGS_SE_DISACCORD:
		throw new SGSyntaxException(std::string(inst) + "前后不一致。");
	case SGS_SE_NOID:
		throw new SGSyntaxException(std::string(inst) + "未定义。");
	case SGS_SE_INCOMPLETE:
		throw new SGSyntaxException(std::string(inst) + "语句不完整。");
	case SGS_SE_UNKNOWN:
		throw new SGSyntaxException(std::string(inst) + "未知错误。");
	case SGS_SE_UNSUPPORT:
		throw new SGSyntaxException(std::string(inst) + "暂不支持。");
	}
}
