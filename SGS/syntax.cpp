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
	vector<std::pair<VarType *, string>> piParam;
	piParam.push_back(std::pair<VarType *, string>(new BasicType(BT_INT), "value"));
	stmts.push_back(new FuncProto(nullptr, "print an int", piParam));
	funcList.push_back(new FuncProto(nullptr, "print an int", piParam));

	vector<std::pair<VarType *, string>> pnParam;
	pnParam.push_back(std::pair<VarType *, string>(new BasicType(BT_FLOAT), "value"));
	stmts.push_back(new FuncProto(nullptr, "print a number", pnParam));
	funcList.push_back(new FuncProto(nullptr, "print a number", pnParam));

	vector<std::pair<VarType *, string>> pbParam;
	pbParam.push_back(std::pair<VarType *, string>(new BasicType(BT_BOOL), "value"));
	stmts.push_back(new FuncProto(nullptr, "print a boolean", pbParam));
	funcList.push_back(new FuncProto(nullptr, "print a boolean", pbParam));

	vector<std::pair<VarType *, string>> psParam;
	psParam.push_back(std::pair<VarType *, string>(new BasicType(BT_STRING), "value"));
	stmts.push_back(new FuncProto(nullptr, "print a str", psParam));
	funcList.push_back(new FuncProto(nullptr, "print a str", psParam));
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
				ClassDef *tmpClass = parseClassDec();
				classList.push_back((ClassType *)tmpClass->getDecType());
				stmts.push_back(tmpClass);
			}
			else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_FUNCTION) {
				proc++;
				FuncProto *tmpFunc = parseFuncDec();
				funcList.push_back(tmpFunc);
				stmts.push_back(tmpFunc);
			}
			else
				error("New", SGS_SE_UNIQUE);
			continue;
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LET) {
			proc++;

			Expression *left;
			int classIdx = -1;
			string newVar;
			if (content[proc].type == SGS_TT_USER) {
				if ((classIdx = findClass()) >= 0) {
					stmts.push_back(new TypeDef(classList[classIdx], newVar = parseUser()));
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
						int length = (int)content[proc].value;
						proc++;
						stmts.push_back(new TypeDef(new ArrayType(new BasicType(BT_INT), length), newVar = parseUser()));
					}
					else {
						proc++;
						stmts.push_back(new TypeDef(new BasicType(BT_INT), newVar = parseUser()));
					}
					left = new IdExp(newVar);
				}
				else if(content[proc].id == SGS_ID_FLOAT) {
					if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
						proc += 2;
						int length = (int)content[proc].value;
						proc++;
						stmts.push_back(new TypeDef(new ArrayType(new BasicType(BT_FLOAT), length), newVar = parseUser()));
					}
					else {
						proc++;
						stmts.push_back(new TypeDef(new BasicType(BT_FLOAT), newVar = parseUser()));
					}
					left = new IdExp(newVar);
				}
				else if (content[proc].id == SGS_ID_BOOL) {
					if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
						proc += 2;
						int length = (int)content[proc].value;
						proc++;
						stmts.push_back(new TypeDef(new ArrayType(new BasicType(BT_BOOL), length), newVar = parseUser()));
					}
					else {
						proc++;
						stmts.push_back(new TypeDef(new BasicType(BT_BOOL), newVar = parseUser()));
					}
					left = new IdExp(newVar);
				}
				else if (content[proc].id == SGS_ID_CHAR) {
					error("char", SGS_SE_UNSUPPORT);
					skipLine();
					continue;
				}
				else if (content[proc].id == SGS_ID_STRING) {
					if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
						proc += 2;
						int length = (int)content[proc].value;
						proc++;
						stmts.push_back(new TypeDef(new ArrayType(new BasicType(BT_STRING), length), newVar = parseUser()));
					}
					else {
						proc++;
						stmts.push_back(new TypeDef(new BasicType(BT_STRING), newVar = parseUser()));
					}
					left = new IdExp(newVar);
				}
				else {
					error(strId[content[proc].id].data(), SGS_SE_UNIQUE);
					skipLine();
					continue;
				}
			}
			else {
				error("Let", SGS_SE_UNIQUE);
				skipLine();
				continue;
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
			if ((funcIdx = findFunc()) >= 0) {
				stmts.push_back(parseFuncDef(funcIdx));
				if (funcIdx != findFunc())
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
				if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_IF) {
					proc++;
					stmts.push_back(new IfStmt(branch, taken, parseBlock(true)));
				}
				else stmts.push_back(new IfStmt(branch, taken, parseBlock()));
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
			else {
				error("Loop(when)", SGS_SE_INCOMPLETE);
				cond = parseExp();
			}

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
			if ((funcIdx = findFunc()) != -1) {
				if (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_WITH)
					error(funcList[funcIdx]->getName().data(), SGS_SE_INCOMPLETE);
				else proc++;
				stmts.push_back(new CallStmt(funcList[funcIdx], parseParam(funcIdx)));
			}
			else error("Function", SGS_SE_NOID);
			continue;
		}
		else if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_DOT) {
			proc++;
			continue;
		}
		else {
			error("", SGS_SE_UNKNOWN);
			skipLine();
			continue;
		}
	}
	return;
}

