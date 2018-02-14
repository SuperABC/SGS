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
void Machine::execute(stateSeq s, varNode *par) {
	stateSeq *iter = &s;
	vector<varNode> localVar;

	while (par != NULL) {
		varNode tmp = varNode(par->t, par->name);
		tmp.val = par->val;
		localVar.push_back(tmp);
	}
	while (iter) {
		step(iter);
		iter = iter->next;
	}
}
void Machine::step(stateSeq *s) {
	if (s->act.op == VO_ASSIGN) {
		for (unsigned int i = 0; i < globeVar.size(); i++) {
			if (globeVar[i].name == s->act.left->name) {
				/*if (s->act.left->t != s->act.right->t)
					error(s->act.left->name.c_str(), VE_TYPEMISMATCH);*/
				globeVar[i].val = exp(s->act.right).val;
				break;
			}
		}
	}
	else if (s->act.op == VO_EXE) {
		varNode *iter = s->act.right;
		while (iter != NULL) {
			if (iter->t == VT_VAR || iter->t == VT_EXP) {
				varNode r = exp(iter);
				iter->t = r.t;
				iter->val = r.val;
			}
			iter = iter->next;
		}
		if (s->act.left->name == "out") {
			out(s->act.right);
		}
		else if (s->act.left->name == "outln") {
			outln(s->act.right);
		}
		else {
			for (auto f : globeFunc) {
				if (f.declare.name == s->act.left->name) {
					execute(f.content, s->act.right);
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
		}
		if (ar) {
			execute(*s->act.right->left->block, NULL);
		}
		else {
			execute(*s->act.right->right->block, NULL);
		}
	}
}

varNode Machine::exp(varNode *e) {
	varNode ret;
	if (e->t == VT_FUNCTION) {

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
			break;
		case OP_MINUS:
			if (l.t == VT_FLOAT && r.t == VT_FLOAT) {
				ret.t = VT_FLOAT;
				ret.val = new float(*(float *)(r.val) - *(float *)(l.val));
			}
			else if (l.t == VT_FLOAT && r.t == VT_INTEGER) {
				ret.t = VT_FLOAT;
				ret.val = new float(*(float *)(r.val) - *(int *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_FLOAT) {
				ret.t = VT_FLOAT;
				ret.val = new float(*(int *)(r.val) - *(float *)(l.val));
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
				ret.val = new float(*(float *)(r.val) / *(int *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_FLOAT) {
				ret.t = VT_FLOAT;
				ret.val = new float(*(int *)(r.val) / *(float *)(l.val));
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
				ret.t = VT_FLOAT;
				ret.val = new int(*(float *)(r.val) == *(float *)(l.val));
			}
			else if (l.t == VT_FLOAT && r.t == VT_INTEGER) {
				ret.t = VT_FLOAT;
				ret.val = new int(*(float *)(r.val) == *(int *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_FLOAT) {
				ret.t = VT_FLOAT;
				ret.val = new int(*(int *)(r.val) == *(float *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_INTEGER) {
				ret.t = VT_INTEGER;
				ret.val = new int(*(int *)(r.val) == *(int *)(l.val));
			}
			break;
		case OP_GREATER:
			if (l.t == VT_FLOAT && r.t == VT_FLOAT) {
				ret.t = VT_FLOAT;
				ret.val = new int(*(float *)(r.val) > *(float *)(l.val));
			}
			else if (l.t == VT_FLOAT && r.t == VT_INTEGER) {
				ret.t = VT_FLOAT;
				ret.val = new int(*(float *)(r.val) > *(int *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_FLOAT) {
				ret.t = VT_FLOAT;
				ret.val = new int(*(int *)(r.val) > *(float *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_INTEGER) {
				ret.t = VT_INTEGER;
				ret.val = new int(*(int *)(r.val) > *(int *)(l.val));
			}
			break;
		case OP_SMALLER:
			if (l.t == VT_FLOAT && r.t == VT_FLOAT) {
				ret.t = VT_FLOAT;
				ret.val = new int(*(float *)(r.val) < *(float *)(l.val));
			}
			else if (l.t == VT_FLOAT && r.t == VT_INTEGER) {
				ret.t = VT_FLOAT;
				ret.val = new int(*(float *)(r.val) < *(int *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_FLOAT) {
				ret.t = VT_FLOAT;
				ret.val = new int(*(int *)(r.val) < *(float *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_INTEGER) {
				ret.t = VT_INTEGER;
				ret.val = new int(*(int *)(r.val) < *(int *)(l.val));
			}
			break;
		case OP_NSMALLER:
			if (l.t == VT_FLOAT && r.t == VT_FLOAT) {
				ret.t = VT_FLOAT;
				ret.val = new int(*(float *)(r.val) >= *(float *)(l.val));
			}
			else if (l.t == VT_FLOAT && r.t == VT_INTEGER) {
				ret.t = VT_FLOAT;
				ret.val = new int(*(float *)(r.val) >= *(int *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_FLOAT) {
				ret.t = VT_FLOAT;
				ret.val = new int(*(int *)(r.val) >= *(float *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_INTEGER) {
				ret.t = VT_INTEGER;
				ret.val = new int(*(int *)(r.val) >= *(int *)(l.val));
			}
			break;
		case OP_NGREATER:
			if (l.t == VT_FLOAT && r.t == VT_FLOAT) {
				ret.t = VT_FLOAT;
				ret.val = new int(*(float *)(r.val) <= *(float *)(l.val));
			}
			else if (l.t == VT_FLOAT && r.t == VT_INTEGER) {
				ret.t = VT_FLOAT;
				ret.val = new int(*(float *)(r.val) <= *(int *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_FLOAT) {
				ret.t = VT_FLOAT;
				ret.val = new int(*(int *)(r.val) <= *(float *)(l.val));
			}
			else if (l.t == VT_INTEGER && r.t == VT_INTEGER) {
				ret.t = VT_INTEGER;
				ret.val = new int(*(int *)(r.val) <= *(int *)(l.val));
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
	}
	else {
		ret.t = e->t;
		ret.val = e->val;
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
	}
}

void Machine::error(const char *inst, int type) {
	switch (type) {
	case VE_DIVBYZERO:
		throw new SyntaxException( "0不能做被除数。");
	case VE_TYPEMISMATCH:
		throw new SyntaxException(std::string(inst) + "类型不匹配。");
	}
}
