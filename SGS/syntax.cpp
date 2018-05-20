#define _CRT_SECURE_NO_WARNINGS
#include "syntax.h"

using namespace sgs;

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
void SgsSyntax::parse() {
	proc = 0;

	while (proc < content.size()) {
		if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_USE) {
			proc++;
			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LIBRARY) {
				proc++;
				parseLib(strId[content[proc].id]);
				proc++;
			}
			else {
				error("Use", SGS_SE_UNIQUE);
				skipLine();
			}
			continue;
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_NEW) {
			proc++;
			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_CLASS) {
				proc++;
				stmts.push_back(parseClassDec());
			}
			else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_FUNCTION) {
				proc++;
				stmts.push_back(parseFuncDec());
			}
			else
				error("New", SGS_SE_UNIQUE);
			continue;
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LET) {
			proc++;

			Expression *left, *right;
			int classIdx = -1;
			string newVar;
			if (content[proc].type == SGS_TT_USER) {
				if ((classIdx = findClass()) >= 0) {
					stmts.push_back(new TypeDef(((TypeDef *)stmts[classIdx])->getDecType(), newVar = parseUser()));
					left = new IdExp(newVar);
				}
				else {
					left = parseExp();
				}
			}
			else if (content[proc].type == SGS_TT_SYS) {
				if (content[proc].id == SGS_ID_INTEGER) {
					if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
						proc += 2;
						int length = content[proc].value;
						proc++;
						stmts.push_back(new TypeDef(new ArrayType(new BasicType(BT_INT), length), newVar = parseUser()));
					}
					else {
						proc++;
						stmts.push_back(new TypeDef(new BasicType(BT_INT), newVar = parseUser()));
						left = new IdExp(newVar);
					}
				}
				else if(content[proc].id == SGS_ID_FLOAT) {
					if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
						proc += 2;
						int length = content[proc].value;
						proc++;
						stmts.push_back(new TypeDef(new ArrayType(new BasicType(BT_FLOAT), length), newVar = parseUser()));
					}
					else {
						proc++;
						stmts.push_back(new TypeDef(new BasicType(BT_FLOAT), newVar = parseUser()));
						left = new IdExp(newVar);
					}
				}
				else if (content[proc].id == SGS_ID_BOOL) {
					if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
						proc += 2;
						int length = content[proc].value;
						proc++;
						stmts.push_back(new TypeDef(new ArrayType(new BasicType(BT_BOOL), length), newVar = parseUser()));
					}
					else {
						proc++;
						stmts.push_back(new TypeDef(new BasicType(BT_BOOL), newVar = parseUser()));
						left = new IdExp(newVar);
					}
				}
				else if (content[proc].id == SGS_ID_CHAR) {
					error("char", SGS_SE_UNSUPPORT);
					skipLine();
				}
				else if (content[proc].id == SGS_ID_STRING) {
					if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
						proc += 2;
						int length = content[proc].value;
						proc++;
						stmts.push_back(new TypeDef(new ArrayType(new BasicType(BT_BOOL), length), newVar = parseUser()));
					}
					else {
						proc++;
						stmts.push_back(new TypeDef(new BasicType(BT_STRING), newVar = parseUser()));
						left = new IdExp(newVar);
					}
				}
				else {
					error(strId[content[proc].id].data(), SGS_SE_UNIQUE);
					skipLine();
				}
			}
			else {
				error("Let", SGS_SE_UNIQUE);
				skipLine();
			}

			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_BE) {
				proc++;
				stmts.push_back(new AssignStmt(left, parseExp()));
			}
			continue;
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_START) {
			proc++;
			int funcIdx;
			if ((funcIdx = findProto()) >= 0) {
				stmts.push_back(parseFuncDef(funcIdx));
				if (funcIdx != findProto())
					error(((FuncProto *)stmts[funcIdx])->getName().data(), SGS_SE_DISACCORD);
			}
			else {
				error(parseUser().data() , SGS_SE_NOID);
				skipLine();
			}
			continue;
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_IF) {
			proc++;

			Expression *branch = parseExp();
			if (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_THEN)
				error("If(then)", SGS_SE_INCOMPLETE);
			else proc++;

			BlockStmt *taken = parseBlock();
			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_ELSE) {
				proc++;
				BlockStmt *untaken = parseUntaken();
				stmts.push_back(new IfStmt(branch, taken, untaken));
			}
			else {
				stmts.push_back(new IfStmt(branch, taken, NULL));
			}

			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_END) {
				proc++;
				if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_IF)
					proc++;
				else error("End if", SGS_SE_INCOMPLETE);
			}
			else error("If(end)", SGS_SE_INCOMPLETE);

			continue;
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LOOP) {
			proc++;

			Expression *cond;
			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_WHEN) {
				proc++;
				cond = parseExp();
			}
			else error("Loop(when)", SGS_SE_INCOMPLETE);

			stmts.push_back(new WhileStmt(cond, parseBlock()));
			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_END) {
				proc++;
				if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LOOP)
					proc++;
				else
					error("End loop", SGS_SE_INCOMPLETE);
			}
			else error("Loop(end)", SGS_SE_INCOMPLETE);

			continue;
		}
		else if (content[proc].type == SGS_TT_USER) {
			int funcIdx;
			string newVar;
			if ((funcIdx = findDef()) != -1) {
				stmts.push_back(new CallStmt(((FuncDef *)stmts[funcIdx]), parseParam()));
			}
			else error("Function", SGS_SE_NOID);
			continue;
		}
		else if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_DOT) {
			proc++;
			continue;
		}
	}
}

