#define _CRT_SECURE_NO_WARNINGS
#include "syntax.h"
#include <iostream>
#include <fstream>

using namespace sgs;

Syntax::Syntax() {
    proc = 0;
}
Syntax::Syntax(vector<string> &ids, vector<TokenPrim> &input) {
    proc = 0;

    content = input;
    strId = ids;
}
Syntax::~Syntax() = default;

bool Syntax::compareOp(int op1, int op2) {//op1<=op2
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
	default:
		prio1 = -1;
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
	default:
		prio2 = -1;
	}
	return prio1 <= prio2;
}

Syntax *Syntax::input(vector<string> &ids, vector<TokenPrim> &src) {
    content = src;
	stmts.clear();

    for (const auto& i : ids) {
        unsigned int j = 0;
        for (; j < strId.size(); j++) {
            if (i == strId[j])break;
        }
        if (j == strId.size())strId.push_back(i);
    }
    return this;
}
void Syntax::parse() {
    proc = 0;

    while (proc < content.size()) {
        if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_USE) {
			int line = content[proc].line;
            proc++;
            if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LIBRARY) {
                proc++;
				if (content[proc].type != SGS_TT_USER)
					error("Use", SGS_SE_UNIQUE, line);
                parseLib(strId[content[proc].id], line);
                proc++;
            }
			else {
                error("", SGS_SE_USELIB, line);
                skipLine();
            }
        } 
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_NEW) {
			int line = content[proc].line;
            proc++;
            if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_CLASS) {
                proc++;
                ClassDef *tmpClass = parseClassDec();
				if (tmpClass) {
					classList.push_back(static_cast<ClassType *>(tmpClass->getDecType()));
					stmts.push_back(tmpClass);
				}
            }
			else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_FUNCTION) {
                proc++;
                FuncProto *tmpFunc = parseFuncDec();
				if (tmpFunc) {
					funcList.push_back(tmpFunc);
					stmts.push_back(tmpFunc);
				}
            }
			else error("", SGS_SE_AFTERNEW, line);
            continue;
        } 
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LET) {
			int line = content[proc].line;
            proc++;

            vector<Expression *>lefts;

			VarType *newType = parseType();
			if (newType) {
				string newVar = parseUser();
				stmts.push_back(new VarDef(line, newType, newVar));
				lefts.push_back(new IdExp(line, newVar));
				while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
					proc++;
					stmts.push_back(new VarDef(line, newType, newVar = parseUser()));
					lefts.push_back(new IdExp(line, newVar));
				}
			}
			else {
				lefts.push_back(parseVar());
				while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
					proc++;
					lefts.push_back(parseVar());
				}
			}

            if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_BE) {
                proc++;

				vector<Expression *>rights;
				rights.push_back(parseExp());
				while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
					proc++;
					rights.push_back(parseExp());
				}

				for (unsigned int i = 0; i < rights.size(); i++) {
					stmts.push_back(new AssignStmt(line, lefts[i], rights[i]));
				}
            }

            continue;
        } 
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_START) {
			int line = content[proc].line;
            proc++;
            int funcIdx, classIdx;
			if ((classIdx = findClass()) != -1) {
				stmts.push_back(parseConstructorDef(classIdx));
				if (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_END)
					error(((ClassType *)classList[classIdx])->getName().data(), SGS_SE_EXPOSE, line);
				proc++;
				if (classIdx != findClass())
					error(((ClassType *)classList[classIdx])->getName().data(), SGS_SE_DISACCORD, line);
			}
            else if ((funcIdx = findFunc()) != -1) {
				stmts.push_back(parseFuncDef(funcIdx));
				if (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_END)
					error(((FuncProto *)funcList[funcIdx])->getName().data(), SGS_SE_EXPOSE, line);
				proc++;
				if (funcIdx != findFunc())
					error(((FuncProto *)funcList[funcIdx])->getName().data(), SGS_SE_DISACCORD, line);
            }
			else {
                error(parseUser().data(), SGS_SE_NOID, line);
                skipLine();
            }
            continue;
        } 
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_IF) {
			int line = content[proc].line;
            proc++;

            Expression *branch = parseExp();
            if (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_THEN)
                error("If(then)", SGS_SE_INCOMPLETE, line);
            else proc++;

            BlockStmt *taken = parseBlock();
            if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_ELSE) {
                proc++;
                if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_IF) {
                    proc++;
                    stmts.push_back(new IfStmt(line, branch, taken, parseBlock(true)));
                }
				else stmts.push_back(new IfStmt(line, branch, taken, parseBlock()));
            }
			else {
                stmts.push_back(new IfStmt(line, branch, taken, nullptr));
            }

            if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_END) {
                proc++;
            }
			else error("If(end)", SGS_SE_INCOMPLETE, line);

            continue;
        } 
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LOOP) {
			int line = content[proc].line;
			proc++;

            Expression *cond;
            if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_WHEN) {
                proc++;
                cond = parseExp();
            }
			else {
                error("Loop(when)", SGS_SE_INCOMPLETE, line);
                cond = parseExp();
            }

            stmts.push_back(new WhileStmt(line, cond, parseBlock()));
            if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_END) {
                proc++;
            }
			else error("Loop(end)", SGS_SE_INCOMPLETE, line);

            continue;
        }
		else if (content[proc].type == SGS_TT_USER) {
			int line = content[proc].line;
			int funcIdx;
            string newVar;
            if ((funcIdx = findFunc()) != -1) {
                if (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_WITH)
                    stmts.push_back(new CallStmt(line, funcList[funcIdx], vector<Expression *>()));
                else {
                    proc++;
                    stmts.push_back(new CallStmt(line, funcList[funcIdx], parseParam(funcIdx)));
                }
            }
			else error(parseUser().data(), SGS_SE_NOID, line);
            continue;
        } 
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_RETURN) {
			int line = content[proc].line;
			proc++;
			stmts.push_back(new ReturnStmt(line));
			continue;
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_BREAK) {
			int line = content[proc].line;
			proc++;
			stmts.push_back(new BreakStmt(line));
			continue;
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_REDO) {
			int line = content[proc].line;
			proc++;
			stmts.push_back(new RedoStmt(line));
			continue;
		}
		else if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_DOT) {
			proc++;
			continue;
		}
		else {
			int line = content[proc].line;
            error("", SGS_SE_UNKNOWN, line);
            skipLine();
            continue;
        }
    }
    return;
}

