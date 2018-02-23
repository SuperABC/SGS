#define _CRT_SECURE_NO_WARNINGS
#include "machine.h"

Machine::Machine() {

}
Machine::~Machine() {
	
}

Machine *Machine::input(stateSeq *seq,
	vector<classType> classType, vector<funcNode> func,
	vector<varNode> var) {
	content = seq;
	globeClassType = classType;
	globeFunc = func;

	for (unsigned int i = globeVar.size(); i < var.size(); i++) {
		globeVar.push_back(var[i]);
	}
	return this;
}
void Machine::execute() {
	stateSeq *iter = content;
	while (iter) {
		step(iter);
		iter = iter->next;
	}
}
void Machine::execute(stateSeq *s, varNode *par, int funcid) {
	int preFunc = func;
	if (funcid != -1)func = funcid;
	stateSeq *iter = s;

	while (par != NULL) {
		for (auto &v : globeFunc[func].localVar) {
			if (v.name == par->name) {
				v.t = par->t;
				v.val = par->val;
			}
		}

		par = par->next;
	}
	while (iter) {
		step(iter);
		iter = iter->next;
	}

	func = preFunc;
}
void Machine::step(stateSeq *s) {
	if (s->act.op == VO_ASSIGN) {
		for (unsigned int i = 0; i < globeVar.size(); i++) {
			if (globeVar[i].name == s->act.left->name) {
				varNode tmp = exp(s->act.right);
				switch (s->act.left->t) {
				case VT_INTEGER:
					if (tmp.t == VT_INTEGER) {
						globeVar[i].val = tmp.val;
					}
					else if (tmp.t == VT_FLOAT) {
						globeVar[i].val = new int((int)*(float *)tmp.val);
					}
					else if (tmp.t == VT_BOOL) {
						globeVar[i].val = new int(*(bool *)tmp.val);
					}
					else if (tmp.t == VT_CHAR) {
						globeVar[i].val = new int(*(char *)tmp.val);
					}
					else
						error(s->act.left->name.c_str(), VE_TYPEMISMATCH);

					break;
				case VT_BOOL:
					if (tmp.t == VT_INTEGER) {
						globeVar[i].val = new bool(*(int *)tmp.val);
					}
					else if (tmp.t == VT_FLOAT) {
						globeVar[i].val = new bool(*(float *)tmp.val);
					}
					else if (tmp.t == VT_BOOL) {
						globeVar[i].val = tmp.val;
					}
					else if (tmp.t == VT_CHAR) {
						globeVar[i].val = new bool(*(char *)tmp.val);
					}
					else
						error(s->act.left->name.c_str(), VE_TYPEMISMATCH);

					break;
				case VT_CHAR:
					if (tmp.t == VT_INTEGER) {
						globeVar[i].val = new char(*(int *)tmp.val);
					}
					else if (tmp.t == VT_FLOAT) {
						globeVar[i].val = new char((char)*(float *)tmp.val);
					}
					else if (tmp.t == VT_BOOL) {
						globeVar[i].val = new char(*(bool *)tmp.val);
					}
					else if (tmp.t == VT_CHAR) {
						globeVar[i].val = tmp.val;
					}
					else
						error(s->act.left->name.c_str(), VE_TYPEMISMATCH);

					break;
				case VT_FLOAT:
					if (tmp.t == VT_INTEGER) {
						globeVar[i].val = new float((float)*(int *)tmp.val);
					}
					else if (tmp.t == VT_FLOAT) {
						globeVar[i].val = tmp.val;
					}
					else if (tmp.t == VT_BOOL) {
						globeVar[i].val = new float(*(bool *)tmp.val);
					}
					else if (tmp.t == VT_CHAR) {
						globeVar[i].val = new float(*(char *)tmp.val);
					}
					else
						error(s->act.left->name.c_str(), VE_TYPEMISMATCH);

					break;
				case VT_STRING:
					if (tmp.t == VT_STRING)
						globeVar[i].val = tmp.val;
					else
						error(s->act.left->name.c_str(), VE_TYPEMISMATCH);
					break;
				case VT_ARRAY:
					if (tmp.t == VT_ARRAY)
						globeVar[i].val = tmp.val;
					else
						error(s->act.left->name.c_str(), VE_TYPEMISMATCH);

					break;
				case VT_NULL:
					globeVar[i] = tmp;

					break;
				}

				break;
			}
		}
		if (func != -1) {
			for (unsigned int i = 0; i < globeFunc[func].localVar.size(); i++) {
				if (globeFunc[func].localVar[i].name == s->act.left->name) {
					varNode tmp = exp(s->act.right);
					switch (s->act.left->t) {
					case VT_INTEGER:
						if (tmp.t == VT_INTEGER) {
							globeFunc[func].localVar[i].val = tmp.val;
						}
						else if (tmp.t == VT_FLOAT) {
							globeFunc[func].localVar[i].val = new int((int)*(float *)tmp.val);
						}
						else if (tmp.t == VT_BOOL) {
							globeFunc[func].localVar[i].val = new int(*(bool *)tmp.val);
						}
						else if (tmp.t == VT_CHAR) {
							globeFunc[func].localVar[i].val = new int(*(char *)tmp.val);
						}
						else
							error(s->act.left->name.c_str(), VE_TYPEMISMATCH);

						break;
					case VT_BOOL:
						if (tmp.t == VT_INTEGER) {
							globeFunc[func].localVar[i].val = new bool(*(int *)tmp.val);
						}
						else if (tmp.t == VT_FLOAT) {
							globeFunc[func].localVar[i].val = new bool(*(float *)tmp.val);
						}
						else if (tmp.t == VT_BOOL) {
							globeFunc[func].localVar[i].val = tmp.val;
						}
						else if (tmp.t == VT_CHAR) {
							globeFunc[func].localVar[i].val = new bool(*(char *)tmp.val);
						}
						else
							error(s->act.left->name.c_str(), VE_TYPEMISMATCH);

						break;
					case VT_CHAR:
						if (tmp.t == VT_INTEGER) {
							globeFunc[func].localVar[i].val = new char(*(int *)tmp.val);
						}
						else if (tmp.t == VT_FLOAT) {
							globeFunc[func].localVar[i].val = new char((char)*(float *)tmp.val);
						}
						else if (tmp.t == VT_BOOL) {
							globeFunc[func].localVar[i].val = new char(*(bool *)tmp.val);
						}
						else if (tmp.t == VT_CHAR) {
							globeFunc[func].localVar[i].val = tmp.val;
						}
						else
							error(s->act.left->name.c_str(), VE_TYPEMISMATCH);

						break;
					case VT_FLOAT:
						if (tmp.t == VT_INTEGER) {
							globeFunc[func].localVar[i].val = new float((float)*(int *)tmp.val);
						}
						else if (tmp.t == VT_FLOAT) {
							globeFunc[func].localVar[i].val = tmp.val;
						}
						else if (tmp.t == VT_BOOL) {
							globeFunc[func].localVar[i].val = new float(*(bool *)tmp.val);
						}
						else if (tmp.t == VT_CHAR) {
							globeFunc[func].localVar[i].val = new float(*(char *)tmp.val);
						}
						else
							error(s->act.left->name.c_str(), VE_TYPEMISMATCH);

						break;
					case VT_STRING:
						if (tmp.t == VT_STRING)
							globeFunc[func].localVar[i].val = tmp.val;
						else
							error(s->act.left->name.c_str(), VE_TYPEMISMATCH);
						break;
					case VT_NULL:
						globeFunc[func].localVar[i].t = tmp.t;
						globeFunc[func].localVar[i].val = tmp.val;

						break;
					}

					break;
				}
			}
		}
	}
	else if (s->act.op == VO_EXE) {
		varNode *iter = s->act.right;
		varNode *par = new varNode();
		varNode *start = par;
		while (iter != NULL) {
			if (iter->t == VT_VAR || iter->t == VT_EXP || iter->t == VT_FUNCTION) {
				*par = exp(iter);
			}
			par->name = iter->name;
			iter = iter->next;
			par = par->next;
		}
		if (s->act.left->name == "out") {
			out(start);
		}
		else if (s->act.left->name == "outln") {
			outln(start);
		}
		else {
			for (unsigned int i = 0; i < globeFunc.size(); i++) {
				if (globeFunc[i].declare.name == s->act.left->name) {
					execute(&globeFunc[i].content, start, i);
				}
			}
		}
	}
	else if (s->act.op == VO_IF) {
		varNode res = exp(s->act.left);
		bool ar;
		switch (res.t) {
		case VT_INTEGER:
			if (*(int *)res.val)ar = true;
			else ar = false;
			break;
		case VT_BOOL:
			ar = *(bool *)res.val;
			break;
		}
		if (ar) {
			execute(s->act.right->left->block, NULL);
		}
		else {
			execute(s->act.right->right->block, NULL);
		}
	}
	else if (s->act.op == VO_WHILE) {
		while (true) {
			varNode cond = exp(s->act.left);

			bool loop;
			switch (cond.t) {
			case VT_INTEGER:
				if (*(int *)cond.val)loop = true;
				else loop = false;
				break;
			case VT_BOOL:
				loop = *(bool *)cond.val;
				break;
			}
			if (!loop)break;

			execute(s->act.right->block, NULL);
		}
	}
}

