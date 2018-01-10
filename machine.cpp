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
				if (s->act.left->t != s->act.right->t)
					error(s->act.left->name.c_str(), VE_TYPEMISMATCH);
				globeVar[i].val = s->act.right->val;
			}
		}
	}
	if (s->act.op == VO_EXE) {
		varNode *iter = s->act.right;
		while (iter != NULL) {
			if (iter->t == VT_VAR) {
				for (auto v : globeVar) {
					if ((char *)iter->val == v.name) {
						iter->t = v.t;
						iter->val = v.val;
						break;
					}
				}
			}
			iter = iter->next;
		}
		if (s->act.left->name == "print") {
			print(s->act.right);
		}
		else if (s->act.left->name == "println") {
			println(s->act.right);
		}
		else {
			for (auto f : globeFunc) {
				if (f.declare.name == s->act.left->name) {
					execute(f.content, s->act.right);
				}
			}
		}
	}
}

void Machine::print(varNode *par) {
	if (par->name == "content") {
		if(par->t==VT_STRING)
			printf("%s", par->val);
		if(par->t==VT_INTEGER)
			printf("%d", *(int *)(par->val));
		if (par->t == VT_FLOAT)
			printf("%f", *(float *)(par->val));
	}
}
void Machine::println(varNode *par) {
	if (par->name == "content") {
		if (par->t == VT_STRING)
			printf("%s\n", par->val);
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
