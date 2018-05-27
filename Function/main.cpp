#define SGS_DLL
#include <iostream>
#include "time.h"
#include "machine.h"

using namespace sgs;

extern "C" {
	__declspec(dllexport) VarNode *printAnInt(vector<VarNode *> param) {
		std::cout << ((IntNode *)param[0])->value << std::endl;
		return NULL;
	}
	__declspec(dllexport) VarNode *printANumber(vector<VarNode *> param) {
		std::cout << ((FloatNode *)param[0])->value << std::endl;
		return NULL;
	}
	__declspec(dllexport) VarNode *printABoolean(vector<VarNode *> param) {
		if(((BoolNode *)param[0])->value)std::cout << "true" << std::endl;
		else std::cout << "false" << std::endl;
		return NULL;
	}
	__declspec(dllexport) VarNode *printAStr(vector<VarNode *> param) {
		std::cout << ((StrNode *)param[0])->value << std::endl;
		return NULL;
	}
	__declspec(dllexport) VarNode *currentTime(vector<VarNode *> param) {
		return new IntNode(clock(), "");
	}
}