varNode Machine::exp(varNode *e) {
	varNode ret;
	if (e->t == VT_FUNCTION) {
		for (unsigned int i = 0; i < globeFunc.size(); i++) {
			if (globeFunc[i].declare.name == e->block->act.left->name) {
				execute(&globeFunc[i].content, e->block->act.right, i);
				ret = globeFunc[i].localVar[0];
			}
		}
	}
	else if (e->t == VT_EXP) {
		varNode l = exp(e->left);
		varNode r = exp(e->right);
		switch (*(int *)e->val) {
		case OP_PLUS:
			if (l.t == VT_FLOAT && r.t == VT_FLOAT) {
				ret.t = VT_FLOAT;
				ret.val = new float(*(float *)(l.val) + *(float *)(r.val));
			}
			else if (l.t == VT_FLOAT && r.t == VT_INTEGER) {
				ret.t = VT_FLOAT;
				ret.val = new float(*(float *)(l.val) + *(int *)(r.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_FLOAT) {
				ret.t = VT_FLOAT;
				ret.val = new float(*(int *)(l.val) + *(float *)(r.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_INTEGER) {
				ret.t = VT_INTEGER;
				ret.val = new int(*(int *)(l.val) + *(int *)(r.val));
			}
			else if (l.t == VT_STRING && r.t == VT_STRING) {
				ret.t = VT_STRING;
				ret.val = new char[strlen((char *)l.val) + strlen((char *)r.val)];
				strcpy((char *)ret.val, (char *)r.val);
				strcat((char *)ret.val, (char *)l.val);
			}
			break;
		case OP_MINUS:
			if (l.t == VT_FLOAT && r.t == VT_FLOAT) {
				ret.t = VT_FLOAT;
				ret.val = new float(*(float *)(r.val) - *(float *)(l.val));
			}
			else if (l.t == VT_FLOAT && r.t == VT_INTEGER) {
				ret.t = VT_FLOAT;
				ret.val = new float(*(int *)(r.val) - *(float *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_FLOAT) {
				ret.t = VT_FLOAT;
				ret.val = new float(*(float *)(r.val) - *(int *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_INTEGER) {
				ret.t = VT_INTEGER;
				ret.val = new int(*(int *)(r.val) - *(int *)(l.val));
			}
			break;
		case OP_MULTY:
			if (l.t == VT_FLOAT && r.t == VT_FLOAT) {
				ret.t = VT_FLOAT;
				ret.val = new float(*(float *)(l.val) * *(float *)(r.val));
			}
			else if (l.t == VT_FLOAT && r.t == VT_INTEGER) {
				ret.t = VT_FLOAT;
				ret.val = new float(*(float *)(l.val) * *(int *)(r.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_FLOAT) {
				ret.t = VT_FLOAT;
				ret.val = new float(*(int *)(l.val) * *(float *)(r.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_INTEGER) {
				ret.t = VT_INTEGER;
				ret.val = new int(*(int *)(l.val) * *(int *)(r.val));
			}
			break;
		case OP_DIVIDE:
			if (l.t == VT_FLOAT && r.t == VT_FLOAT) {
				ret.t = VT_FLOAT;
				ret.val = new float(*(float *)(r.val) / *(float *)(l.val));
			}
			else if (l.t == VT_FLOAT && r.t == VT_INTEGER) {
				ret.t = VT_FLOAT;
				ret.val = new float(*(int *)(r.val) / *(float *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_FLOAT) {
				ret.t = VT_FLOAT;
				ret.val = new float(*(float *)(r.val) / *(int *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_INTEGER) {
				ret.t = VT_INTEGER;
				ret.val = new int(*(int *)(r.val) / *(int *)(l.val));
			}
			break;
		case OP_MOD:
			if (l.t == VT_INTEGER && r.t == VT_INTEGER) {
				ret.t = VT_INTEGER;
				ret.val = new int(*(int *)(r.val) % *(int *)(l.val));
			}
			else error("mod", VE_TYPEMISMATCH);
			break;
		case OP_EQUAL:
			if (l.t == VT_FLOAT && r.t == VT_FLOAT) {
				ret.t = VT_BOOL;
				ret.val = new bool(*(float *)(r.val) == *(float *)(l.val));
			}
			else if (l.t == VT_FLOAT && r.t == VT_INTEGER) {
				ret.t = VT_BOOL;
				ret.val = new bool(*(int *)(r.val) == *(float *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_FLOAT) {
				ret.t = VT_BOOL;
				ret.val = new bool(*(float *)(r.val) == *(int *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_INTEGER) {
				ret.t = VT_BOOL;
				ret.val = new bool(*(int *)(r.val) == *(int *)(l.val));
			}
			break;
		case OP_GREATER:
			if (l.t == VT_FLOAT && r.t == VT_FLOAT) {
				ret.t = VT_BOOL;
				ret.val = new bool(*(float *)(r.val) > *(float *)(l.val));
			}
			else if (l.t == VT_FLOAT && r.t == VT_INTEGER) {
				ret.t = VT_BOOL;
				ret.val = new bool(*(int *)(r.val) > *(float *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_FLOAT) {
				ret.t = VT_BOOL;
				ret.val = new bool(*(float *)(r.val) > *(int *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_INTEGER) {
				ret.t = VT_BOOL;
				ret.val = new bool(*(int *)(r.val) > *(int *)(l.val));
			}
			break;
		case OP_SMALLER:
			if (l.t == VT_FLOAT && r.t == VT_FLOAT) {
				ret.t = VT_BOOL;
				ret.val = new bool(*(float *)(r.val) < *(float *)(l.val));
			}
			else if (l.t == VT_FLOAT && r.t == VT_INTEGER) {
				ret.t = VT_BOOL;
				ret.val = new bool(*(int *)(r.val) < *(float *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_FLOAT) {
				ret.t = VT_BOOL;
				ret.val = new bool(*(float *)(r.val) < *(int *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_INTEGER) {
				ret.t = VT_BOOL;
				ret.val = new bool(*(int *)(r.val) < *(int *)(l.val));
			}
			break;
		case OP_NSMALLER:
			if (l.t == VT_FLOAT && r.t == VT_FLOAT) {
				ret.t = VT_BOOL;
				ret.val = new bool(*(float *)(r.val) >= *(float *)(l.val));
			}
			else if (l.t == VT_FLOAT && r.t == VT_INTEGER) {
				ret.t = VT_BOOL;
				ret.val = new bool(*(int *)(r.val) >= *(float *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_FLOAT) {
				ret.t = VT_BOOL;
				ret.val = new bool(*(float *)(r.val) >= *(int *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_INTEGER) {
				ret.t = VT_BOOL;
				ret.val = new bool(*(int *)(r.val) >= *(int *)(l.val));
			}
			break;
		case OP_NGREATER:
			if (l.t == VT_FLOAT && r.t == VT_FLOAT) {
				ret.t = VT_BOOL;
				ret.val = new bool(*(float *)(r.val) <= *(float *)(l.val));
			}
			else if (l.t == VT_FLOAT && r.t == VT_INTEGER) {
				ret.t = VT_BOOL;
				ret.val = new bool(*(int *)(r.val) <= *(float *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_FLOAT) {
				ret.t = VT_BOOL;
				ret.val = new bool(*(float *)(r.val) <= *(int *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_INTEGER) {
				ret.t = VT_BOOL;
				ret.val = new bool(*(int *)(r.val) <= *(int *)(l.val));
			}
			break;
		}
	}
	else if (e->t == VT_VAR) {
		for (auto v : globeVar) {
			if ((char *)e->val == v.name) {
				ret.t = v.t;
				ret.val = v.val;
				break;
			}
		}
		if (func != -1) {
			for (auto v : globeFunc[func].localVar) {
				if ((char *)e->val == v.name) {
					ret.t = v.t;
					ret.val = v.val;
					break;
				}
			}
		}
	}
	else {
		ret.t = e->t;
		ret.val = e->val;
		ret.next = e->next;
	}

	return ret;
}
void Machine::out(varNode *par) {
	if (par->name == "content") {
		if(par->t==VT_STRING)
			printf("%s", (char *)par->val);
		if(par->t==VT_INTEGER)
			printf("%d", *(int *)(par->val));
		if (par->t == VT_FLOAT)
			printf("%f", *(float *)(par->val));
		if (par->t == VT_ARRAY) {
			varNode *iter = (varNode *)par->val;
			while (iter) {
				switch (iter->t) {
				case VT_STRING:
					printf("%s ", (char *)iter->val);
					break;
				case VT_INTEGER:
					printf("%d ", *(int *)(iter->val));
					break;
				case VT_FLOAT:
					printf("%f ", *(float *)(iter->val));
					break;
				}
				iter = iter->next;
			}
		}
	}
}
void Machine::outln(varNode *par) {
	if (par->name == "content") {
		if (par->t == VT_STRING)
			printf("%s\n", (char *)par->val);
		if (par->t == VT_INTEGER)
			printf("%d\n", *(int *)(par->val));
		if (par->t == VT_FLOAT)
			printf("%f\n", *(float *)(par->val));
		if (par->t == VT_ARRAY) {
			varNode *iter = (varNode *)par->val;
			while (iter) {
				switch (iter->t) {
				case VT_STRING:
					printf("%s ", (char *)iter->val);
					break;
				case VT_INTEGER:
					printf("%d ", *(int *)(iter->val));
					break;
				case VT_FLOAT:
					printf("%f ", *(float *)(iter->val));
					break;
				}
				iter = iter->next;
			}
			printf("\n");
		}
	}
}

void Machine::error(const char *inst, int type) {
	switch (type) {
	case VE_DIVBYZERO:
		throw new SyntaxException( "0��������������");
	case VE_TYPEMISMATCH:
		throw new SyntaxException(std::string(inst) + "���Ͳ�ƥ�䡣");
	}
}