void SgsSyntax::skipLine() {
	int tmpLine = content[proc].line;
	while (proc < content.size() && content[proc].line == tmpLine)proc++;
}
void SgsSyntax::parseLib(string lib) {

}
Expression *SgsSyntax::parseExp() {
	std::stack<int> op;
	std::stack<varNode*> value;

	while (proc < content.size()) {
		if (content[proc].type == SGS_TT_SYS && content[proc].id != SGS_ID_RESULT &&
			content[proc].id != SGS_ID_FALSE && content[proc].id != SGS_ID_TRUE)break;
		else if (content[proc].type == SGS_TT_OP &&
			(content[proc].id == SGS_OP_DOT || content[proc].id == SGS_OP_COMMA ||
				content[proc].id == SGS_OP_LPARENTHESIS || content[proc].id == SGS_OP_RPARENTHESIS ||
				content[proc].id == SGS_OP_LBRAKET || content[proc].id == SGS_OP_RBRAKET))break;
		else if (content[proc].type == SGS_TT_USER ||
			content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_RESULT) {
			int pre = proc;

			int varIdx;
			int classIdx;

			if ((classIdx = findClass()) >= 0) {
				if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_LBRAKET) {
					proc++;
					classNode *tmp = new classNode(globeDec[classIdx], "");

					for (int i = 0; i < ((classDec*)tmp->declaration)->varList.size(); i++) {
						tmp->varList.push_back(parseExp());
						if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							continue;
						}
						if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_RBRAKET) {
							proc++;
							break;
						}
					}

					value.push(tmp);
				}
				else
					error(globeDec[classIdx]->name.data(), SGS_SE_INCOMPLETE);
			}
			else {
				int matched = 0;
				if (func != -1) {
					varIdx = findVar();
					if (varIdx != -1) {
						value.push(((funcNode *)globeVar[((funcDec *)globeDec[func])->varIdx])->localVar[varIdx]);
						matched = 1;
					}
				}
				if (!matched) {
					if (globeVar[varIdx = findVar()]->declaration->type == SGS_DT_FUNCTION) {
						proc = pre;
						if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_ITSELF) {
							value.push(globeVar[varIdx]);
						}
						else {
							varNode *tmp = new varNode(new varDec(SGS_DT_FUNCTION, ""), "");
							tmp->left = globeVar[varIdx];
							tmp->right = parseParam(varIdx);

							value.push(tmp);
						}
					}
					else {
						value.push(globeVar[varIdx]);
					}
				}
			}
		}
		else if (content[proc].type == SGS_TT_DATA) {
			primNode *tmp = new primNode();
			if (content[proc].s != NULL) {
				tmp->declaration->type = SGS_DT_STRING;
				tmp->value = malloc(strlen(content[proc].s) + 1);
				strcpy((char *)tmp->value, content[proc].s);
			}
			else {
				switch (content[proc].id) {
				case CT_INT:
					tmp->declaration->type = SGS_DT_INTEGER;
					tmp->value = new int((int)content[proc].value);
					break;
				case CT_FLOAT:
					tmp->declaration->type = SGS_DT_FLOAT;
					tmp->value = new float((float)content[proc].value);
					break;
				case CT_CHAR:
					tmp->declaration->type = SGS_DT_CHAR;
					tmp->value = new char((char)content[proc].value);
					break;
				}
			}

			value.push(tmp);
			proc++;
		}
		else if (content[proc].type == SGS_TT_SYS &&
			(content[proc].id == SGS_ID_FALSE || content[proc].id == SGS_ID_TRUE)) {
			primNode *tmp = new primNode();
			tmp->declaration->type = SGS_DT_BOOL;
			if (content[proc].id == SGS_ID_FALSE)
				tmp->value = new bool(false);
			else if (content[proc].id == SGS_ID_TRUE)
				tmp->value = new bool(true);
			value.push(tmp);
			proc++;
		}
		else if (content[proc].type == SGS_TT_OP) {
			if (content[proc].id == SGS_OP_LBRACE) {
				op.push(SGS_OP_LBRACE);
			}
			else if (content[proc].id == SGS_OP_RBRACE) {
				while (op.top() != SGS_OP_LBRACE) {
					int o = op.top();
					op.pop();
					if (value.size() < 2)
						error("", SGS_SE_INCOMPLETE);
					varNode *a = value.top();
					value.pop();
					varNode *b = value.top();
					value.pop();

					varNode *tmp = new varNode(new varDec(SGS_DT_EXP, ""), "");
					tmp->op = (SGSOPERATOR)o;
					tmp->left = a;
					tmp->right = b;
					value.push(tmp);
				}
				op.pop();
			}
			else {
				if (op.empty()) {
					op.push(content[proc].id);
				}
				else {
					while (!op.empty() && compare(content[proc].id, op.top())) {
						int o = op.top();
						op.pop();
						if (value.size() < 2)
							error("", SGS_SE_INCOMPLETE);
						varNode *a = value.top();
						value.pop();
						varNode *b = value.top();
						value.pop();

						varNode *tmp = new varNode(new varDec(SGS_DT_EXP, ""), "");
						tmp->op = (SGSOPERATOR)o;
						tmp->left = a;
						tmp->right = b;
						value.push(tmp);
					}
					op.push(content[proc].id);
				}
			}
			proc++;
		}
		else break;
	}

	while (!op.empty()) {
		int o = op.top();
		op.pop();
		if (value.size() < 2)
			error("", SGS_SE_INCOMPLETE);
		varNode *a = value.top();
		value.pop();
		varNode *b = value.top();
		value.pop();

		varNode *tmp = new varNode(new varDec(SGS_DT_EXP, ""), "");
		tmp->op = (SGSOPERATOR)o;
		tmp->left = a;
		tmp->right = b;
		value.push(tmp);
	}

	if (value.size() == 1) {
		return value.top();
	}
	else {
		arrayNode *arr = new arrayNode(new arrayDec(SGS_DT_ARRAY, "", value.top()->declaration),
			"", std::vector<varNode *>());

		while (!value.empty()) {
			if (value.top()->declaration != arr->declaration)
				error("array", SGS_SE_INVALIDTYPE);
			arr->content.push_back(value.top());
			value.pop();
		}
		return arr;
	}
	return NULL;
}
TypeDef *SgsSyntax::parseClassDec() {
	return NULL;
}
FuncProto *SgsSyntax::parseFuncDec() {
	funcDec *ret = new funcDec();
	ret->type = SGS_DT_FUNCTION;
	ret->name = parseUser();

	if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_WITH) {
		proc++;

		int type, cla;
		while (true) {
			if ((cla = findClass()) != -1) {
				ret->varList.push_back(new varNode(globeDec[cla], parseUser()));
			}
			else if ((type = findType()) != -1) {
				ret->varList.push_back(new varNode(new varDec((decType)type, ""), parseUser()));
			}
			else if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA)proc++;
			else break;
		}
	}

	if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_RETURN) {
		int type, cla;
		if ((cla = findClass()) != -1) {
			ret->result = globeDec[cla];
		}
		else if ((type = findType()) != -1) {
			ret->result = new varDec((decType)type, "");
		}
	}
	return ret;
}
FuncDef *SgsSyntax::parseFuncDef(int funcid) {
	int preFunc = func;
	func = funcid;
	funcNode *tmpFunc = new funcNode(globeDec[funcid], globeDec[funcid]->name, NULL, vector<varNode *>());
	tmpFunc->localVar.push_back(new varNode(globeDec[funcid], "result"));
	for (auto v : ((funcDec *)globeDec[funcid])->varList)
		tmpFunc->localVar.push_back(v);

	stateNode *ret = new stateNode();
	while (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_END) {
		if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LET) {
			proc++;

			varNode *lvalue;
			int varType;
			int classIdx = -1;
			if (content[proc].type == SGS_TT_USER) {
				if ((classIdx = findClass()) >= 0) {
					tmpFunc->localVar.push_back(lvalue = new classNode(globeDec[classIdx], parseUser()));
				}
				else {
					lvalue = parseExp();
				}
			}
			else if (content[proc].type == SGS_TT_SYS) {
				if ((varType = findType()) != -1) {
					tmpFunc->localVar.push_back(lvalue = new primNode(new primDec(varType), parseUser()));
				}
				else {
					error(strId[content[proc].id].data(), SGS_SE_UNIQUE);
					skipLine();
				}
			}
			else {
				error("Let", SGS_SE_UNIQUE);
				skipLine();
			}

			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_BE) {
				proc++;
				stateSeq statement = stateSeq(VO_ASSIGN);
				statement.left = lvalue;
				statement.right = parseExp();
				ret->statements.push_back(statement);
			}

			continue;
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_IF) {
			proc++;
			varNode *branch = parseExp();
			if (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_THEN)
				error("If(then)", SGS_SE_INCOMPLETE);
			else proc++;

			stateSeq statement = stateSeq(VO_IF);
			statement.left = branch;
			statement.right = new varNode(new varDec(SGS_DT_EXP, ""), "");
			((varNode *)statement.right)->left = parseBlock();
			stateNode *reject = new stateNode();
			((varNode *)statement.right)->right = reject;
			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_ELSE) {
				proc++;
				int last = 1;
				while (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_IF) {
					proc++;
					stateSeq statement = stateSeq(VO_IF);
					statement.left = parseExp();
					statement.right = new varNode(new varDec(SGS_DT_EXP, ""), "");
					if (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_THEN)
						error("If(then)", SGS_SE_INCOMPLETE);
					proc++;
					((varNode *)statement.right)->left = parseBlock();
					reject->statements.push_back(statement);
					reject = new stateNode();
					((varNode *)statement.right)->right = reject;

					if (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_ELSE) {
						last = 0;
						break;
					}
					proc++;
				}
				if (last)reject->statements = parseBlock()->statements;
			}

			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_END) {
				proc++;
				if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_IF)
					proc++;
				else error("End if", SGS_SE_INCOMPLETE);
			}
			else error("If(end)", SGS_SE_INCOMPLETE);

			ret->statements.push_back(statement);
			continue;
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LOOP) {
			proc++;
			varNode *cond = NULL;
			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_WHEN) {
				proc++;
				cond = parseExp();
			}
			else error("Loop(when)", SGS_SE_INCOMPLETE);

			stateSeq statement = stateSeq(VO_WHILE);
			statement.left = cond;
			statement.right = parseBlock();

			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_END) {
				proc++;
				if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LOOP)
					proc++;
				else
					error("End loop", SGS_SE_INCOMPLETE);
			}
			else error("Loop(end)", SGS_SE_INCOMPLETE);

			ret->statements.push_back(statement);
			continue;
		}
		else if (content[proc].type == SGS_TT_USER) {
			int funcIdx;
			string newVar;
			if ((funcIdx = findProto()) != -1) {
				stateSeq statement = stateSeq(VO_CALL);
				statement.left = globeDec[funcIdx];
				statement.right = parseParam(funcIdx);

				ret->statements.push_back(statement);
			}
			else error("Function", SGS_SE_NOID);
			continue;
		}
		else if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_DOT) {
			proc++;
			continue;
		}
	}
	proc++;
	tmpFunc->content = ret;

	func = preFunc;
	globeVar.push_back(tmpFunc);
}
vector<sgs::Expression *>SgsSyntax::parseParam() {

}
BlockStmt *SgsSyntax::parseBlock() {
	return NULL;
}
sgs::BlockStmt *SgsSyntax::parseUntaken() {

}

