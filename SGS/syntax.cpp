#define _CRT_SECURE_NO_WARNINGS
#include "syntax.h"
#include <iostream>
#include <fstream>

using namespace sgs;

Syntax::Syntax() {
    proc = 0;

    prepare();
}
Syntax::Syntax(vector<string> &ids, vector<TokenPrim> &input) {
    proc = 0;

    content = input;
    strId = ids;

    prepare();
}
Syntax::~Syntax() = default;

void Syntax::prepare() {

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
        } 
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_NEW) {
            proc++;
            if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_CLASS) {
                proc++;
                ClassDef *tmpClass = parseClassDec();
                classList.push_back(static_cast<ClassType *>(tmpClass->getDecType()));
                stmts.push_back(tmpClass);
            }
			else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_FUNCTION) {
                proc++;
                FuncProto *tmpFunc = parseFuncDec();
				if (tmpFunc) {
					funcList.push_back(tmpFunc);
					stmts.push_back(tmpFunc);
				}
            }
			else
                error("New", SGS_SE_UNIQUE);
            continue;
        } 
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LET) {
			int line = content[proc].line;
            proc++;

            vector<Expression *>lefts;
            int classIdx;
            string newVar;

            if (content[proc].type == SGS_TT_USER) {
                if ((classIdx = findClass()) >= 0) {
                    if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_ARRAY) {
                        proc++;
                        int length = int(content[proc].ivalue);
                        proc++;
                        stmts.push_back(new VarDef(line, new ArrayType(classList[classIdx], length), newVar = parseUser()));
						lefts.push_back(new IdExp(line, newVar));
						while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							stmts.push_back(new VarDef(line, new ArrayType(classList[classIdx], length), newVar = parseUser()));
							lefts.push_back(new IdExp(line, newVar));
						}
                    }
					else {
                        stmts.push_back(new VarDef(line, classList[classIdx], newVar = parseUser()));
						lefts.push_back(new IdExp(line, newVar));
						while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							stmts.push_back(new VarDef(line, classList[classIdx], newVar = parseUser()));
							lefts.push_back(new IdExp(line, newVar));
						}
                    }
                } 
				else {
					lefts.push_back(parseExp());
					while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
						proc++;
						lefts.push_back(parseExp());
					}
                }
            }
			else if (content[proc].type == SGS_TT_SYS) {
                if (content[proc].id == SGS_ID_INTEGER) {
                    if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
                        proc += 2;
                        int length = static_cast<int>(content[proc].ivalue);
                        proc++;
                        stmts.push_back(new VarDef(line, new ArrayType(new BasicType(BT_INT), length), newVar = parseUser()));
						lefts.push_back(new IdExp(line, newVar));
						while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							stmts.push_back(new VarDef(line, new ArrayType(new BasicType(BT_INT), length), newVar = parseUser()));
							lefts.push_back(new IdExp(line, newVar));
						}
                    } 
					else {
                        proc++;
                        stmts.push_back(new VarDef(line, new BasicType(BT_INT), newVar = parseUser()));
						lefts.push_back(new IdExp(line, newVar));
						while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							stmts.push_back(new VarDef(line, new BasicType(BT_INT), newVar = parseUser()));
							lefts.push_back(new IdExp(line, newVar));
						}
                    }
                } 
				else if (content[proc].id == SGS_ID_FLOAT) {
                    if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
                        proc += 2;
                        int length = static_cast<int>(content[proc].ivalue);
                        proc++;
                        stmts.push_back(new VarDef(line, new ArrayType(new BasicType(BT_FLOAT), length), newVar = parseUser()));
						lefts.push_back(new IdExp(line, newVar));
						while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							stmts.push_back(new VarDef(line, new ArrayType(new BasicType(BT_FLOAT), length), newVar = parseUser()));
							lefts.push_back(new IdExp(line, newVar));
						}
                    }
					else {
                        proc++;
                        stmts.push_back(new VarDef(line, new BasicType(BT_FLOAT), newVar = parseUser()));
						lefts.push_back(new IdExp(line, newVar));
						while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							stmts.push_back(new VarDef(line, new BasicType(BT_FLOAT), newVar = parseUser()));
							lefts.push_back(new IdExp(line, newVar));
						}
                    }
                } 
				else if (content[proc].id == SGS_ID_BOOL) {
                    if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
                        proc += 2;
                        int length = static_cast<int>(content[proc].ivalue);
                        proc++;
                        stmts.push_back(new VarDef(line, new ArrayType(new BasicType(BT_BOOL), length), newVar = parseUser()));
						lefts.push_back(new IdExp(line, newVar));
						while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							stmts.push_back(new VarDef(line, new ArrayType(new BasicType(BT_BOOL), length), newVar = parseUser()));
							lefts.push_back(new IdExp(line, newVar));
						}
                    }
					else {
                        proc++;
                        stmts.push_back(new VarDef(line, new BasicType(BT_BOOL), newVar = parseUser()));
						lefts.push_back(new IdExp(line, newVar));
						while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							stmts.push_back(new VarDef(line, new BasicType(BT_BOOL), newVar = parseUser()));
							lefts.push_back(new IdExp(line, newVar));
						}
                    }
                }
				else if (content[proc].id == SGS_ID_CHAR) {
                    if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
                        proc += 2;
                        int length = static_cast<int>(content[proc].ivalue);
                        proc++;
                        stmts.push_back(new VarDef(line, new ArrayType(new BasicType(BT_CHAR), length), newVar = parseUser()));
						lefts.push_back(new IdExp(line, newVar));
						while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							stmts.push_back(new VarDef(line, new ArrayType(new BasicType(BT_CHAR), length), newVar = parseUser()));
							lefts.push_back(new IdExp(line, newVar));
						}
                    }
					else {
                        proc++;
                        stmts.push_back(new VarDef(line, new BasicType(BT_CHAR), newVar = parseUser()));
						lefts.push_back(new IdExp(line, newVar));
						while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							stmts.push_back(new VarDef(line, new BasicType(BT_CHAR), newVar = parseUser()));
							lefts.push_back(new IdExp(line, newVar));
						}
                    }
                }
				else if (content[proc].id == SGS_ID_STRING) {
                    if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
                        proc += 2;
                        int length = static_cast<int>(content[proc].ivalue);
                        proc++;
                        stmts.push_back(new VarDef(line, new ArrayType(new BasicType(BT_STRING), length), newVar = parseUser()));
						lefts.push_back(new IdExp(line, newVar));
						while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							stmts.push_back(new VarDef(line, new ArrayType(new BasicType(BT_STRING), length), newVar = parseUser()));
							lefts.push_back(new IdExp(line, newVar));
						}
                    }
					else {
                        proc++;
                        stmts.push_back(new VarDef(line, new BasicType(BT_STRING), newVar = parseUser()));
						lefts.push_back(new IdExp(line, newVar));
						while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							stmts.push_back(new VarDef(line, new BasicType(BT_STRING), newVar = parseUser()));
							lefts.push_back(new IdExp(line, newVar));
						}
                    }
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
            proc++;
            int funcIdx, classIdx;
			if ((classIdx = findClass()) != -1) {
				stmts.push_back(parseConstructorDef(classIdx));
				if (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_END)
					error(((ClassType *)classList[classIdx])->getName().data(), SGS_SE_EXPOSE);
				proc++;
				if (classIdx != findClass())
					error(((ClassType *)classList[classIdx])->getName().data(), SGS_SE_DISACCORD);
			}
            else if ((funcIdx = findFunc()) != -1) {
				stmts.push_back(parseFuncDef(funcIdx));
				if (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_END)
					error(((FuncProto *)funcList[funcIdx])->getName().data(), SGS_SE_EXPOSE);
				proc++;
				if (funcIdx != findFunc())
					error(((FuncProto *)funcList[funcIdx])->getName().data(), SGS_SE_DISACCORD);
            }
			else {
                error(parseUser().data(), SGS_SE_NOID);
                skipLine();
            }
            continue;
        } 
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_IF) {
			int line = content[proc].line;
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
			else error("If(end)", SGS_SE_INCOMPLETE);

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
                error("Loop(when)", SGS_SE_INCOMPLETE);
                cond = parseExp();
            }

            stmts.push_back(new WhileStmt(line, cond, parseBlock()));
            if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_END) {
                proc++;
            }
			else error("Loop(end)", SGS_SE_INCOMPLETE);

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
			else error("Function", SGS_SE_NOID);
            continue;
        } 
		else if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_DOT) {
            proc++;
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
		else {
            error("", SGS_SE_UNKNOWN);
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
void Syntax::parseLib(string lib) {
	Lexeme l = Lexeme();
	Syntax s = Syntax();

	std::ifstream fin(lib + ".sgs");
	string input, tmp;
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
Expression *Syntax::parseExp() {
    std::stack<std::pair<int, int>> op;
    std::stack<Expression*> value;

    while (proc < content.size()) {
        if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA)break;
        else if (content[proc].type == SGS_TT_SYS && content[proc].id != SGS_ID_RESULT &&
            content[proc].id != SGS_ID_FALSE && content[proc].id != SGS_ID_TRUE)break;
        else if (content[proc].type == SGS_TT_OP && (content[proc].id == SGS_OP_DOT ||
            content[proc].id == SGS_OP_LPARENTHESIS || content[proc].id == SGS_OP_RPARENTHESIS ||
			content[proc].id == SGS_OP_LBRAKET || content[proc].id == SGS_OP_RBRAKET))break;
        else if (content[proc].type == SGS_TT_USER ||
            (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_RESULT)) {
            int pre = proc;

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
		else if (content[proc].type == SGS_TT_OP) {
			int line = content[proc].line;
            if (content[proc].id == SGS_OP_LBRACE) {
                op.push(std::make_pair(SGS_OP_LBRACE, line));
            } 
			else if (content[proc].id == SGS_OP_RBRACE) {
                while (op.top().first != SGS_OP_LBRACE) {
                    int o = op.top().first;
                    op.pop();
                    if (value.size() < 2)
                        error("", SGS_SE_INCOMPLETE);
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
                if (op.empty()) {
					op.push(std::make_pair(content[proc].id, line));
                } 
				else {
                    while (!op.empty() && compare(content[proc].id, op.top().first)) {
                        int o = op.top().first;
                        op.pop();
                        if (value.size() < 2)
                            error("", SGS_SE_INCOMPLETE);
                        Expression *a = value.top();
                        value.pop();
                        Expression *b = value.top();
                        value.pop();

                        OpExp *tmp = new OpExp(line, (OPERATOR)o);
                        tmp->setLeft(b);
                        tmp->setRight(a);
                        value.push(tmp);
                    }
                    op.push(std::make_pair(content[proc].id, line));
                }
            }
            proc++;
        } 
		else break;
    }

    while (!op.empty()) {
        int o = op.top().first;
		int line = op.top().second;
        op.pop();
        if (value.size() < 2)
            error("", SGS_SE_INCOMPLETE);
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
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_OF) {
            proc++;
            ret = new AccessExp(line, ret, parseUser());
        }
		else if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_LPARENTHESIS) {
            proc++;
            ret = new VisitExp(line, ret, parseExp());
            if (content[proc].type != SGS_TT_OP || content[proc].id != SGS_OP_RPARENTHESIS)
                error("array index", SGS_SE_EXPBRACE);
            else proc++;
        }
		else break;
    }

    return ret;
}
ClassDef *Syntax::parseClassDec() {
	int line = content[proc].line;
    string name = parseUser();
    vector<std::pair<VarType *, string>> elements;

    int classIdx;
    if (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_WITH)
        error("new class", SGS_SE_UNIQUE);
    proc++;
    while (proc < content.size()) {
        if (content[proc].type == SGS_TT_USER) {
            if ((classIdx = findClass()) >= 0) {
				if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_ARRAY) {
					proc++;
					int length = int(content[proc].ivalue);
					proc++;
					elements.emplace_back(new ArrayType(classList[classIdx], length), parseUser());
				}
				else {
					elements.emplace_back(classList[classIdx], parseUser());
				}
            }
			else error(parseUser().data(), SGS_SE_NOID);
        }
		else if (content[proc].type == SGS_TT_SYS) {
            if (content[proc].id == SGS_ID_INTEGER) {
				if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
					proc += 2;
					int length = int(content[proc].ivalue);
					proc++;
					elements.push_back(std::pair<VarType *, string>(new ArrayType(new BasicType(BT_INT), length), parseUser()));
				}
				else {
					proc++;
					elements.push_back(std::pair<VarType *, string>(new BasicType(BT_INT), parseUser()));
				}
            }
			else if (content[proc].id == SGS_ID_FLOAT) {
				if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
					proc += 2;
					int length = int(content[proc].ivalue);
					proc++;
					elements.push_back(std::pair<VarType *, string>(new ArrayType(new BasicType(BT_FLOAT), length), parseUser()));
				}
				else {
					proc++;
					elements.push_back(std::pair<VarType *, string>(new BasicType(BT_FLOAT), parseUser()));
				}
            }
			else if (content[proc].id == SGS_ID_BOOL) {
				if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
					proc += 2;
					int length = int(content[proc].ivalue);
					proc++;
					elements.push_back(std::pair<VarType *, string>(new ArrayType(new BasicType(BT_BOOL), length), parseUser()));
				}
				else {
					proc++;
					elements.push_back(std::pair<VarType *, string>(new BasicType(BT_BOOL), parseUser()));
				}
            }
			else if (content[proc].id == SGS_ID_CHAR) {
				if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
					proc += 2;
					int length = int(content[proc].ivalue);
					proc++;
					elements.push_back(std::pair<VarType *, string>(new ArrayType(new BasicType(BT_CHAR), length), parseUser()));
				}
				else {
					proc++;
					elements.push_back(std::pair<VarType *, string>(new BasicType(BT_CHAR), parseUser()));
				}
            }
			else if (content[proc].id == SGS_ID_STRING) {
				if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
					proc += 2;
					int length = int(content[proc].ivalue);
					proc++;
					elements.push_back(std::pair<VarType *, string>(new ArrayType(new BasicType(BT_STRING), length), parseUser()));
				}
				else {
					proc++;
					elements.push_back(std::pair<VarType *, string>(new BasicType(BT_STRING), parseUser()));
				}
            }
			else break;
        }
		else if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
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
		if (!hide) {
			idx = parseUser(name);
			if (idx == -1) {
				if (i == 0)hide = true;
				else error(parseUser().data(), SGS_SE_NOID);
			}
		}
		if (hide) {
			ele[i] = parseExp();
			proc++;
		}
		else {
			ele[idx] = parseExp();
			proc++;
		}
	}
	return new ClassLiteral(line, classList[classid]->getName(), classList[classid]->getEle(), ele);
}
FuncProto *Syntax::parseFuncDec() {
	int line = content[proc].line;
    string name = parseUser();
    vector<std::pair<VarType *, string>> params;

	int classIdx;
	for (auto &c : classList) {
		if (c->getName() == name) {
			if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_WITH) {
				proc++;
				while (proc < content.size()) {
					if (content[proc].type == SGS_TT_USER) {
						if ((classIdx = findClass()) >= 0) {
							if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_ARRAY) {
								proc++;
								int length = int(content[proc].ivalue);
								proc++;
								params.emplace_back(new ArrayType(classList[classIdx], length), parseUser());
							}
							else {
								params.emplace_back(classList[classIdx], parseUser());
							}
						}
						else
							error(parseUser().data(), SGS_SE_NOID);
					}
					else if (content[proc].type == SGS_TT_SYS) {
						if (content[proc].id == SGS_ID_INTEGER) {
							if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
								proc += 2;
								int length = int(content[proc].ivalue);
								proc++;
								params.push_back(std::pair<VarType *, string>(new ArrayType(new BasicType(BT_INT), length), parseUser()));
							}
							else {
								proc++;
								params.push_back(std::pair<VarType *, string>(new BasicType(BT_INT), parseUser()));
							}
						}
						else if (content[proc].id == SGS_ID_FLOAT) {
							if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
								proc += 2;
								int length = int(content[proc].ivalue);
								proc++;
								params.push_back(std::pair<VarType *, string>(new ArrayType(new BasicType(BT_FLOAT), length), parseUser()));
							}
							else {
								proc++;
								params.push_back(std::pair<VarType *, string>(new BasicType(BT_FLOAT), parseUser()));
							}
						}
						else if (content[proc].id == SGS_ID_BOOL) {
							if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
								proc += 2;
								int length = int(content[proc].ivalue);
								proc++;
								params.push_back(std::pair<VarType *, string>(new ArrayType(new BasicType(BT_BOOL), length), parseUser()));
							}
							else {
								proc++;
								params.push_back(std::pair<VarType *, string>(new BasicType(BT_BOOL), parseUser()));
							}
						}
						else if (content[proc].id == SGS_ID_CHAR) {
							if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
								proc += 2;
								int length = int(content[proc].ivalue);
								proc++;
								params.push_back(std::pair<VarType *, string>(new ArrayType(new BasicType(BT_CHAR), length), parseUser()));
							}
							else {
								proc++;
								params.push_back(std::pair<VarType *, string>(new BasicType(BT_CHAR), parseUser()));
							}
						}
						else if (content[proc].id == SGS_ID_STRING) {
							if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
								proc += 2;
								int length = int(content[proc].ivalue);
								proc++;
								params.push_back(std::pair<VarType *, string>(new ArrayType(new BasicType(BT_STRING), length), parseUser()));
							}
							else {
								proc++;
								params.push_back(std::pair<VarType *, string>(new BasicType(BT_STRING), parseUser()));
							}
						}
						else break;
					}
					else if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
						proc++;
						continue;
					}
					else break;
				}
			}
			c->decConstructor(new FuncProto(line, nullptr, name, params));
			return NULL;
		}
	}

    if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_WITH) {
        proc++;
        while (proc < content.size()) {
            if (content[proc].type == SGS_TT_USER) {
                if ((classIdx = findClass()) >= 0) {
					if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_ARRAY) {
						proc++;
						int length = int(content[proc].ivalue);
						proc++;
						params.emplace_back(new ArrayType(classList[classIdx], length), parseUser());
					}
					else {
						params.emplace_back(classList[classIdx], parseUser());
					}
                } 
				else 
					error(parseUser().data(), SGS_SE_NOID);
            } 
			else if (content[proc].type == SGS_TT_SYS) {
                if (content[proc].id == SGS_ID_INTEGER) {
					if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
						proc += 2;
						int length = int(content[proc].ivalue);
						proc++;
						params.push_back(std::pair<VarType *, string>(new ArrayType(new BasicType(BT_INT), length), parseUser()));
					}
					else {
						proc++;
						params.push_back(std::pair<VarType *, string>(new BasicType(BT_INT), parseUser()));
					}
                } 
				else if (content[proc].id == SGS_ID_FLOAT) {
					if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
						proc += 2;
						int length = int(content[proc].ivalue);
						proc++;
						params.push_back(std::pair<VarType *, string>(new ArrayType(new BasicType(BT_FLOAT), length), parseUser()));
					}
					else {
						proc++;
						params.push_back(std::pair<VarType *, string>(new BasicType(BT_FLOAT), parseUser()));
					}
                } 
				else if (content[proc].id == SGS_ID_BOOL) {
					if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
						proc += 2;
						int length = int(content[proc].ivalue);
						proc++;
						params.push_back(std::pair<VarType *, string>(new ArrayType(new BasicType(BT_BOOL), length), parseUser()));
					}
					else {
						proc++;
						params.push_back(std::pair<VarType *, string>(new BasicType(BT_BOOL), parseUser()));
					}
                } 
				else if (content[proc].id == SGS_ID_CHAR) {
					if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
						proc += 2;
						int length = int(content[proc].ivalue);
						proc++;
						params.push_back(std::pair<VarType *, string>(new ArrayType(new BasicType(BT_CHAR), length), parseUser()));
					}
					else {
						proc++;
						params.push_back(std::pair<VarType *, string>(new BasicType(BT_CHAR), parseUser()));
					}
                } 
				else if (content[proc].id == SGS_ID_STRING) {
					if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
						proc += 2;
						int length = int(content[proc].ivalue);
						proc++;
						params.push_back(std::pair<VarType *, string>(new ArrayType(new BasicType(BT_STRING), length), parseUser()));
					}
					else {
						proc++;
						params.push_back(std::pair<VarType *, string>(new BasicType(BT_STRING), parseUser()));
					}
                } 
				else break;
            } 
			else if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
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
                error(parseUser().data(), SGS_SE_NOID);
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
                error(parseUser().data(), SGS_SE_NOID);
                return new FuncProto(line, nullptr, name, params);
            }
        } 
		else {
            error("", SGS_SE_UNKNOWN);
            return new FuncProto(line, nullptr, name, params);
        }
    } 
	else {
        return new FuncProto(line, nullptr, name, params);
    }
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
    vector<Expression *> para(funcList[funcid]->getParam().size(), nullptr);
    vector<string> name;
    for (auto item : funcList[funcid]->getParam()) {
        name.push_back(item.second);
    }
    bool hide = false;
    for (unsigned int i = 0; i < funcList[funcid]->getParam().size(); i++) {
        int idx = parseUser(name);
        if (idx == -1) {
            if (i == 0)hide = true;
            else error(parseUser().data(), SGS_SE_NOID);
        }
        if (hide) {
            para[i] = parseExp();
			if (proc >= content.size())break;
			if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA)
				proc++;
        }
		else {
            para[idx] = parseExp();
			if (proc >= content.size())break;
			if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA)
				proc++;
        }
    }

    return para;
}
vector<Expression *> Syntax::parseAttrib(int classid) {
	vector<Expression *> para(classList[classid]->latestConstructor().first->getParam().size(), nullptr);
	vector<string> name;
	for (auto item : classList[classid]->latestConstructor().first->getParam()) {
		name.push_back(item.second);
	}
	bool hide = false;
	for (unsigned int i = 0; i < classList[classid]->latestConstructor().first->getParam().size(); i++) {
		int idx = parseUser(name);
		if (idx == -1) {
			if (i == 0)hide = true;
			else error(parseUser().data(), SGS_SE_NOID);
		}
		if (hide) {
			para[i] = parseExp();
			if (proc >= content.size())break;
			if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA)
				proc++;
		}
		else {
			para[idx] = parseExp();
			if (proc >= content.size())break;
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
            int classIdx;
            string newVar;

            if (content[proc].type == SGS_TT_USER ||
                (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_RESULT)) {
                if ((classIdx = findClass()) >= 0) {
					if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_ARRAY) {
						proc++;
						int length = int(content[proc].ivalue);
						proc++;
						block->pushAST(new VarDef(line, new ArrayType(classList[classIdx], length), newVar = parseUser()));
						lefts.push_back(new IdExp(line, newVar));
						while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							block->pushAST(new VarDef(line, new ArrayType(classList[classIdx], length), newVar = parseUser()));
							lefts.push_back(new IdExp(line, newVar));
						}
					}
					else {
						block->pushAST(new VarDef(line, classList[classIdx], newVar = parseUser()));
						lefts.push_back(new IdExp(line, newVar));
						while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							block->pushAST(new VarDef(line, classList[classIdx], newVar = parseUser()));
							lefts.push_back(new IdExp(line, newVar));
						}
					}
                }
				else {
					lefts.push_back(parseExp());
					while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
						proc++;
						lefts.push_back(parseExp());
					}
                }
            }
			else if (content[proc].type == SGS_TT_SYS) {
                if (content[proc].id == SGS_ID_INTEGER) {
					if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
						proc += 2;
						int length = static_cast<int>(content[proc].ivalue);
						proc++;
						block->pushAST(new VarDef(line, new ArrayType(new BasicType(BT_INT), length), newVar = parseUser()));
						lefts.push_back(new IdExp(line, newVar));
						while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							block->pushAST(new VarDef(line, new ArrayType(new BasicType(BT_INT), length), newVar = parseUser()));
							lefts.push_back(new IdExp(line, newVar));
						}
					}
					else {
						proc++;
						block->pushAST(new VarDef(line, new BasicType(BT_INT), newVar = parseUser()));
						lefts.push_back(new IdExp(line, newVar));
						while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							block->pushAST(new VarDef(line, new BasicType(BT_INT), newVar = parseUser()));
							lefts.push_back(new IdExp(line, newVar));
						}
					}
                }
				else if (content[proc].id == SGS_ID_FLOAT) {
					if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
						proc += 2;
						int length = static_cast<int>(content[proc].ivalue);
						proc++;
						block->pushAST(new VarDef(line, new ArrayType(new BasicType(BT_FLOAT), length), newVar = parseUser()));
						lefts.push_back(new IdExp(line, newVar));
						while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							block->pushAST(new VarDef(line, new ArrayType(new BasicType(BT_FLOAT), length), newVar = parseUser()));
							lefts.push_back(new IdExp(line, newVar));
						}
					}
					else {
						proc++;
						block->pushAST(new VarDef(line, new BasicType(BT_FLOAT), newVar = parseUser()));
						lefts.push_back(new IdExp(line, newVar));
						while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							block->pushAST(new VarDef(line, new BasicType(BT_FLOAT), newVar = parseUser()));
							lefts.push_back(new IdExp(line, newVar));
						}
					}
                }
				else if (content[proc].id == SGS_ID_BOOL) {
					if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
						proc += 2;
						int length = static_cast<int>(content[proc].ivalue);
						proc++;
						block->pushAST(new VarDef(line, new ArrayType(new BasicType(BT_BOOL), length), newVar = parseUser()));
						lefts.push_back(new IdExp(line, newVar));
						while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							block->pushAST(new VarDef(line, new ArrayType(new BasicType(BT_BOOL), length), newVar = parseUser()));
							lefts.push_back(new IdExp(line, newVar));
						}
					}
					else {
						proc++;
						block->pushAST(new VarDef(line, new BasicType(BT_BOOL), newVar = parseUser()));
						lefts.push_back(new IdExp(line, newVar));
						while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							block->pushAST(new VarDef(line, new BasicType(BT_BOOL), newVar = parseUser()));
							lefts.push_back(new IdExp(line, newVar));
						}
					}
                }
				else if (content[proc].id == SGS_ID_CHAR) {
					if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
						proc += 2;
						int length = static_cast<int>(content[proc].ivalue);
						proc++;
						block->pushAST(new VarDef(line, new ArrayType(new BasicType(BT_CHAR), length), newVar = parseUser()));
						lefts.push_back(new IdExp(line, newVar));
						while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							block->pushAST(new VarDef(line, new ArrayType(new BasicType(BT_CHAR), length), newVar = parseUser()));
							lefts.push_back(new IdExp(line, newVar));
						}
					}
					else {
						proc++;
						block->pushAST(new VarDef(line, new BasicType(BT_CHAR), newVar = parseUser()));
						lefts.push_back(new IdExp(line, newVar));
						while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							block->pushAST(new VarDef(line, new BasicType(BT_CHAR), newVar = parseUser()));
							lefts.push_back(new IdExp(line, newVar));
						}
					}
                }
				else if (content[proc].id == SGS_ID_STRING) {
					if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
						proc += 2;
						int length = static_cast<int>(content[proc].ivalue);
						proc++;
						block->pushAST(new VarDef(line, new ArrayType(new BasicType(BT_STRING), length), newVar = parseUser()));
						lefts.push_back(new IdExp(line, newVar));
						while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							block->pushAST(new VarDef(line, new ArrayType(new BasicType(BT_STRING), length), newVar = parseUser()));
							lefts.push_back(new IdExp(line, newVar));
						}
					}
					else {
						proc++;
						block->pushAST(new VarDef(line, new BasicType(BT_STRING), newVar = parseUser()));
						lefts.push_back(new IdExp(line, newVar));
						while (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA) {
							proc++;
							block->pushAST(new VarDef(line, new BasicType(BT_STRING), newVar = parseUser()));
							lefts.push_back(new IdExp(line, newVar));
						}
					}
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
                error("If(then)", SGS_SE_INCOMPLETE);
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
                    else error("End if", SGS_SE_INCOMPLETE);
                }
				else error("If(end)", SGS_SE_INCOMPLETE);
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
                error("Loop(when)", SGS_SE_INCOMPLETE);
                cond = parseExp();
            }

            block->pushAST(new WhileStmt(line, cond, parseBlock()));
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
			int line = content[proc].line;
            int funcIdx;
            string newVar;
            if ((funcIdx = findFunc()) != -1) {
                if (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_WITH)
                    error(funcList[funcIdx]->getName().data(), SGS_SE_INCOMPLETE);
                else proc++;
                block->pushAST(new CallStmt(line, funcList[funcIdx], parseParam(funcIdx)));
            }
			else error("Function", SGS_SE_NOID);
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
            error("", SGS_SE_UNKNOWN);
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
    if (proc == content.size()) {
        error(tmp.data(), SGS_SE_INCOMPLETE);
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
    if (proc == content.size()) {
        error(tmp.data(), SGS_SE_INCOMPLETE);
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

bool Syntax::compare(int op1, int op2) {//op1<=op2
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
void Syntax::error(const char *inst, SYNTAXERROR type) {
    switch (type) {
    case SGS_SE_EXPOSE:
        msgList.emplace_back(string("语句块不封闭。\n"), MT_ERROR);
        break;
    case SGS_SE_UNIQUE:
        msgList.emplace_back(inst + string("语法错误。\n"), MT_ERROR);
        break;
    case SGS_SE_EXPDOT:
        msgList.emplace_back(string("缺少句号。\n"), MT_ERROR);
        break;
    case SGS_SE_EXPCOMMA:
        msgList.emplace_back(inst + string("缺少逗号。\n"), MT_ERROR);
        break;
    case SGS_SE_EXPBRACE:
        msgList.emplace_back(string("括号不匹配。\n"), MT_ERROR);
        break;
    case SGS_SE_REDEF:
        msgList.emplace_back(inst + string("重复定义。\n"), MT_WARNING);
        break;
    case SGS_SE_INVALIDTYPE:
        msgList.emplace_back(inst + string("类型错误。\n"), MT_WARNING);
        break;
    case SGS_SE_DISACCORD:
        msgList.emplace_back(inst + string("前后不一致。\n"), MT_WARNING);
        break;
    case SGS_SE_NOID:
        msgList.emplace_back(inst + string("未定义。\n"), MT_ERROR);
        break;
    case SGS_SE_INCOMPLETE:
        msgList.emplace_back(inst + string("不完整。\n"), MT_WARNING);
        break;
    case SGS_SE_UNKNOWN:
        msgList.emplace_back(string("未知错误。\n"), MT_ERROR);
        break;
    case SGS_SE_UNSUPPORT:
        msgList.emplace_back(string("暂不支持。\n"), MT_ERROR);
        break;
    }
}

