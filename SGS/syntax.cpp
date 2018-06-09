#define _CRT_SECURE_NO_WARNINGS
#include "syntax.h"
#include <iostream>
using namespace sgs;

SgsSyntax::SgsSyntax() {
    proc = 0;

    prepare();
}
SgsSyntax::SgsSyntax(vector<string> &ids, vector<SgsTokenPrim> &input) {
    proc = 0;

    content = input;
    strId = ids;

    prepare();
}
SgsSyntax::~SgsSyntax() = default;

void SgsSyntax::prepare() {

    funcList.push_back(new FuncProto(new BasicType(BT_INT), "getchar", {}));
    funcList.push_back(new FuncProto(new BasicType(BT_INT), "putchar", { std::make_pair(new BasicType(BT_INT), "value") }));
    funcList.push_back(new FuncProto(nullptr, "print an int", { std::make_pair(new BasicType(BT_INT), "value") }));
    funcList.push_back(new FuncProto(nullptr, "print a number", { std::make_pair(new BasicType(BT_FLOAT), "value") }));
    funcList.push_back(new FuncProto(nullptr, "print a str", { std::make_pair(new ArrayType(new BasicType(BT_CHAR), 0), "value") }));
    funcList.push_back(new FuncProto(new BasicType(BT_INT), "newline", {}));
    funcList.push_back(new FuncProto(new BasicType(BT_FLOAT), "intToFloat", { std::make_pair(new BasicType(BT_INT), "value")}));
    funcList.push_back(new FuncProto(new BasicType(BT_BOOL), "intToBool", { std::make_pair(new BasicType(BT_INT), "value") }));
    funcList.push_back(new FuncProto(new BasicType(BT_CHAR), "intToChar", { std::make_pair(new BasicType(BT_INT), "value") }));
    funcList.push_back(new FuncProto(new BasicType(BT_INT), "floatToInt", {std::make_pair(new BasicType(BT_FLOAT), "value")}));
    funcList.push_back(new FuncProto(new BasicType(BT_INT), "boolToInt", { std::make_pair(new BasicType(BT_BOOL), "value") }));
    funcList.push_back(new FuncProto(new BasicType(BT_INT), "charToInt", { std::make_pair(new BasicType(BT_CHAR), "value") }));
}

