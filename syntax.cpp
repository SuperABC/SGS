#define _CRT_SECURE_NO_WARNINGS
#include "syntax.h"

Syntax::Syntax(){
	proc = 0;

	prepare();

	output = new stateSeq();//Safely released [0].
	last = output;
}
Syntax::Syntax(vector<string> &ids, vector<tokenPrim> &input) {
	proc = 0;

	content = input;
	strId = ids;

	prepare();

	output = new stateSeq();//Safely released [0].
	last = output;
}
Syntax::~Syntax() {
	stateSeq *tmp = output;
	while (output != last) {
		tmp = output->next;
		delete output;//Releasing [0].
		output = tmp;
	}
	delete tmp;//Releasing [0].
}
void Syntax::prepare() {
	funcNode out = funcNode(funcType(string("out")));
	out.declare.varList.push_back(varNode(VT_STRING, "content"));
	globeFunc.push_back(out);

	funcNode outln = funcNode(funcType(string("outln")));
	outln.declare.varList.push_back(varNode(VT_STRING, "content"));
	globeFunc.push_back(outln);

	funcNode in = funcNode(funcType(string("in")));
	in.localVar.push_back(varNode());
	globeFunc.push_back(in);

	funcNode time = funcNode(funcType(string("current time")));
	time.localVar.push_back(varNode());
	globeFunc.push_back(time);

	funcNode random = funcNode(funcType(string("random number")));
	random.localVar.push_back(varNode());
	globeFunc.push_back(random);
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

Syntax *Syntax::input(vector<string> &ids, vector<tokenPrim> &src) {
	content = src;

	stateSeq *tmp = output;
	while (output != last) {
		tmp = output->next;
		delete output;//Releasing [0].
		output = tmp;
	}
	delete tmp;//Releasing [0].
	output = new stateSeq();//Safely released [0].
	last = output;
	
	for (auto i : ids) {
		unsigned int j = 0;
		for (; j < strId.size(); j++) {
			if (i == strId[j])break;
		}
		if (j == strId.size())strId.push_back(i);
	}
	return this;
}
stateSeq *Syntax::parse() {
	proc = 0;

	while (proc < content.size()) {
		if (content[proc].type == TT_SYS&&content[proc].id == ID_USE) {
			proc++;
			if (content[proc].type == TT_SYS&&content[proc].id == ID_LIBRARY) {
				proc++;
				parseLibrary(strId[content[proc].id]);

				proc++;
				/*if (content[proc].type == TT_OP&&content[proc].id != OP_DOT)
					error("Use library", SE_EXPDOT);
				proc++;*/
			}
			else {
				proc++;
				error("Use", SE_UNIQUE);
			}
			continue;
		}
		else if (content[proc].type == TT_SYS&&content[proc].id == ID_NEW) {
			proc++;
			if (content[proc].type == TT_SYS && content[proc].id == ID_CLASS) {
				proc++;
				globeClassType.push_back(parseClassDec());
			}
			else if (content[proc].type == TT_SYS && content[proc].id == ID_FUNCTION) {
				proc++;
				globeFunc.push_back(parseFuncDec());
			}
			else
				error("New", SE_UNIQUE);
			continue;
		}
		else if (content[proc].type == TT_SYS&&content[proc].id == ID_LET) {
			proc++;
			string newVar;
			int varType;
			int classIdx = -1;
			if (content[proc].type == TT_USER) {
				if ((classIdx = findClass()) >= 0) {
					varType = VT_CLASS;
					newVar = findVar();
					if (newVar[0] != '$') {
						globeVar.push_back(varNode(varType, newVar, globeClassType[classIdx].name));
					}
					else
						error(newVar.c_str(), SE_REDEF);
				}
				else if ((newVar = findVar())[0] == '$') {
					newVar = newVar.c_str() + 1;
					if (newVar[0] >= '0' &&newVar[0] <= '9') {
						string comp = newVar;
						int offset = atoi(newVar.c_str());
						newVar = newVar.c_str() + newVar.find('@') + 1;

						for (auto n : globeVar) {
							if (n.name == newVar) {
								varType = n.t;
								break;
							}
						}
						newVar = comp;
					}
					else {
						for (auto n : globeVar) {
							if (n.name == newVar) {
								varType = n.t;
								break;
							}
						}
					}
				}
				else
					error(strId[content[proc].id].c_str(), SE_NOID);
			}
			else if (content[proc].type == TT_SYS) {
				if ((varType = findType()) != -1) {
					if ((newVar = findVar())[0] == '$')
						error(strId[content[proc].id].c_str(), SE_REDEF);
					else {
						globeVar.push_back(varNode(varType, newVar));
					}
				}
				else
					error(strId[content[proc].id].c_str(), SE_UNIQUE);
			}
			else
				error("Let", SE_UNIQUE);

			if (content[proc].type == TT_OP&&content[proc].id == OP_DOT)
				continue;
			else if (content[proc].type == TT_SYS&&content[proc].id == ID_BE) {
				proc++;
				rtVal st = rtVal();
				st.op = VO_ASSIGN;
				if (classIdx == -1)
					st.left = new varNode(varType, newVar);//Safely released [1].
				else
					st.left = new varNode(varType, newVar, globeClassType[classIdx].name);
				st.right = parseExpression();//Safely released [2].
				last->act = st;
				last->next = new stateSeq();//Safely released [0].
				last = last->next;
			}
			else
				error("Let(be)", SE_INCOMPLETE);
			continue;
		}
		else if (content[proc].type == TT_SYS&&content[proc].id == ID_START) {
			proc++;
			int funcIdx;
			if ((funcIdx = findFunc()) >= 0) {
				parseFuncDef(funcIdx);
				if (funcIdx != findFunc())
					error(globeFunc[funcIdx].declare.name.c_str(), SE_DISACCORD);
			}
			else
				error(strId[content[proc].id].c_str(), SE_NOID);
			continue;
		}
		else if (content[proc].type == TT_SYS && content[proc].id == ID_IF) {
			proc++;

			varNode *branch = parseExpression();
			if (content[proc].type != TT_SYS || content[proc].id != ID_THEN)
				error("If(then)", SE_INCOMPLETE);
			proc++;

			rtVal st = rtVal();
			st.op = VO_IF;
			st.left = branch;
			varNode *accept = new varNode(VT_FUNCTION, "");
			accept->block = parseBlock();
			varNode * reject = new varNode(VT_FUNCTION, "");
			if (content[proc].type == TT_SYS && content[proc].id == ID_ELSE) {
				proc++;
				reject->block = parseBlock();
			}
			st.right = new varNode(VT_FUNCTION, "");
			st.right->left = accept;
			st.right->right = reject;

			if (content[proc].type == TT_SYS && content[proc].id == ID_END) {
				proc++;
				if (content[proc].type == TT_SYS && content[proc].id == ID_IF)
					proc++;
				else
					error("End if", SE_INCOMPLETE);
			}

			last->act = st;
			last->next = new stateSeq();//Safely released [0].
			last = last->next;

			continue;
		}
		else if (content[proc].type == TT_SYS && content[proc].id == ID_LOOP) {
			proc++;

			varNode *cond = NULL;
			if (content[proc].type == TT_SYS && content[proc].id == ID_WHEN) {
				proc++;
				cond = parseExpression();
			}

			rtVal st = rtVal();
			st.op = VO_WHILE;
			st.left = cond;
			varNode *body = new varNode(VT_FUNCTION, "");
			body->block = parseBlock();
			st.right = body;

			if (content[proc].type == TT_SYS && content[proc].id == ID_END) {
				proc++;
				if (content[proc].type == TT_SYS && content[proc].id == ID_LOOP)
					proc++;
				else
					error("End loop", SE_INCOMPLETE);
			}

			last->act = st;
			last->next = new stateSeq();//Safely released [0].
			last = last->next;

			continue;
		}
		else if (content[proc].type == TT_USER) {
			int funcIdx;
			if ((funcIdx = findFunc()) != -1) {
				rtVal st = rtVal();

				st.op = VO_EXE;
				st.left = new varNode(VT_FUNCTION, globeFunc[funcIdx].declare.name);
				st.right = parseParameter(funcIdx);
				last->act = st;
				last->next = new stateSeq();//Safely released [0].
				last = last->next;
			}
		}
		else if (content[proc].type == TT_OP&&content[proc].id == OP_DOT) {
			proc++;
			continue;
		}
	}

	return output;
}

void Syntax::parseLibrary(string lib) {//Not finished.

}
string Syntax::parseUser() {
	string tmp;
	while (proc < content.size() && content[proc].type == TT_USER) {
		tmp += strId[content[proc++].id];
		tmp += " ";
	}
	if (proc == content.size()) {
		error(tmp.c_str(), SE_INCOMPLETE);
	}
	tmp.pop_back();
	return tmp;
}
varNode *Syntax::parseExpression() {
	std::stack<int> op;
	std::stack<varNode*> value;

	while (proc < content.size()) {
		if (content[proc].type == TT_SYS && content[proc].id != ID_RESULT)break;
		else if (content[proc].type == TT_OP && 
			(content[proc].id == OP_DOT || content[proc].id == OP_COMMA ||
			content[proc].id == OP_LPARENTHESIS || content[proc].id == OP_RPARENTHESIS ||
			content[proc].id == OP_LBRAKET || content[proc].id == OP_RBRAKET))break;
		else if (content[proc].type == TT_USER ||
			content[proc].type == TT_SYS && content[proc].id == ID_RESULT) {
			int pre = proc;

			string varTmp;
			int funcIdx;
			int classIdx;

			if ((classIdx = findClass()) >= 0) {
				if (content[proc].type == TT_OP && content[proc].id == OP_LBRAKET) {
					proc++;
					varNode *tmp = new varNode(VT_CLASS, "", globeClassType[classIdx].name);
					classNode *node = new classNode(globeClassType[classIdx], "");

					varNode *ele;
					unsigned int i;
					for (i = 0; i < globeClassType[classIdx].varList.size(); i++) {
						ele = parseExpression();
						node->varList.push_back(*ele);

						if (content[proc].type == TT_OP && content[proc].id == OP_COMMA) {
							proc++;
							continue;
						}
						if (content[proc].type == TT_OP && content[proc].id == OP_RBRAKET) {
							proc++;
							break;
						}
					}

					tmp->val = node;
					value.push(tmp);
				}
				else
					error(globeClassType[classIdx].name.c_str(), SE_INCOMPLETE);
			}
			else if ((varTmp = findVar())[0] == '$') {

				varNode *tmp = new varNode(VT_VAR, "");
				string *n = new string(varTmp.c_str() + 1);
				tmp->val = n->c_str();

				value.push(tmp);
			}
			else {
				proc = pre;
				if ((funcIdx = findFunc()) != -1) {
					rtVal st = rtVal();

					st.op = VO_EXE;
					st.left = new varNode(VT_FUNCTION, globeFunc[funcIdx].declare.name);
					st.right = parseParameter(funcIdx);

					varNode *tmp = new varNode(VT_FUNCTION, "");
					tmp->t = VT_FUNCTION;
					tmp->block = new stateSeq();
					tmp->block->act = st;

					value.push(tmp);
				}
			}
		}
		else if (content[proc].type == TT_DATA) {
			varNode *tmp = new varNode();
			if (content[proc].s != NULL) {
				tmp->t = VT_STRING;
				tmp->val = malloc(sizeof(content[proc].s) + 1);
				strcpy((char *)tmp->val, content[proc].s);
			}
			else {
				switch (content[proc].id) {
				case CT_INT:
					tmp->t = VT_INTEGER;
					tmp->val = new int((int)content[proc].value);
					break;
				case CT_FLOAT:
					tmp->t = VT_FLOAT;
					tmp->val = new float((float)content[proc].value);
					break;
				case CT_CHAR:
					tmp->t = VT_CHAR;
					tmp->val = new char((char)content[proc].value);
					break;
				case CT_BOOL:
					tmp->t = VT_BOOL;
					tmp->val = new bool((bool)content[proc].value);
					break;
				}
			}

			value.push(tmp);
			proc++;
		}
		else if (content[proc].type == TT_OP) {
			if (content[proc].id == OP_LBRACE) {
				op.push(OP_LBRACE);
			}
			else if (content[proc].id == OP_RBRACE) {
				while (op.top() != OP_LBRACE) {
					int o = op.top();
					op.pop();
					if (value.size() < 2)
						error("", SE_INCOMPLETE);
					varNode *a = value.top();
					value.pop();
					varNode *b = value.top();
					value.pop();

					varNode *tmp = new varNode(VT_EXP, "");
					tmp->val = new int(o);
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
							error("", SE_INCOMPLETE);
						varNode *a = value.top();
						value.pop();
						varNode *b = value.top();
						value.pop();

						varNode *tmp = new varNode(VT_EXP, "");
						tmp->val = new int(o);
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
			error("", SE_INCOMPLETE);
		varNode *a = value.top();
		value.pop();
		varNode *b = value.top();
		value.pop();

		varNode *tmp = new varNode(VT_EXP, "");
		tmp->val = new int(o);
		tmp->left = a;
		tmp->right = b;
		value.push(tmp);
	}

	if (value.size() == 1) {
		return value.top();
	}
	else {
		varNode *tmp = NULL;
		while (!value.empty()) {
			value.top()->next = tmp;
			tmp = value.top();
			value.pop();
		}
		varNode *arr = new varNode(VT_ARRAY, "");
		arr->val = tmp;
		return arr;
	}
}
funcNode Syntax::parseFuncDec() {
	funcNode ret = funcNode();
	string tmp = parseUser();

	if (content[proc].type == TT_SYS && content[proc].id == ID_WITH) {
		proc++;
		ret.declare.name = tmp;

		int type, cla;
		while (true) {
			if ((type = findType()) != -1) {
				ret.declare.varList.push_back(varNode((varType)type, parseUser()));
			}
			else if ((cla = findClass()) != -1) {
				ret.declare.varList.push_back(varNode(VT_CLASS, parseUser(), globeClassType[cla].name));
			}

			if (content[proc].type == TT_OP && content[proc].id == OP_DOT)break;
			if (content[proc].type == TT_OP && content[proc].id == OP_COMMA)proc++;
		}
	}
	else if (content[proc].type == TT_OP && content[proc].id == OP_DOT) {
		proc++;
		ret.declare.name = tmp;
	}
	else error("New function ", SE_INCOMPLETE);

	return ret;
}
stateSeq Syntax::parseFuncDef(int funcid) {
	int preFunc = func;
	func = funcid;
	globeFunc[funcid].localVar.push_back(varNode(VT_NULL, "result"));
	for(auto v : globeFunc[funcid].declare.varList)
		globeFunc[funcid].localVar.push_back(v);

	stateSeq ret = stateSeq();
	stateSeq *last = &ret;
	while (content[proc].type != TT_SYS || content[proc].id != ID_END) {
		if (content[proc].type == TT_SYS&&content[proc].id == ID_LET) {
			proc++;
			string newVar;
			int varType;
			int classIdx;
			if (content[proc].type == TT_USER) {
				string valueType;
				if (classIdx = findClass() >= 0) {
					varType = VT_CLASS;
					newVar = findVar();
					if (newVar[0] != '$') {
						globeFunc[funcid].localVar.push_back(varNode(varType, newVar, globeClassType[classIdx].name));
					}
					else
						error(newVar.c_str(), SE_REDEF);
				}
				else if ((valueType = findVar())[0] == '$') {
					newVar = newVar.c_str() + 1;
					if (newVar[0] >= '0' &&newVar[0] <= '9') {
						string comp = newVar;
						int offset = atoi(newVar.c_str());
						newVar = newVar.c_str() + newVar.find('@') + 1;

						for (auto n : globeVar) {
							if (n.name == newVar) {
								varType = n.t;
								break;
							}
						}
						for (auto n : globeFunc[funcid].localVar) {
							if (n.name == newVar) {
								varType = n.t;
								break;
							}
						}
						newVar = comp;
					}
					else {
						for (auto n : globeVar) {
							if (n.name == newVar) {
								varType = n.t;
								break;
							}
						}
						for (auto n : globeFunc[funcid].localVar) {
							if (n.name == newVar) {
								varType = n.t;
								break;
							}
						}
					}
				}
				else
					error(strId[content[proc].id].c_str(), SE_NOID);
			}
			else if (content[proc].type == TT_SYS) {
				if (content[proc].id == ID_RESULT) {
					varType = VT_NULL;
					newVar = "result";
					proc++;
				}
				else if ((varType = findType()) != -1) {
					if ((newVar = findVar())[0] == '$')
						error(strId[content[proc].id].c_str(), SE_REDEF);
					else {
						globeFunc[funcid].localVar.push_back(varNode(varType, newVar));
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
				st.right = parseExpression();
				last->act = st;
				last->next = new stateSeq();
				last = last->next;
			}
			else
				error("", SE_INCOMPLETE);
			continue;
		}
		else if (content[proc].type == TT_SYS && content[proc].id == ID_IF) {
			proc++;

			varNode *branch = parseExpression();
			if (content[proc].type != TT_SYS || content[proc].id != ID_THEN)
				error("if", SE_INCOMPLETE);
			proc++;

			rtVal st = rtVal();
			st.op = VO_IF;
			st.left = branch;
			varNode *accept = new varNode(VT_FUNCTION, "");
			accept->block = parseBlock();
			varNode * reject = new varNode(VT_FUNCTION, "");
			if (content[proc].type == TT_SYS && content[proc].id == ID_ELSE) {
				proc++;
				reject->block = parseBlock();
			}
			st.right = new varNode(VT_FUNCTION, "");
			st.right->left = accept;
			st.right->right = reject;

			if (content[proc].type == TT_SYS && content[proc].id == ID_END) {
				proc += 2;
			}

			last->act = st;
			last->next = new stateSeq();
			last = last->next;
		}
		else if (content[proc].type == TT_SYS && content[proc].id == ID_LOOP) {
			proc++;

			varNode *cond = NULL;
			if (content[proc].type == TT_SYS && content[proc].id == ID_WHEN) {
				proc++;
				cond = parseExpression();
			}

			rtVal st = rtVal();
			st.op = VO_WHILE;
			st.left = cond;
			varNode *body = new varNode(VT_FUNCTION, "");
			body->block = parseBlock();
			st.right = body;

			if (content[proc].type == TT_SYS && content[proc].id == ID_END) {
				proc += 2;
			}

			last->act = st;
			last->next = new stateSeq();
			last = last->next;
		}
		else if (content[proc].type == TT_USER) {
			int funcIdx;
			rtVal st = rtVal();
			if ((funcIdx = findFunc()) != -1) {
				st.op = VO_EXE;
				st.left = new varNode(VT_FUNCTION, globeFunc[funcIdx].declare.name);
				st.right = parseParameter(funcIdx);
				last->act = st;
				last->next = new stateSeq();
				last = last->next;
			}
		}
		else if (content[proc].type == TT_OP&&content[proc].id == OP_DOT) {
			proc++;
			continue;
		}
	}
	proc++;
	globeFunc[funcid].content = ret;

	func = preFunc;
	return ret;
}
varNode *Syntax::parseParameter(int funcid){
	int parNum = globeFunc[funcid].declare.varList.size();
	varNode *rt = new varNode();
	varNode *last = rt;

	if (parNum == 0) {
		delete rt;
		return NULL;
	}
	else {
		if (content[proc].type == TT_SYS&&content[proc].id == ID_WITH) {
			proc++;
			for (unsigned int i = 0; i < globeFunc[funcid].declare.varList.size(); i++) {
				last->t = VT_VAR;
				last->name = globeFunc[funcid].declare.varList[i].name;
				if (i == globeFunc[funcid].declare.varList.size() - 1)break;
				last->next = new varNode();
				last = last->next;
			}
			last->next = NULL;

			string tmp;
			bool accord = false;
			varNode *iter;
			while (content[proc].type != TT_OP && content[proc].id != OP_DOT) {
				tmp.clear();
				while (content[proc].type == TT_USER ||
					content[proc].type == TT_SYS && content[proc].id == ID_RESULT) {
					tmp += strId[content[proc++].id];
					for (auto p : globeFunc[funcid].declare.varList) {
						if (p.name == tmp)accord = true;
					}
					tmp += " ";
					if (accord == true)break;
				}
				tmp.pop_back();
				if (accord == true)accord = false;
				else error(tmp.c_str(), SE_NOID);

				iter = rt;
				while (iter != NULL) {
					if (iter->name == tmp) {
						varNode * val = parseExpression();
						iter->t = val->t;
						iter->left = val->left;
						iter->right = val->right;
						iter->val = val->val;
						iter->block = val->block;
						break;
					}
					iter = iter->next;
				}

				if (content[proc].type != TT_OP || content[proc].id != OP_COMMA) {
					if (content[proc].type != TT_OP || content[proc].id != OP_DOT)
						error("", SE_EXPCOMMA);
				}
				else proc++;
			}
		}
		else error((string("Start ") + globeFunc[funcid].declare.name).c_str(), SE_INCOMPLETE);
		return rt;
	}
}
classType Syntax::parseClassDec() {
	classType ret = classType();
	string tmp = parseUser();

	if (content[proc].type == TT_SYS && content[proc].id == ID_WITH) {
		proc++;
		ret.name = tmp;

		int type, cla;
		while (true) {
			if ((type = findType()) != -1) {
				ret.varList.push_back(varNode((varType)type, parseUser()));
			}
			else if ((cla = findClass()) != -1) {
				//ret.classList.push_back(classNode(globeClassType[cla], parseUser()));
			}

			if (content[proc].type == TT_OP && content[proc].id == OP_DOT)break;
			if (content[proc].type == TT_OP && content[proc].id == OP_COMMA)proc++;
		}
	}
	else error("New class ", SE_INCOMPLETE);

	return ret;
}
classNode Syntax::parseClassConstr() {//Not finished.
	classNode ret = classNode();
	return ret;
}
stateSeq *Syntax::parseBlock() {
	stateSeq *ret = new stateSeq();
	stateSeq *last = ret;
	while ((content[proc].type != TT_SYS || content[proc].id != ID_END)&&
		(content[proc].type != TT_SYS || content[proc].id != ID_ELSE)) {
		if (content[proc].type == TT_SYS && content[proc].id == ID_LET) {
			proc++;
			string newVar;
			int varType;
			if (content[proc].type == TT_USER) {
				int classIdx;
				if (classIdx = findClass() >= 0) {
					varType = VT_CLASS;
					newVar = findVar();
					if (newVar[0] != '$') {
						if(func==-1)
							globeVar.push_back(varNode(varType, newVar, globeClassType[classIdx].name));
						else
							globeFunc[func].localVar.push_back(varNode(varType, newVar, globeClassType[classIdx].name));
					}
					else
						error(newVar.c_str(), SE_REDEF);
				}
				else if ((newVar = findVar())[0] == '$') {
					newVar = newVar.c_str() + 1;

					if (newVar[0] >= '0' &&newVar[0] <= '9') {
						string comp = newVar;
						int offset = atoi(newVar.c_str());
						newVar = newVar.c_str() + newVar.find('@') + 1;

						for (auto n : globeVar) {
							if (n.name == newVar) {
								varType = n.t;
								break;
							}
						}
						if (func != -1) {
							for (auto n : globeFunc[func].localVar) {
								if (n.name == newVar) {
									varType = n.t;
									break;
								}
							}
						}
						newVar = comp;
					}
					else {
						for (auto n : globeVar) {
							if (n.name == newVar) {
								varType = n.t;
								break;
							}
						}
						if (func != -1) {
							for (auto &n : globeFunc[func].localVar) {
								if (n.name == newVar) {
									varType = n.t;
									break;
								}
							}
						}
					}
				}
				else
					error(strId[content[proc].id].c_str(), SE_NOID);
			}
			else if (content[proc].type == TT_SYS) {
				if (content[proc].id == ID_RESULT) {
					varType = VT_NULL;
					newVar = "result";
					proc++;
				}
				else if ((varType = findType()) != -1) {
					if ((newVar = findVar())[0] == '$')
						error(strId[content[proc].id].c_str(), SE_REDEF);
					else {
						if(func==-1)
							globeVar.push_back(varNode(varType, newVar));
						else
							globeFunc[func].localVar.push_back(varNode(varType, newVar));
					}
				}
				else
					error(strId[content[proc].id].c_str(), SE_UNIQUE);
			}

			if (content[proc].type == TT_OP && content[proc].id == OP_DOT)
				continue;
			else if (content[proc].type == TT_SYS && content[proc].id == ID_BE) {
				proc++;
				rtVal st = rtVal();
				st.op = VO_ASSIGN;
				st.left = new varNode(varType, newVar);
				st.right = parseExpression();
				last->act = st;
				last->next = new stateSeq();
				last = last->next;
			}
			else
				error("", SE_INCOMPLETE);
			continue;
		}
		else if (content[proc].type == TT_SYS && content[proc].id == ID_IF) {
			proc++;

			varNode *branch = parseExpression();
			if (content[proc].type != TT_SYS || content[proc].id != ID_THEN)
				error("if", SE_INCOMPLETE);
			proc++;

			rtVal st = rtVal();
			st.op = VO_IF;
			st.left = branch;
			varNode *accept = new varNode(VT_FUNCTION, "");
			accept->block = parseBlock();
			varNode * reject = new varNode(VT_FUNCTION, "");
			if (content[proc].type == TT_SYS && content[proc].id == ID_ELSE) {
				proc++;
				reject->block = parseBlock();
			}
			st.right = new varNode(VT_FUNCTION, "");
			st.right->left = accept;
			st.right->right = reject;

			if (content[proc].type == TT_SYS && content[proc].id == ID_END) {
				proc += 2;
			}

			last->act = st;
			last->next = new stateSeq();
			last = last->next;
		}
		else if (content[proc].type == TT_SYS && content[proc].id == ID_LOOP) {
			proc++;

			varNode *cond = NULL;
			if (content[proc].type == TT_SYS && content[proc].id == ID_WHEN) {
				proc++;
				cond = parseExpression();
			}

			rtVal st = rtVal();
			st.op = VO_WHILE;
			st.left = cond;
			varNode *body = new varNode(VT_FUNCTION, "");
			body->block = parseBlock();
			st.right = body;

			if (content[proc].type == TT_SYS && content[proc].id == ID_END) {
				proc += 2;
			}

			last->act = st;
			last->next = new stateSeq();
			last = last->next;
		}
		else if (content[proc].type == TT_USER) {
			int funcIdx;
			rtVal st = rtVal();
			if ((funcIdx = findFunc()) != -1) {
				st.op = VO_EXE;
				st.left = new varNode(VT_FUNCTION, globeFunc[funcIdx].declare.name);
				st.right = parseParameter(funcIdx);
				last->act = st;
				last->next = new stateSeq();
				last = last->next;
			}
		}
		else if (content[proc].type == TT_OP && content[proc].id == OP_DOT) {
			proc++;
			continue;
		}
	}
	return ret;
}
string Syntax::findVar() {
	string tmp;
	while (proc < content.size() && content[proc].type == TT_USER ||
		content[proc].type == TT_SYS && content[proc].id == ID_RESULT) {
		tmp += strId[content[proc++].id];

		for (auto n : globeVar) {
			if (n.name == tmp) {
				if (n.t == VT_ARRAY) {
					int offset;
					if (proc < content.size() &&
						content[proc].type == TT_OP && content[proc].id == OP_LPARENTHESIS) {
						proc++;
						varNode *ofvar = parseExpression();
						if (ofvar->t != VT_INTEGER)
							error("Array index", SE_INVALIDTYPE);
						if (content[proc].type != TT_OP || content[proc].id != OP_RPARENTHESIS)
							error("[]", SE_EXPBRACE);
						proc++;
						offset = *(int *)ofvar->val;

						char buf[16];
						_itoa(offset, buf, 10);
						return string("$") + buf + "@" + tmp;
					}
				}
				return "$" + tmp;
			}
		}
		if (func != -1) {
			for (auto n : globeFunc[func].localVar) {
				if (n.name == tmp) {
					if (n.t == VT_ARRAY) {
						int offset;
						if (proc < content.size() &&
							content[proc].type == TT_OP && content[proc].id == OP_LPARENTHESIS) {
							varNode *ofvar = parseExpression();
							if (ofvar->t != VT_INTEGER)
								error("Array index", SE_INVALIDTYPE);
							if (content[proc].type != TT_OP || content[proc].id != OP_RPARENTHESIS)
								error("[]", SE_EXPBRACE);
							proc++;
							offset = *(int *)ofvar->val;

							char buf[16];
							_itoa(offset, buf, 10);
							return string("$") + buf + "@" + tmp;
						}
					}
					return "$" + tmp;
				}
			}
		}

		tmp += " ";
	}
	if(tmp.size())tmp.pop_back();

	return tmp;
}
int Syntax::findClass() {
	string tmp;
	int preProc = proc;
	while (proc < content.size() && content[proc].type == TT_USER) {
		tmp += strId[content[proc++].id];
		for (unsigned int i = 0; i < globeClassType.size(); i++) {
			if (globeClassType[i].name == tmp)return i;
		}
		tmp += " ";
	}
	if (tmp.size())tmp.pop_back();
	proc = preProc;
	return -1;
}
int Syntax::findFunc() {
	string tmp;
	int preProc = proc;
	while (proc < content.size() && content[proc].type == TT_USER) {
		tmp += strId[content[proc++].id];
		for (unsigned int i = 0; i < globeFunc.size(); i++) {
			if (globeFunc[i].declare.name == tmp)return i;
		}
		tmp += " ";
	}
	proc = preProc;
	return -1;
}
int Syntax::findType() {
	if (proc >= content.size())
		error("", SE_INCOMPLETE);
	if (content[proc].type == TT_SYS) {
		if (content[proc].id >= VT_INTEGER&&content[proc].id <= VT_ARRAY)
			return content[proc++].id;
	}
	return -1;
}

void Syntax::clearMem() {
	synMem.clear();
}

bool Syntax::compare(int op1, int op2) {//op1<=op2
	int prio1, prio2;
	switch (op1) {
	case OP_LBRACE:
		prio1 = 0;
		break;
	case OP_OROR:
		prio1 = 1;
		break;
	case OP_ANDAND:
		prio1 = 2;
		break;
	case OP_OR:
		prio1 = 3;
		break;
	case OP_NOR:
		prio1 = 4;
		break;
	case OP_AND:
		prio1 = 5;
		break;
	case OP_EQUAL:
	case OP_NOTEQ:
		prio1 = 6;
		break;
	case OP_GREATER:
	case OP_NGREATER:
	case OP_SMALLER:
	case OP_NSMALLER:
		prio1 = 7;
		break;
	case OP_PLUS:
	case OP_MINUS:
		prio1 = 8;
		break;
	case OP_MULTY:
	case OP_DIVIDE:
	case OP_MOD:
		prio1 = 9;
		break;
	case OP_NOT:
	case OP_INVERSE:
	case OP_NEG:
		prio1 = 10;
		break;
	}
	switch (op2) {
	case OP_LBRACE:
		prio2 = 0;
		break;
	case OP_OROR:
		prio2 = 1;
		break;
	case OP_ANDAND:
		prio2 = 2;
		break;
	case OP_OR:
		prio2 = 3;
		break;
	case OP_NOR:
		prio2 = 4;
		break;
	case OP_AND:
		prio2 = 5;
		break;
	case OP_EQUAL:
	case OP_NOTEQ:
		prio2 = 6;
		break;
	case OP_GREATER:
	case OP_NGREATER:
	case OP_SMALLER:
	case OP_NSMALLER:
		prio2 = 7;
		break;
	case OP_PLUS:
	case OP_MINUS:
		prio2 = 8;
		break;
	case OP_MULTY:
	case OP_DIVIDE:
	case OP_MOD:
		prio2 = 9;
		break;
	case OP_NOT:
	case OP_INVERSE:
	case OP_NEG:
		prio2 = 10;
		break;
	}
	return prio1 <= prio2;
}
void Syntax::error(const char *inst, int type)  {
	switch (type) {
	case SE_EXPOSE:
		throw new SyntaxException(std::string(inst) + "���鲻��ա�");
	case SE_UNIQUE:
		throw new SyntaxException(std::string(inst) + "�޴��÷���");
	case SE_EXPDOT:
		throw new SyntaxException(std::string(inst) + "ȱ�پ�š�");
	case SE_EXPCOMMA:
		throw new SyntaxException(std::string(inst) + "ȱ�ٶ��š�");
	case SE_EXPBRACE:
		throw new SyntaxException(std::string(inst) + "���Ų�������");
	case SE_REDEF:
		throw new SyntaxException(std::string(inst) + "�ض��塣");
	case SE_INVALIDTYPE:
		throw new SyntaxException(std::string(inst) + "�����������ʹ���");
	case SE_DISACCORD:
		throw new SyntaxException(std::string(inst) + "ǰ��һ�¡�");
	case SE_NOID:
		throw new SyntaxException(std::string(inst) + "δ���塣");
	case SE_INCOMPLETE:
		throw new SyntaxException(std::string(inst) + "��䲻������");
	case SE_UNKNOWN:
		throw new SyntaxException(std::string(inst) + "δ֪����");
	case SE_UNSUPPORT:
		throw new SyntaxException(std::string(inst) + "�ݲ�֧�֡�");
	}
}