void SgsSyntax::skipLine() {
	int tmpLine = content[proc].line;
	while (proc < content.size() && content[proc].line == tmpLine)proc++;
}
void SgsSyntax::parseLib(string lib) {

}
Expression *SgsSyntax::parseExp() {
	std::stack<int> op;
	std::stack<Expression*> value;

	while (proc < content.size()) {
		if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA)continue;
		else if (content[proc].type == SGS_TT_SYS && content[proc].id != SGS_ID_RESULT &&
			content[proc].id != SGS_ID_FALSE && content[proc].id != SGS_ID_TRUE)break;
		else if (content[proc].type == SGS_TT_OP && (content[proc].id == SGS_OP_DOT ||
				content[proc].id == SGS_OP_LPARENTHESIS || content[proc].id == SGS_OP_RPARENTHESIS ||
				content[proc].id == SGS_OP_LBRAKET || content[proc].id == SGS_OP_RBRAKET))break;
		else if (content[proc].type == SGS_TT_USER ||
			content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_RESULT) {
			int pre = proc;

			int classIdx;
			int funcIdx;

			if ((classIdx = findClass()) >= 0) {
				if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_LBRAKET) {
					proc++;
					value.push(parseClassConst(classIdx));
					if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_RBRAKET) {
						proc++;
					}
					else {
						error(classList[classIdx]->getName().data(), SGS_SE_EXPBRACE);
					}
				}
				else
					error(classList[classIdx]->getName().data(), SGS_SE_INCOMPLETE);
			}
			else if ((funcIdx = findFunc()) >= 0) {
				if (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_WITH)
					error(funcList[funcIdx]->getName().data(), SGS_SE_INCOMPLETE);
				else proc++;
				value.push(new CallExp(funcList[funcIdx], parseParam(funcIdx)));
			}
			else {
				value.push(parseVar());
			}
		}
		else if (content[proc].type == SGS_TT_DATA) {
			if (content[proc].s != NULL) {
				value.push(new StrLiteral(content[proc].s));
			}
			else {
				switch (content[proc].id) {
				case CT_INT:
					value.push(new IntLiteral((int)content[proc].value));
					break;
				case CT_FLOAT:
					value.push(new FloatLiteral(content[proc].value));
					break;
				}
			}

			proc++;
		}
		else if (content[proc].type == SGS_TT_SYS &&
			(content[proc].id == SGS_ID_FALSE || content[proc].id == SGS_ID_TRUE)) {
			if (content[proc].id == SGS_ID_FALSE)
				value.push(new BoolLiteral(false));
			else if (content[proc].id == SGS_ID_TRUE)
				value.push(new BoolLiteral(true));

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
					Expression *a = value.top();
					value.pop();
					Expression *b = value.top();
					value.pop();

					OpExp *tmp = new OpExp((SGSOPERATOR)o);
					tmp->setLeft(a);
					tmp->setRight(b);
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
						Expression *a = value.top();
						value.pop();
						Expression *b = value.top();
						value.pop();

						OpExp *tmp = new OpExp((SGSOPERATOR)o);
						tmp->setLeft(a);
						tmp->setRight(b);
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
		Expression *a = value.top();
		value.pop();
		Expression *b = value.top();
		value.pop();

		OpExp *tmp = new OpExp((SGSOPERATOR)o);
		tmp->setLeft(a);
		tmp->setRight(b);
		value.push(tmp);
	}

	if (value.size() == 1) {
		return value.top();
	}
	else {
		VarType *t = value.top()->getResType();
		vector<Expression *> cont;

		while (!value.empty()) {
			if (value.top()->getResType() != t)
				error("array", SGS_SE_INVALIDTYPE);
			cont.push_back(value.top());
			value.pop();
		}
		return new ArrayLiteral(t, cont);
	}
}
Expression *SgsSyntax::parseVar() {
	Expression *ret = NULL;

	while (proc < content.size()) {
		if (content[proc].type == SGS_TT_USER ||
			content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_RESULT) {
			ret = new IdExp(parseUser());
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_OF) {
			proc++;
			ret = new AccessExp(ret, parseUser());
		}
		else if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_LPARENTHESIS) {
			proc++;
			ret = new VisitExp(ret, parseExp());
			if (content[proc].type != SGS_TT_OP || content[proc].id == SGS_OP_RPARENTHESIS)
				error("array index", SGS_SE_EXPBRACE);
			else proc++;
		}
		else break;
	}

	return ret;
}
ClassDef *SgsSyntax::parseClassDec() {
	string name = parseUser();
	vector<std::pair<VarType *, string>> elements;

	int classIdx;
	if (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_WITH)
		error("new class", SGS_SE_UNIQUE);
	proc++;
	while (proc < content.size()) {
		if (content[proc].type == SGS_TT_USER) {
			if ((classIdx = findClass()) >= 0) {
				elements.push_back(std::pair<VarType *, string>(classList[classIdx], parseUser()));
			}
			else error(parseUser().data(), SGS_SE_NOID);
		}
		else if (content[proc].type == SGS_TT_SYS) {
			if (content[proc].id == SGS_ID_INTEGER) {
				elements.push_back(std::pair<VarType *, string>(new BasicType(BT_INT), parseUser()));
			}
			else if (content[proc].id == SGS_ID_FLOAT) {
				elements.push_back(std::pair<VarType *, string>(new BasicType(BT_FLOAT), parseUser()));
			}
			else if (content[proc].id == SGS_ID_BOOL) {
				elements.push_back(std::pair<VarType *, string>(new BasicType(BT_BOOL), parseUser()));
			}
			else if (content[proc].id == SGS_ID_CHAR) {
				error("char", SGS_SE_UNSUPPORT);
				skipLine();
			}
			else if (content[proc].id == SGS_ID_STRING) {
				elements.push_back(std::pair<VarType *, string>(new BasicType(BT_STRING), parseUser()));
			}
			else break;
		}
		else break;
	}
	return new ClassDef(new ClassType(name, elements));
}
ClassLiteral *SgsSyntax::parseClassConst(int classid) {
	vector<Expression *> ele(classList[classid]->getEle().size());
	vector<string> name;
	for (auto item : classList[classid]->getEle()) {
		name.push_back(item.second);
	}
	bool hide = false;
	for (unsigned int i = 0; i < classList[classid]->getEle().size(); i++) {
		int idx = parseUser(name);
		if (idx == -1) {
			if (i == 0)hide = true;
			else error(parseUser().data(), SGS_SE_NOID);
		}
		if (hide) {
			ele[i] = parseExp();
			break;
		}
		else {
			ele[idx] = parseExp();
		}
	}
	return new ClassLiteral(classList[classid]->getName(), ele);
}
FuncProto *SgsSyntax::parseFuncDec() {
	string name = parseUser();
	vector<std::pair<VarType *, string>> params;

	int classIdx;
	if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_WITH) {
		proc++;
		while (proc < content.size()) {
			if (content[proc].type == SGS_TT_USER) {
				if ((classIdx = findClass()) >= 0) {
					params.push_back(std::pair<VarType *, string>(classList[classIdx], parseUser()));
				}
				else error(parseUser().data(), SGS_SE_NOID);
			}
			else if (content[proc].type == SGS_TT_SYS) {
				if (content[proc].id == SGS_ID_INTEGER) {
					params.push_back(std::pair<VarType *, string>(new BasicType(BT_INT), parseUser()));
				}
				else if (content[proc].id == SGS_ID_FLOAT) {
					params.push_back(std::pair<VarType *, string>(new BasicType(BT_FLOAT), parseUser()));
				}
				else if (content[proc].id == SGS_ID_BOOL) {
					params.push_back(std::pair<VarType *, string>(new BasicType(BT_BOOL), parseUser()));
				}
				else if (content[proc].id == SGS_ID_CHAR) {
					error("char", SGS_SE_UNSUPPORT);
					skipLine();
				}
				else if (content[proc].id == SGS_ID_STRING) {
					params.push_back(std::pair<VarType *, string>(new BasicType(BT_STRING), parseUser()));
				}
				else break;
			}
			else break;
		}
	}
	if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_RETURN) {
		if (content[proc].type == SGS_TT_USER) {
			if ((classIdx = findClass()) >= 0) {
				return new FuncProto(classList[classIdx], name, params);
			}
			else {
				error(parseUser().data(), SGS_SE_NOID);
				return new FuncProto(nullptr, name, params);
			}
		}
		else if (content[proc].type == SGS_TT_SYS) {
			if (content[proc].id == SGS_ID_INTEGER) {
				return new FuncProto(new BasicType(BT_INT), name, params);
			}
			else if (content[proc].id == SGS_ID_FLOAT) {
				return new FuncProto(new BasicType(BT_FLOAT), name, params);
			}
			else if (content[proc].id == SGS_ID_BOOL) {
				return new FuncProto(new BasicType(BT_BOOL), name, params);
			}
			else if (content[proc].id == SGS_ID_CHAR) {
				error("char", SGS_SE_UNSUPPORT);
				skipLine();
				return new FuncProto(nullptr, name, params);
			}
			else if (content[proc].id == SGS_ID_STRING) {
				return new FuncProto(new BasicType(BT_STRING), name, params);
			}
			else {
				error(parseUser().data(), SGS_SE_NOID);
				return new FuncProto(nullptr, name, params);
			}
		}
		else {
			error("", SGS_SE_UNKNOWN);
			return new FuncProto(nullptr, name, params);
		}
	}
	else {
		error(name.data(), SGS_SE_INCOMPLETE);
		return new FuncProto(nullptr, name, params);
	}
}
FuncDef *SgsSyntax::parseFuncDef(int funcid) {
	FuncDef *ret = new FuncDef(funcList[funcid]);
	ret->setBody(parseBlock());
	return ret;
}
vector<Expression *> SgsSyntax::parseParam(int funcid) {
	vector<Expression *> para(funcList[funcid]->getParam().size());
	vector<string> name;
	for (auto item : funcList[funcid]->getParam()) {
		name.push_back(item.second);
	}
	bool hide = false;
	for (unsigned int i = 0; i < funcList[funcid]->getParam().size(); i++) {
		int idx = parseUser(name);
		if (idx == -1) {
			if(i == 0)hide = true;
			else error(parseUser().data(), SGS_SE_NOID);
		}
		if (hide) {
			para[i] = parseExp();
			break;
		}
		else {
			para[idx] = parseExp();
		}
	}

	return para;
}
BlockStmt *SgsSyntax::parseBlock(bool untaken) {
	BlockStmt *block = new BlockStmt();
	while (proc < content.size()) {
		if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LET) {
			proc++;

			Expression *left;
			int classIdx = -1;
			string newVar;
			if (content[proc].type == SGS_TT_USER) {
				if ((classIdx = findClass()) >= 0) {
					block->pushAST(new TypeDef(classList[classIdx], newVar = parseUser()));
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
						int length = (int)content[proc].value;
						proc++;
						block->pushAST(new TypeDef(new ArrayType(new BasicType(BT_INT), length), newVar = parseUser()));
					}
					else {
						proc++;
						block->pushAST(new TypeDef(new BasicType(BT_INT), newVar = parseUser()));
					}
					left = new IdExp(newVar);
				}
				else if (content[proc].id == SGS_ID_FLOAT) {
					if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
						proc += 2;
						int length = (int)content[proc].value;
						proc++;
						block->pushAST(new TypeDef(new ArrayType(new BasicType(BT_FLOAT), length), newVar = parseUser()));
					}
					else {
						proc++;
						block->pushAST(new TypeDef(new BasicType(BT_FLOAT), newVar = parseUser()));
					}
					left = new IdExp(newVar);
				}
				else if (content[proc].id == SGS_ID_BOOL) {
					if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
						proc += 2;
						int length = (int)content[proc].value;
						proc++;
						block->pushAST(new TypeDef(new ArrayType(new BasicType(BT_BOOL), length), newVar = parseUser()));
					}
					else {
						proc++;
						block->pushAST(new TypeDef(new BasicType(BT_BOOL), newVar = parseUser()));
					}
					left = new IdExp(newVar);
				}
				else if (content[proc].id == SGS_ID_CHAR) {
					error("char", SGS_SE_UNSUPPORT);
					skipLine();
					continue;
				}
				else if (content[proc].id == SGS_ID_STRING) {
					if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
						proc += 2;
						int length = (int)content[proc].value;
						proc++;
						block->pushAST(new TypeDef(new ArrayType(new BasicType(BT_STRING), length), newVar = parseUser()));
					}
					else {
						proc++;
						block->pushAST(new TypeDef(new BasicType(BT_STRING), newVar = parseUser()));
					}
					left = new IdExp(newVar);
				}
				else {
					error(strId[content[proc].id].data(), SGS_SE_UNIQUE);
					skipLine();
					continue;
				}
			}
			else {
				error("Let", SGS_SE_UNIQUE);
				skipLine();
				continue;
			}

			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_BE) {
				proc++;
				block->pushAST(new AssignStmt(left, parseExp()));
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
				if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_IF) {
					proc++;
					block->pushAST(new IfStmt(branch, taken, parseBlock(true)));
				}
				else block->pushAST(new IfStmt(branch, taken, parseBlock()));
			}
			else {
				block->pushAST(new IfStmt(branch, taken, NULL));
			}

			if (!untaken) {
				if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_END) {
					proc++;
					if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_IF)
						proc++;
					else error("End if", SGS_SE_INCOMPLETE);
				}
				else error("If(end)", SGS_SE_INCOMPLETE);
			}

			if (untaken)break;
			else continue;
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LOOP) {
			proc++;

			Expression *cond;
			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_WHEN) {
				proc++;
				cond = parseExp();
			}
			else {
				error("Loop(when)", SGS_SE_INCOMPLETE);
				cond = parseExp();
			}

			block->pushAST(new WhileStmt(cond, parseBlock()));
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
			if ((funcIdx = findFunc()) != -1) {
				if (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_WITH)
					error(funcList[funcIdx]->getName().data(), SGS_SE_INCOMPLETE);
				else proc++;
				block->pushAST(new CallStmt(funcList[funcIdx], parseParam(funcIdx)));
			}
			else error("Function", SGS_SE_NOID);
			continue;
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_RETURN) {
			proc++;
			block->pushAST(new ReturnStmt());
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_BREAK) {
			proc++;
			block->pushAST(new BreakStmt());
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_REDO) {
			proc++;
			block->pushAST(new ContinueStmt());
		}
		else if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_DOT) {
			proc++;
			continue;
		}
		else {
			error("", SGS_SE_UNKNOWN);
			skipLine();
			continue;
		}
	}
	return block;
}

