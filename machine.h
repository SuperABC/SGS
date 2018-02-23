#ifndef MACHINE_H
#define MACHINE_H
#include "syntax.h"

class Machine {
private:
	stateSeq *content;

	vector<varNode> globeVar;
	vector<classType> globeClassType;
	vector<classNode> globeClassNode;
	vector<funcNode> globeFunc;

	int func = -1;
public:
	Machine();
	~Machine();

	Machine *input(stateSeq *seq,
		vector<classType> classType, vector<funcNode> func,
		vector<varNode> var);
	void execute();
	void execute(stateSeq *s, varNode *par, int funcid = -1);
	void step(stateSeq *s);

	varNode exp(varNode *e);
	void out(varNode *par);
	void outln(varNode *par);

	static void error(const char *inst, int type);
};

enum VMERROR {
	VE_DIVBYZERO,
	VE_TYPEMISMATCH
};
class MachineException {
private:
	std::string msg;
public:
	MachineException(std::string s) {
		msg = s;
	}
	const char *message() { return msg.c_str(); }
};
#endif