string SgsSyntax::parseUser() {
	return string();
}
int SgsSyntax::findVar() {

}
int SgsSyntax::findClass() {

}
int SgsSyntax::findProto() {

}
int SgsSyntax::findDef() {

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
void SgsSyntax::error(const char *inst, SGSYNTAXERROR type) {
	switch (type) {
	case SGS_SE_EXPOSE:
		msgList.push_back(sgsMsg(string("语句块不封闭。\n"), MT_ERROR));
	case SGS_SE_UNIQUE:
		msgList.push_back(sgsMsg(inst + string("无此用法。\n"), MT_ERROR));
	case SGS_SE_EXPDOT:
		msgList.push_back(sgsMsg(string("缺少句号。\n"), MT_ERROR));
	case SGS_SE_EXPCOMMA:
		msgList.push_back(sgsMsg(inst + string("缺少逗号。\n"), MT_ERROR));
	case SGS_SE_EXPBRACE:
		msgList.push_back(sgsMsg(string("括号不完整。\n"), MT_ERROR));
	case SGS_SE_REDEF:
		msgList.push_back(sgsMsg(inst + string("重定义。\n"), MT_WARNING));
	case SGS_SE_INVALIDTYPE:
		msgList.push_back(sgsMsg(inst + string("操作对象类型错误。\n"), MT_WARNING));
	case SGS_SE_DISACCORD:
		msgList.push_back(sgsMsg(inst + string("前后不一致。\n"), MT_WARNING));
	case SGS_SE_NOID:
		msgList.push_back(sgsMsg(inst + string("未定义。\n"), MT_ERROR));
	case SGS_SE_INCOMPLETE:
		msgList.push_back(sgsMsg(inst + string("语句不完整。\n"), MT_WARNING));
	case SGS_SE_UNKNOWN:
		msgList.push_back(sgsMsg(string("未知错误。\n"), MT_ERROR));
	case SGS_SE_UNSUPPORT:
		msgList.push_back(sgsMsg(string("暂不支持。\n"), MT_ERROR));
	}
}
