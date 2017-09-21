#ifndef MACHINE_H
#define MACHINE_H
#include "syntax.h"

class Machine {
private:
	stateSeq content;

	vector<varNode> globeVar;
	vector<classType> globeClassType;
	vector<classNode> globeClassNode;
	vector<funcNode> globeFunc;
public:
	Machine();
	~Machine();

	Machine *input(stateSeq seq,
		vector<classType> classType, vector<funcNode> func,
		vector<varNode> var);
	void execute();

	static void error(const char *inst, int type);
};

enum VMERROR {
	VE_DIVBYZERO
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