void Syntax::skipLine() {
    int tmpLine = content[proc].line;
    while (proc < content.size() && content[proc].line == tmpLine)proc++;
}
bool Syntax::checkBoarder() {
	return proc <= content.size();
}
void Syntax::parseLib(string lib, int line) {
	Lexeme l = Lexeme();
	Syntax s = Syntax();

	std::ifstream fin(lib + ".sgs");
	string input, tmp;

	if (!fin.is_open()) {
		error((lib + ".sgs").data(), SGS_SE_NOLIB, line);
	}

	while (!fin.eof()) {
		getline(fin, tmp);
		input += tmp + '\n';
	}
	fin.close();
	l.input(input.data())->parse();
	s.input(l.strId, l.output)->parse();

	for (auto st : s.stmts)stmts.push_back(st);
	for (auto c : s.classList)classList.push_back(c);
	for (auto f : s.funcList)funcList.push_back(f);
	for (auto m : s.msgList)msgList.push_back(m);
}
VarType *Syntax::parseType() {
	VarType *element = NULL;
	if (content[proc].type == SGS_TT_USER) {
		int classIdx;
		if ((classIdx = findClass()) >= 0) {
			element = classList[classIdx];
		}
		else return NULL;
	}
	else if (content[proc].type == SGS_TT_SYS) {
		switch (content[proc].id) {
		case SGS_ID_INTEGER:
			element = new BasicType(BT_INT);
			break;
		case SGS_ID_FLOAT:
			element = new BasicType(BT_FLOAT);
			break;
		case SGS_ID_BOOL:
			element = new BasicType(BT_BOOL);
			break;
		case SGS_ID_CHAR:
			element = new BasicType(BT_CHAR);
			break;
		case SGS_ID_STRING:
			element = new BasicType(BT_STRING);
			break;
		default:
			return NULL;
		}
		proc++;
	}

	VarType *ret = element;
	while (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_ARRAY) {
		ret = new ArrayType(ret, 0);
		proc++;
	}

	return ret;
}
Expression *Syntax::parseExp() {
    std::stack<std::pair<int, int>> op;
    std::stack<Expression*> value;

    while (proc < content.size()) {
        if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA)break;
        else if (content[proc].type == SGS_TT_SYS && content[proc].id != SGS_ID_RESULT &&
            content[proc].id != SGS_ID_FALSE && content[proc].id != SGS_ID_TRUE &&
			content[proc].id != SGS_ID_AND && content[proc].id != SGS_ID_OR &&
			content[proc].id != SGS_ID_IS && content[proc].id != SGS_ID_ITS)break;
        else if (content[proc].type == SGS_TT_OP && (content[proc].id == SGS_OP_DOT ||
            content[proc].id == SGS_OP_LPARENTHESIS || content[proc].id == SGS_OP_RPARENTHESIS ||
			content[proc].id == SGS_OP_LBRAKET || content[proc].id == SGS_OP_RBRAKET))break;
        else if (content[proc].type == SGS_TT_USER ||
            (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_RESULT)) {
            int classIdx;
            int funcIdx;

            if ((classIdx = findClass()) >= 0) {
				int line = content[proc].line;
                if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_LBRAKET) {
					proc++;
					value.push(parseClassLiteral(classIdx));
                }
				else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_WITH) {
					proc++;
					value.push(new CallExp(line, classList[classIdx]->latestConstructor().first, parseAttrib(classIdx)));
				}
				else
					value.push(new CallExp(line, classList[classIdx]->latestConstructor().first, vector<Expression *>()));
            }
			else if ((funcIdx = findFunc()) != -1) {
				int line = content[proc].line;
				if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_WITH) {
					proc++;
					value.push(new CallExp(line, funcList[funcIdx], parseParam(funcIdx)));
				}
                else {
                    value.push(new CallExp(line, funcList[funcIdx], vector<Expression *>()));
                }
            }
			else {
                value.push(parseVar());
            }
        }
		else if (content[proc].type == SGS_TT_DATA) {
			int line = content[proc].line;
            switch (content[proc].id) {
			case SGS_CT_BOOL:
				value.push(new BoolLiteral(line, content[proc].bvalue));
				break;
            case SGS_CT_CHAR:
                value.push(new CharLiteral(line, content[proc].cvalue));
                break;
            case SGS_CT_INTEGER:
                value.push(new IntLiteral(line, content[proc].ivalue));
                break;
            case SGS_CT_FLOAT:
                value.push(new FloatLiteral(line, content[proc].fvalue));
                break;
			case SGS_CT_STRING:
				value.push(new StrLiteral(line, content[proc].svalue));
				break;
            default:
				break;
            }

            proc++;
        }
		else if (content[proc].type == SGS_TT_OP ||
			content[proc].type == SGS_TT_SYS &&(content[proc].id == SGS_ID_IS ||
				content[proc].id == SGS_ID_AND || content[proc].id == SGS_ID_OR)) {
			int line = content[proc].line;
            if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_LBRACE) {
                op.push(std::make_pair(SGS_OP_LBRACE, line));
            } 
			else if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_RBRACE) {
                while (op.top().first != SGS_OP_LBRACE) {
                    int o = op.top().first;
                    op.pop();
                    if (value.size() < 2)
                        error("表达式", SGS_SE_INCOMPLETE, line);
                    Expression *a = value.top();
                    value.pop();
                    Expression *b = value.top();
                    value.pop();

                    OpExp *tmp = new OpExp(line, (OPERATOR)o);
                    tmp->setLeft(b);
                    tmp->setRight(a);
                    value.push(tmp);
                }
                op.pop();
            } 
			else {
				int real;
				if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_IS)
					real = SGS_OP_EQUAL;
				else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_AND)
					real = SGS_OP_ANDAND;
				else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_OR)
					real = SGS_OP_OROR;
				else real = content[proc].id;

                if (op.empty()) {
					op.push(std::make_pair(real, line));
                } 
				else {
                    while (!op.empty() && compareOp(real, op.top().first)) {
                        int o = op.top().first;
                        op.pop();
                        if (value.size() < 2)
                            error("表达式", SGS_SE_INCOMPLETE, line);
                        Expression *a = value.top();
                        value.pop();
                        Expression *b = value.top();
                        value.pop();

                        OpExp *tmp = new OpExp(line, (OPERATOR)o);
                        tmp->setLeft(b);
                        tmp->setRight(a);
                        value.push(tmp);
                    }
                    op.push(std::make_pair(real, line));
                }
            }
            proc++;
        } 
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_ITS) {
			value.push(parseVar());
		}
		else break;
    }

    while (!op.empty()) {
        int o = op.top().first;
		int line = op.top().second;
        op.pop();
        if (value.size() < 2)
            error("表达式", SGS_SE_INCOMPLETE, line);
        Expression *a = value.top();
        value.pop();
        Expression *b = value.top();
        value.pop();

        OpExp *tmp = new OpExp(line, (OPERATOR)o);
        tmp->setLeft(b);
        tmp->setRight(a);
        value.push(tmp);
    }

    if (value.size() == 1) {
        return value.top();
    } 
	else {
		/*VarType *t = value.top()->getResType();
		vector<Expression *> cont;

		while (!value.empty()) {
			if (value.top()->getResType() != t)
				error("array", SGS_SE_INVALIDTYPE);
			cont.push_back(value.top());
			value.pop();
		}
		return new ArrayLiteral(t, cont);*/
	}
    return nullptr;
}
Expression *Syntax::parseVar() {
	int line = content[proc].line;
    Expression *ret = nullptr;

    while (proc < content.size()) {
        if (content[proc].type == SGS_TT_USER ||
            (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_RESULT)) {
            ret = new IdExp(line, parseUser());
        }
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_ITS) {
			proc++;
			ret = new AccessExp(line, NULL, parseUser());
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_OF) {
            proc++;
            ret = new AccessExp(line, ret, parseUser());
        }
		else if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_LPARENTHESIS) {
            proc++;
            ret = new VisitExp(line, ret, parseExp());
            if (content[proc].type != SGS_TT_OP || content[proc].id != SGS_OP_RPARENTHESIS)
                error("array index", SGS_SE_EXPBRACE, line);
            else proc++;
        }
		else break;
    }

    return ret;
}
FuncProto *Syntax::parseFuncDec() {
	int line = content[proc].line;
    vector<std::pair<VarType *, string>> params;

	int classIdx;
	if ((classIdx = findClass()) != -1) {
		if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_WITH) {
			proc++;
			while (proc < content.size()) {
				VarType *tmpType = parseType();
				params.emplace_back(tmpType, parseUser());

				if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
					proc++;
					continue;
				}
				else break;
			}
		}
		classList[classIdx]->decConstructor(new FuncProto(line, nullptr, classList[classIdx]->getName(), params));
		return NULL;
	}
	else {
		string name = parseUser();
		if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_WITH) {
			proc++;
			while (proc < content.size()) {
				VarType *tmpType = parseType();
				params.emplace_back(tmpType, parseUser());

				if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
					proc++;
					continue;
				}
				else break;
			}
		}
		if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_RETURN) {
			proc++;
			if (content[proc].type == SGS_TT_USER) {
				if ((classIdx = findClass()) >= 0) {
					return new FuncProto(line, classList[classIdx], name, params);
				}
				else {
					error(parseUser().data(), SGS_SE_EXPTYPE, line);
					return new FuncProto(line, nullptr, name, params);
				}
			}
			else if (content[proc].type == SGS_TT_SYS) {
				if (content[proc].id == SGS_ID_INTEGER) {
					proc++;
					return new FuncProto(line, new BasicType(BT_INT), name, params);
				}
				else if (content[proc].id == SGS_ID_FLOAT) {
					proc++;
					return new FuncProto(line, new BasicType(BT_FLOAT), name, params);
				}
				else if (content[proc].id == SGS_ID_BOOL) {
					proc++;
					return new FuncProto(line, new BasicType(BT_BOOL), name, params);
				}
				else if (content[proc].id == SGS_ID_CHAR) {
					proc++;
					return new FuncProto(line, new BasicType(BT_CHAR), name, params);
				}
				else if (content[proc].id == SGS_ID_STRING) {
					proc++;
					return new FuncProto(line, new BasicType(BT_STRING), name, params);
				}
				else {
					error(strId[content[proc].id].data(), SGS_SE_EXPTYPE, line);
					return new FuncProto(line, nullptr, name, params);
				}
			}
			else {
				error("", SGS_SE_EXPTYPE, line);
				return new FuncProto(line, nullptr, name, params);
			}
		}
		else {
			return new FuncProto(line, nullptr, name, params);
		}
	}
}
ClassDef *Syntax::parseClassDec() {
	int line = content[proc].line;
    string name = parseUser();
    vector<std::pair<VarType *, string>> elements;

    if (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_WITH)
        error("", SGS_SE_EMPTYCLASS, line);
    proc++;
    while (proc < content.size()) {
		VarType *tmpType = parseType();
		elements.emplace_back(tmpType, parseUser());
        
		if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
            proc++;
            continue;
        }
		else break;
    }
    return new ClassDef(line, new ClassType(name, elements));
}
ClassLiteral *Syntax::parseClassLiteral(int classid) {
	int line = content[proc].line;

	vector<Expression *> ele(classList[classid]->getEle().size(), nullptr);
	vector<string> name;
	for (auto item : classList[classid]->getEle()) {
		name.push_back(item.second);
	}
	bool hide = false;
	int idx = 0;
	for (unsigned int i = 0; i < classList[classid]->getEle().size(); i++) {
		int idx;
		if (i == 0) {
			idx = parseUser(name);
			if (idx == -1)hide = true;
		}

		if (hide) {
			ele[i] = parseExp();
			proc++;
		}
		else {
			if (idx == -1)idx = parseUser(name);

			if (idx != -1)
				ele[idx] = parseExp();
			else
				error(parseUser().data(), SGS_SE_NOID, line);

			idx = -1;
			if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA)
				proc++;
		}
	}
	return new ClassLiteral(line, classList[classid]->getName(), classList[classid]->getEle(), ele);
}
FuncDef *Syntax::parseFuncDef(int funcid) {
	int line = content[proc].line;

    FuncDef *ret = new FuncDef(line, funcList[funcid]);
    ret->setBody(parseBlock());
    return ret;
}
FuncDef *Syntax::parseConstructorDef(int classid) {
	int line = content[proc].line;

	FuncDef *ret = new FuncDef(line, classList[classid]->latestConstructor().first);
	ret->setBody(parseBlock());
	classList[classid]->defConstructor(ret);
	return ret;
}
vector<Expression *> Syntax::parseParam(int funcid) {
	int line = content[proc].line;

    vector<Expression *> para(funcList[funcid]->getParam().size(), nullptr);
    vector<string> name;
    for (auto item : funcList[funcid]->getParam()) {
        name.push_back(item.second);
    }
    bool hide = false;
    for (unsigned int i = 0; i < funcList[funcid]->getParam().size(); i++) {
		int idx;
		if (i == 0) {
			idx = parseUser(name);
			if (idx == -1)hide = true;
		}

        if (hide) {
            para[i] = parseExp();
			if (proc >= content.size())break;
			if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA)
				proc++;
        }
		else {
			if(idx == -1)idx = parseUser(name);

			if (idx != -1)
				para[idx] = parseExp();
			else
				error(parseUser().data(), SGS_SE_NOID, line);

			idx = -1;
			if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA)
				proc++;
        }
    }

    return para;
}
vector<Expression *> Syntax::parseAttrib(int classid) {
	int line = content[proc].line;

	vector<Expression *> para(classList[classid]->latestConstructor().first->getParam().size(), nullptr);
	vector<string> name;
	for (auto item : classList[classid]->latestConstructor().first->getParam()) {
		name.push_back(item.second);
	}
	bool hide = false;
	for (unsigned int i = 0; i < classList[classid]->latestConstructor().first->getParam().size(); i++) {
		int idx;
		if (i == 0) {
			idx = parseUser(name);
			if (idx == -1)hide = true;
		}

		if (hide) {
			para[i] = parseExp();
			if (proc >= content.size())break;
			if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA)
				proc++;
		}
		else {
			if (idx == -1)idx = parseUser(name);

			if (idx != -1)
				para[idx] = parseExp();
			else
				error(parseUser().data(), SGS_SE_NOID, line);

			idx = -1;
			if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA)
				proc++;
		}
	}

	return para;
}
BlockStmt *Syntax::parseBlock(bool untaken) {
	int line = content[proc].line;
    BlockStmt *block = new BlockStmt(line);
    while (proc < content.size()) {
        if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LET) {
			int line = content[proc].line;
            proc++;

			vector<Expression *>lefts;

			VarType *newType = parseType();
			if (newType) {
				string newVar = parseUser();
				stmts.push_back(new VarDef(line, newType, newVar));
				lefts.push_back(new IdExp(line, newVar));
				while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
					proc++;
					stmts.push_back(new VarDef(line, newType, newVar = parseUser()));
					lefts.push_back(new IdExp(line, newVar));
				}
			}
			else {
				lefts.push_back(parseVar());
				while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
					proc++;
					lefts.push_back(parseVar());
				}
			}

            if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_BE) {
				proc++;

				vector<Expression *>rights;
				rights.push_back(parseExp());
				while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
					proc++;
					rights.push_back(parseExp());
				}

				for (unsigned int i = 0; i < rights.size(); i++) {
					block->pushAST(new AssignStmt(line, lefts[i], rights[i]));
				}
            }

            continue;
        }
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_IF) {
			int line = content[proc].line;
            proc++;

            Expression *branch = parseExp();
			if (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_THEN)
				error("If(then)", SGS_SE_INCOMPLETE, line);
            else proc++;

            BlockStmt *taken = parseBlock();
            if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_ELSE) {
                proc++;
                if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_IF) {
                    proc++;
                    block->pushAST(new IfStmt(line, branch, taken, parseBlock(true)));
                }
				else block->pushAST(new IfStmt(line, branch, taken, parseBlock()));
            }
			else {
                block->pushAST(new IfStmt(line, branch, taken, nullptr));
            }

            if (!untaken) {
                if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_END) {
                    proc++;
                    if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_IF)
                        proc++;
                    else error("End if", SGS_SE_INCOMPLETE, line);
                }
				else error("If(end)", SGS_SE_INCOMPLETE, line);
            }

            if (untaken)break;
            else continue;
        }
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LOOP) {
			int line = content[proc].line;
            proc++;

            Expression *cond;
            if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_WHEN) {
                proc++;
                cond = parseExp();
            }
			else {
                error("Loop(when)", SGS_SE_INCOMPLETE, line);
                cond = parseExp();
            }

            block->pushAST(new WhileStmt(line, cond, parseBlock()));
            if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_END) {
                proc++;
                if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LOOP)
                    proc++;
                else
                    error("End loop", SGS_SE_INCOMPLETE, line);
            }
			else error("Loop(end)", SGS_SE_INCOMPLETE, line);

            continue;
        }
		else if (content[proc].type == SGS_TT_USER) {
			int line = content[proc].line;
            int funcIdx;
            string newVar;
            if ((funcIdx = findFunc()) != -1) {
				if (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_WITH)
					block->pushAST(new CallStmt(line, funcList[funcIdx], vector<Expression *>()));
				else {
					proc++;
					block->pushAST(new CallStmt(line, funcList[funcIdx], parseParam(funcIdx)));
				}
            }
			else error(parseUser().data(), SGS_SE_NOID, line);
            continue;
        }
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_RETURN) {
			int line = content[proc].line;
            proc++;
            block->pushAST(new ReturnStmt(line));
			continue;
        }
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_BREAK) {
			int line = content[proc].line;
            proc++;
            block->pushAST(new BreakStmt(line));
			continue;
        }
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_REDO) {
			int line = content[proc].line;
            proc++;
            block->pushAST(new RedoStmt(line));
			continue;
        }
		else if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_DOT) {
            proc++;
            continue;
        }
		else if ((content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_END) ||
            (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_ELSE)) {
            break;
        }
		else {
            error("", SGS_SE_UNKNOWN, line);
            skipLine();
            continue;
        }
    }
    return block;
}