string SgsSyntax::parseUser(string guide) {
	string tmp;
	while (proc < content.size() && content[proc].type == SGS_TT_USER) {
		tmp += strId[content[proc++].id];
		if (tmp == guide)break;
		tmp += " ";
	}
	if (proc == content.size()) {
		error(tmp.data(), SGS_SE_INCOMPLETE);
	}
	tmp.pop_back();
	return tmp;
}
int SgsSyntax::parseUser(vector<string> guides) {
	string tmp;
	int pre = proc;
	while (proc < content.size() && content[proc].type == SGS_TT_USER) {
		tmp += strId[content[proc++].id];
		for (unsigned int i = 0; i < guides.size(); i++) {
			if (tmp == guides[i])return i;
		}
		tmp += " ";
	}
	if (proc == content.size()) {
		error(tmp.data(), SGS_SE_INCOMPLETE);
	}
	else {
		error(tmp.data(), SGS_SE_NOID);
	}
	proc = pre;
	return -1;
}
int SgsSyntax::findClass() {
	string tmp;
	int pre = proc;
	while (proc < content.size() && content[proc].type == SGS_TT_USER) {
		tmp += strId[content[proc++].id];
		for (unsigned int i = 0; i < classList.size(); i++) {
			if (tmp == classList[i]->getName())return i;
		}
		tmp += " ";
	}
	if (proc == content.size()) {
		error(tmp.data(), SGS_SE_INCOMPLETE);
	}
	else error(tmp.data(), SGS_SE_NOID);
	proc = pre;
	return -1;
}
int SgsSyntax::findFunc() {
	string tmp;
	int pre = proc;
	while (proc < content.size() && content[proc].type == SGS_TT_USER) {
		tmp += strId[content[proc++].id];
		for (unsigned int i = 0; i < funcList.size(); i++) {
			if (tmp == funcList[i]->getName())return i;
		}
		tmp += " ";
	}
	if (proc == content.size()) {
		error(tmp.data(), SGS_SE_INCOMPLETE);
	}
	else error(tmp.data(), SGS_SE_NOID);
	proc = pre;
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
void SgsSyntax::error(const char *inst, SGSYNTAXERROR type) {
	switch (type) {
	case SGS_SE_EXPOSE:
		msgList.push_back(sgsMsg(string("语句块不封闭。\n"), MT_ERROR));
		break;
	case SGS_SE_UNIQUE:
		msgList.push_back(sgsMsg(inst + string("无此用法。\n"), MT_ERROR));
		break;
	case SGS_SE_EXPDOT:
		msgList.push_back(sgsMsg(string("缺少句号。\n"), MT_ERROR));
		break;
	case SGS_SE_EXPCOMMA:
		msgList.push_back(sgsMsg(inst + string("缺少逗号。\n"), MT_ERROR));
		break;
	case SGS_SE_EXPBRACE:
		msgList.push_back(sgsMsg(string("括号不完整。\n"), MT_ERROR));
		break;
	case SGS_SE_REDEF:
		msgList.push_back(sgsMsg(inst + string("重定义。\n"), MT_WARNING));
		break;
	case SGS_SE_INVALIDTYPE:
		msgList.push_back(sgsMsg(inst + string("操作对象类型错误。\n"), MT_WARNING));
		break;
	case SGS_SE_DISACCORD:
		msgList.push_back(sgsMsg(inst + string("前后不一致。\n"), MT_WARNING));
		break;
	case SGS_SE_NOID:
		msgList.push_back(sgsMsg(inst + string("未定义。\n"), MT_ERROR));
		break;
	case SGS_SE_INCOMPLETE:
		msgList.push_back(sgsMsg(inst + string("语句不完整。\n"), MT_WARNING));
		break;
	case SGS_SE_UNKNOWN:
		msgList.push_back(sgsMsg(string("未知错误。\n"), MT_ERROR));
		break;
	case SGS_SE_UNSUPPORT:
		msgList.push_back(sgsMsg(string("暂不支持。\n"), MT_ERROR));
		break;
	}
}
