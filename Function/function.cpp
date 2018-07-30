#define SGS_DLL
#include <iostream>
#include "time.h"
#include "machine.h"

using namespace sgs;

extern "C" {

	__declspec(dllexport) VarNode *inputCh(int n, VarNode *param[]) {
		return new IntNode(getchar(), "");
	}
	__declspec(dllexport) VarNode *inputInt(int n, VarNode *param[]) {
		int res;
		std::cin >> res;
		return new IntNode(res, "");
	}
	__declspec(dllexport) VarNode *inputNumber(int n, VarNode *param[]) {
		float res;
		std::cin >> res;
		return new FloatNode(res, "");
	}
	__declspec(dllexport) VarNode *inputStr(int n, VarNode *param[]) {
		std::string res;
		std::cin >> res;
		return new StrNode(res.data(), "");
	}
    __declspec(dllexport) VarNode *printCh(int n, VarNode *param[]) {
        std::cout << ((CharNode *)param[0])->value << std::endl;
        return NULL;
    }
	__declspec(dllexport) VarNode *printInt(int n, VarNode *param[]) {
		std::cout << ((IntNode *)param[0])->value << std::endl;
		return NULL;
	}
    __declspec(dllexport) VarNode *printNumber(int n, VarNode *param[]) {
        std::cout << ((FloatNode *)param[0])->value << std::endl;
        return NULL;
    }
    __declspec(dllexport) VarNode *printStr(int n, VarNode *param[]) {
        std::cout << ((StrNode *)param[0])->value << std::endl;
        return NULL;
    }
	__declspec(dllexport) VarNode *skipLine(int n, VarNode *param[]) {
		std::cout << std::endl;
		return NULL;
	}
    __declspec(dllexport) VarNode *currentTime(int n, VarNode *param[]) {
        return new IntNode(clock(), "");
    }
}