SgsSyntax *SgsSyntax::input(vector<string> &ids, vector<SgsTokenPrim> &src) {
    content = src;

    for (const auto& i : ids) {
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
            } else {
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
            } else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_FUNCTION) {
                proc++;
                FuncProto *tmpFunc = parseFuncDec();
                funcList.push_back(tmpFunc);
                stmts.push_back(tmpFunc);
            } else
                error("New", SGS_SE_UNIQUE);
            continue;
        } 
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LET) {
            proc++;

            Expression *left;
            int classIdx;
            string newVar;
            if (content[proc].type == SGS_TT_USER) {
                if ((classIdx = findClass()) >= 0) {
                    if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_ARRAY) {
                        proc++;
                        int length = int(content[proc].value);
                        proc++;
                        stmts.push_back(new VarDef(new ArrayType(classList[classIdx], length), newVar = parseUser()));
                    }
					else {
                        stmts.push_back(new VarDef(classList[classIdx], newVar = parseUser()));
                    }
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
                        int length = int(content[proc].value);
                        proc++;
                        stmts.push_back(new VarDef(new ArrayType(new BasicType(BT_INT), length), newVar = parseUser()));
                    } 
					else {
                        proc++;
                        stmts.push_back(new VarDef(new BasicType(BT_INT), newVar = parseUser()));
                    }
                    left = new IdExp(newVar);
                } 
				else if (content[proc].id == SGS_ID_FLOAT) {
                    if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
                        proc += 2;
                        int length = static_cast<int>(content[proc].value);
                        proc++;
                        stmts.push_back(new VarDef(new ArrayType(new BasicType(BT_FLOAT), length), newVar = parseUser()));
                    } else {
                        proc++;
                        stmts.push_back(new VarDef(new BasicType(BT_FLOAT), newVar = parseUser()));
                    }
                    left = new IdExp(newVar);
                } 
				else if (content[proc].id == SGS_ID_BOOL) {
                    if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
                        proc += 2;
                        int length = static_cast<int>(content[proc].value);
                        proc++;
                        stmts.push_back(new VarDef(new ArrayType(new BasicType(BT_BOOL), length), newVar = parseUser()));
                    } else {
                        proc++;
                        stmts.push_back(new VarDef(new BasicType(BT_BOOL), newVar = parseUser()));
                    }
                    left = new IdExp(newVar);
                }
				else if (content[proc].id == SGS_ID_CHAR) {
                    if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
                        proc += 2;
                        int length = (int)content[proc].value;
                        proc++;
                        stmts.push_back(new VarDef(new ArrayType(new BasicType(BT_CHAR), length), newVar = parseUser()));
                    } else {
                        proc++;
                        stmts.push_back(new VarDef(new BasicType(BT_CHAR), newVar = parseUser()));
                    }
                    left = new IdExp(newVar);
                } else if (content[proc].id == SGS_ID_STRING) {
                    if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
                        proc += 2;
                        int length = (int)content[proc].value;
                        proc++;
                        stmts.push_back(new VarDef(new ArrayType(new BasicType(BT_STRING), length), newVar = parseUser()));
                    } else {
                        proc++;
                        stmts.push_back(new VarDef(new BasicType(BT_STRING), newVar = parseUser()));
                    }
                    left = new IdExp(newVar);
                } else {
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
                    error(((FuncProto *)funcList[funcIdx])->getName().data(), SGS_SE_DISACCORD);
            } else {
                error(parseUser().data(), SGS_SE_NOID);
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
                } else stmts.push_back(new IfStmt(branch, taken, parseBlock()));
            } else {
                stmts.push_back(new IfStmt(branch, taken, nullptr));
            }

            if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_END) {
                proc++;
                if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_IF)
                    proc++;
                else error("End if", SGS_SE_INCOMPLETE);
            } else error("If(end)", SGS_SE_INCOMPLETE);

            continue;
        } 
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LOOP) {
            proc++;

            Expression *cond;
            if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_WHEN) {
                proc++;
                cond = parseExp();
            } else {
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
            } else error("Loop(end)", SGS_SE_INCOMPLETE);

            continue;
        } 
		else if (content[proc].type == SGS_TT_USER) {
            int funcIdx;
            string newVar;
            if ((funcIdx = findFunc()) != -1) {
                if (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_WITH)
                    stmts.push_back(new CallStmt(funcList[funcIdx], vector<Expression *>()));
                else {
                    proc++;
                    stmts.push_back(new CallStmt(funcList[funcIdx], parseParam(funcIdx)));
                }
            } else error("Function", SGS_SE_NOID);
            continue;
        } 
		else if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_DOT) {
            proc++;
            continue;
        }
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_RETURN) {
			proc++;
			error("return", SGS_SE_INCOMPLETE);
			continue;
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_BREAK) {
			proc++;
			error("break", SGS_SE_INCOMPLETE);
			continue;
		}
		else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_REDO) {
			proc++;
			error("redo", SGS_SE_INCOMPLETE);
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
                if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_LBRAKET) {
                    proc++;
                    value.push(parseClassConst(classIdx));
                }
				else
                    error(classList[classIdx]->getName().data(), SGS_SE_INCOMPLETE);
            }
			else if ((funcIdx = findFunc()) >= 0) {
                if (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_WITH)
                    value.push(new CallExp(funcList[funcIdx], vector<Expression *>()));
                else {
                    proc++;
                    value.push(new CallExp(funcList[funcIdx], parseParam(funcIdx)));
                }
            }
			else {
                value.push(parseVar());
            }
        } 
		else if (content[proc].type == SGS_TT_DATA) {
            if (content[proc].s != nullptr) {
                value.push(new StrLiteral(content[proc].s));
            }
			else {
                switch (content[proc].id) {
                case CT_INT:
                    value.push(new IntLiteral(int(content[proc].value)));
                    break;
                case CT_FLOAT:
                    value.push(new FloatLiteral(content[proc].value));
                    break;
                default:;
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
                    tmp->setLeft(b);
                    tmp->setRight(a);
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
                        tmp->setLeft(b);
                        tmp->setRight(a);
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
        tmp->setLeft(b);
        tmp->setRight(a);
        value.push(tmp);
    }

    if (value.size() == 1) {
        return value.top();
    } 
	// else {
 //        VarType *t = value.top()->getResType();
 //        vector<Expression *> cont;
 //
 //        while (!value.empty()) {
 //            if (value.top()->getResType() != t)
 //                error("array", SGS_SE_INVALIDTYPE);
 //            cont.push_back(value.top());
 //            value.pop();
 //        }
 //        return new ArrayLiteral(t, cont);
 //    }
    return nullptr;
}
Expression *SgsSyntax::parseVar() {
    Expression *ret = nullptr;

    while (proc < content.size()) {
        if (content[proc].type == SGS_TT_USER ||
            (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_RESULT)) {
            ret = new IdExp(parseUser());
        } else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_OF) {
            proc++;
            ret = new AccessExp(ret, parseUser());
        } else if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_LPARENTHESIS) {
            proc++;
            ret = new VisitExp(ret, parseExp());
            if (content[proc].type != SGS_TT_OP || content[proc].id != SGS_OP_RPARENTHESIS)
                error("array index", SGS_SE_EXPBRACE);
            else proc++;
        } else break;
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
				if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_ARRAY) {
					proc++;
					int length = int(content[proc].value);
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
					int length = int(content[proc].value);
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
					int length = int(content[proc].value);
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
					int length = int(content[proc].value);
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
					int length = int(content[proc].value);
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
					int length = int(content[proc].value);
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
    return new ClassDef(new ClassType(name, elements));
}
ClassLiteral *SgsSyntax::parseClassConst(int classid) {
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
        } else {
            ele[idx] = parseExp();
            proc++;
        }
    }
    return new ClassLiteral(classList[classid]->getName(), classList[classid]->getEle(), ele);
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
					if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_ARRAY) {
						proc++;
						int length = int(content[proc].value);
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
						int length = int(content[proc].value);
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
						int length = int(content[proc].value);
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
						int length = int(content[proc].value);
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
						int length = int(content[proc].value);
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
						int length = int(content[proc].value);
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
			else 
				break;
        }
    }
    if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_RETURN) {
        proc++;
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
                return new FuncProto(new BasicType(BT_CHAR), name, params);
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
        } else {
            para[idx] = parseExp();
			if (proc >= content.size())break;
			if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_COMMA)
				proc++;
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
            int classIdx;
            string newVar;
            if (content[proc].type == SGS_TT_USER ||
                (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_RESULT)) {
                if ((classIdx = findClass()) >= 0) {
                    block->pushAST(new VarDef(classList[classIdx], newVar = parseUser()));
                    left = new IdExp(newVar);
                } else {
                    left = parseExp();
                }
            } else if (content[proc].type == SGS_TT_SYS) {
                if (content[proc].id == SGS_ID_INTEGER) {
                    if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
                        proc += 2;
                        int length = static_cast<int>(content[proc].value);
                        proc++;
                        block->pushAST(new VarDef(new ArrayType(new BasicType(BT_INT), length), newVar = parseUser()));
                    } else {
                        proc++;
                        block->pushAST(new VarDef(new BasicType(BT_INT), newVar = parseUser()));
                    }
                    left = new IdExp(newVar);
                } else if (content[proc].id == SGS_ID_FLOAT) {
                    if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
                        proc += 2;
                        int length = static_cast<int>(content[proc].value);
                        proc++;
                        block->pushAST(new VarDef(new ArrayType(new BasicType(BT_FLOAT), length), newVar = parseUser()));
                    } else {
                        proc++;
                        block->pushAST(new VarDef(new BasicType(BT_FLOAT), newVar = parseUser()));
                    }
                    left = new IdExp(newVar);
                } else if (content[proc].id == SGS_ID_BOOL) {
                    if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
                        proc += 2;
                        int length = static_cast<int>(content[proc].value);
                        proc++;
                        block->pushAST(new VarDef(new ArrayType(new BasicType(BT_BOOL), length), newVar = parseUser()));
                    } else {
                        proc++;
                        block->pushAST(new VarDef(new BasicType(BT_BOOL), newVar = parseUser()));
                    }
                    left = new IdExp(newVar);
                } else if (content[proc].id == SGS_ID_CHAR) {
                    if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
                        proc += 2;
                        int length = static_cast<int>(content[proc].value);
                        proc++;
                        block->pushAST(new VarDef(new ArrayType(new BasicType(BT_CHAR), length), newVar = parseUser()));
                    } else {
                        proc++;
                        block->pushAST(new VarDef(new BasicType(BT_CHAR), newVar = parseUser()));
                    }
                    left = new IdExp(newVar);
                } else if (content[proc].id == SGS_ID_STRING) {
                    if (content[proc + 1].type == SGS_TT_SYS && content[proc + 1].id == SGS_ID_ARRAY) {
                        proc += 2;
                        int length = static_cast<int>(content[proc].value);
                        proc++;
                        block->pushAST(new VarDef(new ArrayType(new BasicType(BT_STRING), length), newVar = parseUser()));
                    } else {
                        proc++;
                        block->pushAST(new VarDef(new BasicType(BT_STRING), newVar = parseUser()));
                    }
                    left = new IdExp(newVar);
                } else {
                    error(strId[content[proc].id].data(), SGS_SE_UNIQUE);
                    skipLine();
                    continue;
                }
            } else {
                error("Let", SGS_SE_UNIQUE);
                skipLine();
                continue;
            }

            if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_BE) {
                proc++;
                block->pushAST(new AssignStmt(left, parseExp()));
            }
            continue;
        } else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_IF) {
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
                } else block->pushAST(new IfStmt(branch, taken, parseBlock()));
            } else {
                block->pushAST(new IfStmt(branch, taken, nullptr));
            }

            if (!untaken) {
                if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_END) {
                    proc++;
                    if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_IF)
                        proc++;
                    else error("End if", SGS_SE_INCOMPLETE);
                } else error("If(end)", SGS_SE_INCOMPLETE);
            }

            if (untaken)break;
            else continue;
        } else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_LOOP) {
            proc++;

            Expression *cond;
            if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_WHEN) {
                proc++;
                cond = parseExp();
            } else {
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
            } else error("Loop(end)", SGS_SE_INCOMPLETE);

            continue;
        } else if (content[proc].type == SGS_TT_USER) {
            int funcIdx;
            string newVar;
            if ((funcIdx = findFunc()) != -1) {
                if (content[proc].type != SGS_TT_SYS || content[proc].id != SGS_ID_WITH)
                    error(funcList[funcIdx]->getName().data(), SGS_SE_INCOMPLETE);
                else proc++;
                block->pushAST(new CallStmt(funcList[funcIdx], parseParam(funcIdx)));
            } else error("Function", SGS_SE_NOID);
            continue;
        } else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_RETURN) {
            proc++;
            block->pushAST(new ReturnStmt());
        } else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_BREAK) {
            proc++;
            block->pushAST(new BreakStmt());
        } else if (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_REDO) {
            proc++;
            block->pushAST(new ContinueStmt());
        } else if (content[proc].type == SGS_TT_OP && content[proc].id == SGS_OP_DOT) {
            proc++;
            continue;
        } else if ((content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_END) ||
            (content[proc].type == SGS_TT_SYS && content[proc].id == SGS_ID_ELSE)) {
            break;
        } else {
            error("", SGS_SE_UNKNOWN);
            skipLine();
            continue;
        }
    }
    return block;
}

