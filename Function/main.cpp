#define SGS_DLL
#include <iostream>
#include "machine.h"

using namespace sgs;

extern "C" {
	__declspec(dllexport) VarNode *printAnInt(vector<VarNode *> param) {
		std::cout << ((IntNode *)param[0])->value;
		return NULL;
	}
	__declspec(dllexport) VarNode *printAFloat(vector<VarNode *> param) {
		std::cout << ((FloatNode *)param[0])->value;
		return NULL;
	}
	__declspec(dllexport) VarNode *printABool(vector<VarNode *> param) {
		std::cout << ((BoolNode *)param[0])->value;
		return NULL;
	}
	__declspec(dllexport) VarNode *printAStr(vector<VarNode *> param) {
		std::cout << ((StrNode *)param[0])->value;
		return NULL;
	}
}