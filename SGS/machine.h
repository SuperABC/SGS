#ifndef MACHINE_H
#define MACHINE_H
#include "syntax.h"

class SgsMachine {
private:
	vector<sgs::AST *> stmts;

	vector<sgs::ClassType *>classList;
	vector<sgs::FuncProto *>funcList;

	SgsMemory macMem;

	void *env;
	void initModule();
	void loadDlls();

public:
	SgsMachine();
	~SgsMachine();

	SgsMachine *input(vector<sgs::AST *> *s,
		vector<sgs::ClassType *> c, vector<sgs::FuncProto *> f);
	void execute();
	void step(sgs::AST *s);
	void environment(void *env);

	void clearMem();
	static void error(const char *inst, int type);
};

enum SGSVMERROR {
	VE_DIVBYZERO,
	VE_TYPEMISMATCH,
	VE_BROKEN
};
class SGSMachineException {
private:
	std::string msg;
public:
	SGSMachineException(std::string s) {
		msg = s;
	}
	const char *message() { return msg.data(); }
};
#endif