string Syntax::parseUser(string guide) {
    string tmp;
    while (proc < content.size() && (content[proc].type == SGS_TT_USER ||
        (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_RESULT))) {
        tmp += strId[content[proc++].id];
        if (tmp == guide)break;
        tmp += " ";
    }
    if (tmp.length())tmp.pop_back();
    return tmp;
}
int Syntax::parseUser(vector<string> guides) {
    string tmp;
    int pre = proc;
    while (proc < content.size() && content[proc].type == SGS_TT_USER) {
        tmp += strId[content[proc++].id];
        for (unsigned int i = 0; i < guides.size(); i++) {
            if (tmp == guides[i])return i;
        }
        tmp += " ";
    }
    proc = pre;
    return -1;
}
int Syntax::findClass() {
    string tmp;
    int pre = proc;
    while (proc < content.size() && content[proc].type == SGS_TT_USER) {
        tmp += strId[content[proc++].id];
        for (unsigned int i = 0; i < classList.size(); i++) {
            if (tmp == classList[i]->getName())return i;
        }
        tmp += " ";
    }
    proc = pre;
    return -1;
}
int Syntax::findFunc() {
    string tmp;
    int pre = proc;
    while (proc < content.size() && content[proc].type == SGS_TT_USER) {
        tmp += strId[content[proc++].id];
        for (unsigned int i = 0; i < funcList.size(); i++) {
            if (tmp == funcList[i]->getName())return i;
        }
        tmp += " ";
    }
    proc = pre;
    return -1;
}

