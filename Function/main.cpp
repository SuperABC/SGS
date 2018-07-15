#define SGS_DLL
#include <iostream>
#include "time.h"
#include "machine.h"

using namespace sgs;

extern "C" {

	__declspec(dllexport) VarNode *inputCh(vector<VarNode *> param) {
		return new IntNode(getchar(), "");
	}
	__declspec(dllexport) VarNode *inputInt(vector<VarNode *> param) {
		int res;
		std::cin >> res;
		return new IntNode(res, "");
	}
	__declspec(dllexport) VarNode *inputNumber(vector<VarNode *> param) {
		float res;
		std::cin >> res;
		return new FloatNode(res, "");
	}
	__declspec(dllexport) VarNode *inputStr(vector<VarNode *> param) {
		std::string res;
		std::cin >> res;
		return new StrNode(res.data(), "");
	}
    __declspec(dllexport) VarNode *printCh(vector<VarNode *> param) {
        std::cout << ((CharNode *)param[0])->value << std::endl;
        return NULL;
    }
	__declspec(dllexport) VarNode *printInt(vector<VarNode *> param) {
		std::cout << ((IntNode *)param[0])->value << std::endl;
		return NULL;
	}
    __declspec(dllexport) VarNode *printNumber(vector<VarNode *> param) {
        std::cout << ((FloatNode *)param[0])->value << std::endl;
        return NULL;
    }
    __declspec(dllexport) VarNode *printStr(vector<VarNode *> param) {
        std::cout << ((StrNode *)param[0])->value << std::endl;
        return NULL;
    }
	__declspec(dllexport) VarNode *newLine(vector<VarNode *> param) {
		std::cout << std::endl;
		return NULL;
	}

    __declspec(dllexport) VarNode *currentTime(vector<VarNode *> param) {
        return new IntNode(clock(), "");
    }
}