string SgsSyntax::parseUser(string guide) {
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
void SgsSyntax::error(const char *inst, SGSYNTAXERROR type) {
    switch (type) {
    case SGS_SE_EXPOSE:
        msgList.emplace_back(string("Unclosed block statement.\n"), MT_ERROR);
        break;
    case SGS_SE_UNIQUE:
        msgList.emplace_back(inst + string("is an unrecogenized unique operator usage.\n"), MT_ERROR);
        break;
    case SGS_SE_EXPDOT:
        msgList.emplace_back(string("'.' is missing.\n"), MT_ERROR);
        break;
    case SGS_SE_EXPCOMMA:
        msgList.emplace_back(inst + string(": ',' is missing.\n"), MT_ERROR);
        break;
    case SGS_SE_EXPBRACE:
        msgList.emplace_back(string("Mismatching braces.\n"), MT_ERROR);
        break;
    case SGS_SE_REDEF:
        msgList.emplace_back(inst + string(" Redefinition.\n"), MT_WARNING);
        break;
    case SGS_SE_INVALIDTYPE:
        msgList.emplace_back(inst + string(" has invalid type.\n"), MT_WARNING);
        break;
    case SGS_SE_DISACCORD:
        msgList.emplace_back(inst + string(" is disaccord.\n"), MT_WARNING);
        break;
    case SGS_SE_NOID:
        msgList.emplace_back(inst + string(" is undefined.\n"), MT_ERROR);
        break;
    case SGS_SE_INCOMPLETE:
        msgList.emplace_back(inst + string(" uncomplete statement.\n"), MT_WARNING);
        break;
    case SGS_SE_UNKNOWN:
        msgList.emplace_back(string("Unknown error.\n"), MT_ERROR);
        break;
    case SGS_SE_UNSUPPORT:
        msgList.emplace_back(string("Will be supported.\n"), MT_ERROR);
        break;
    }
}