void Syntax::generate(string file) {

}

void Syntax::error(const char *inst, SYNTAXERROR type, int line) {
    switch (type) {
	case SGS_SE_NOLIB:
		msgList.emplace_back(string("第") + std::to_string(line) + string("行：") +
			string("找不到sgs库") + inst + string("，请确认文件存在并添加环境变量。\n"), MT_WARNING);
		break;
	case SGS_SE_USELIB:
		msgList.emplace_back(string("第") + std::to_string(line) + string("行：") +
			string("Use 后面需要 library 关键字。\n"), MT_ERROR);
		break;
	case SGS_SE_LIBNAME:
		msgList.emplace_back(string("第") + std::to_string(line) + string("行：") +
			string("Use library 后面直接输入库名即可（不含.sgs）。例：use library io.\n"), MT_ERROR);
		break;
	case SGS_SE_AFTERNEW:
		msgList.emplace_back(string("第") + std::to_string(line) + string("行：") +
			string("New 关键字后面只可以出现 class 或者 function。\n"), MT_ERROR);
		break;
	case SGS_SE_EMPTYCLASS:
		msgList.emplace_back(string("第") + std::to_string(line) + string("行：") +
			string("New class 中没有元素（使用 with 使其包含元素）。\n"), MT_WARNING);
		break;
	case SGS_SE_EXPTYPE:
		msgList.emplace_back(string("第") + std::to_string(line) + string("行：") +
			inst + string("错误，请输入正确的基本类型或已定义的类。\n"), MT_ERROR);
		break;
    case SGS_SE_EXPOSE:
        msgList.emplace_back(string("第") + std::to_string(line) + string("行：") +
			inst + string("语句块不封闭，请在正确位置加入 end 关键字。\n"), MT_ERROR);
        break;
    case SGS_SE_UNIQUE:
        msgList.emplace_back(string("第") + std::to_string(line) + string("行：") +
			inst + string("语法错误。\n"), MT_ERROR);
        break;
    case SGS_SE_EXPDOT:
        msgList.emplace_back(string("第") + std::to_string(line) + string("行：") +
			string("缺少句号。\n"), MT_ERROR);
        break;
    case SGS_SE_EXPCOMMA:
        msgList.emplace_back(string("第") + std::to_string(line) + string("行：") +
			inst + string("缺少逗号。\n"), MT_ERROR);
        break;
    case SGS_SE_EXPBRACE:
        msgList.emplace_back(string("第") + std::to_string(line) + string("行：") +
			string("括号不匹配。\n"), MT_ERROR);
        break;
    case SGS_SE_REDEF:
        msgList.emplace_back(string("第") + std::to_string(line) + string("行：") +
			inst + string("重复定义。\n"), MT_WARNING);
        break;
    case SGS_SE_INVALIDTYPE:
        msgList.emplace_back(string("第") + std::to_string(line) + string("行：") +
			inst + string("类型错误。\n"), MT_WARNING);
        break;
    case SGS_SE_DISACCORD:
        msgList.emplace_back(string("第") + std::to_string(line) + string("行：") +
			inst + string("前后不一致。\n"), MT_WARNING);
        break;
    case SGS_SE_NOID:
        msgList.emplace_back(string("第") + std::to_string(line) + string("行：") +
			inst + string("未定义。\n"), MT_ERROR);
        break;
    case SGS_SE_INCOMPLETE:
        msgList.emplace_back(string("第") + std::to_string(line) + string("行：") +
			inst + string("不完整。\n"), MT_WARNING);
        break;
    case SGS_SE_UNKNOWN:
        msgList.emplace_back(string("第") + std::to_string(line) + string("行：") +
			string("未知错误。\n"), MT_ERROR);
        break;
    case SGS_SE_UNSUPPORT:
        msgList.emplace_back(string("第") + std::to_string(line) + string("行：") +
			string("暂不支持。\n"), MT_ERROR);
        break;
    }
}

