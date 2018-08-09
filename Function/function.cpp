#define _CRT_SECURE_NO_WARNINGS
#define SGS_DLL
#include <iostream>
#include <string>
#include <cmath>
#include "time.h"
#include "machine.h"

using namespace sgs;

extern "C" {

	//IO
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
	__declspec(dllexport) VarNode *inputCh(int n, VarNode *param[]) {
		return new IntNode(getchar(), "");
	}
	__declspec(dllexport) VarNode *inputStr(int n, VarNode *param[]) {
		std::string res;
		std::cin >> res;
		return new StrNode(res.data(), "");
	}
	__declspec(dllexport) VarNode *inputLine(int n, VarNode *param[]) {
		std::string res;
		getline(std::cin, res);
		return new StrNode(res.data(), "");
	}

    __declspec(dllexport) VarNode *printCh(int n, VarNode *param[]) {
		switch (((BasicType *)param[0]->type)->getBasicType()) {
		case BT_INT:
			std::cout << (char)((IntNode *)param[0])->value << std::endl;
			break;
		case BT_FLOAT:
			std::cout << (char)((FloatNode *)param[0])->value << std::endl;
			break;
		case BT_CHAR:
			std::cout << (char)((CharNode *)param[0])->value << std::endl;
			break;
		}
        return NULL;
    }
	__declspec(dllexport) VarNode *printInt(int n, VarNode *param[]) {
		switch (((BasicType *)param[0]->type)->getBasicType()) {
		case BT_INT:
			std::cout << (int)((IntNode *)param[0])->value << std::endl;
			break;
		case BT_FLOAT:
			std::cout << (int)((FloatNode *)param[0])->value << std::endl;
			break;
		case BT_CHAR:
			std::cout << (int)((CharNode *)param[0])->value << std::endl;
			break;
		}
		return NULL;
	}
    __declspec(dllexport) VarNode *printNumber(int n, VarNode *param[]) {
		switch (((BasicType *)param[0]->type)->getBasicType()) {
		case BT_INT:
			std::cout << (float)((IntNode *)param[0])->value << std::endl;
			break;
		case BT_FLOAT:
			std::cout << (float)((FloatNode *)param[0])->value << std::endl;
			break;
		case BT_CHAR:
			std::cout << (float)((CharNode *)param[0])->value << std::endl;
			break;
		}
        return NULL;
    }
    __declspec(dllexport) VarNode *printStr(int n, VarNode *param[]) {
		switch (((BasicType *)param[0]->type)->getBasicType()) {
		case BT_INT:
			std::cout << ((IntNode *)param[0])->value << std::endl;
			break;
		case BT_FLOAT:
			std::cout << ((FloatNode *)param[0])->value << std::endl;
			break;
		case BT_CHAR:
			std::cout << ((CharNode *)param[0])->value << std::endl;
			break;
		case BT_BOOL:
			std::cout << ((BoolNode *)param[0])->value << std::endl;
			break;
		case BT_STRING:
			std::cout << ((StrNode *)param[0])->value << std::endl;
			break;
		}
        return NULL;
    }

	__declspec(dllexport) VarNode *skipLine(int n, VarNode *param[]) {
		std::cout << std::endl;
		return NULL;
	}

	__declspec(dllexport) VarNode *openFile(int n, VarNode *param[]) {
		int fp;
		fp = (int)fopen(((StrNode *)param[0])->value, "w+");
		return new IntNode(fp, "");
	}


	//Time
    __declspec(dllexport) VarNode *currentTime(int n, VarNode *param[]) {
        return new IntNode(clock(), "");
    }
	__declspec(dllexport) VarNode *randomInt(int n, VarNode *param[]) {
		return new IntNode(rand(), "");
	}

	//Math
	__declspec(dllexport) VarNode *absent(int n, VarNode *param[]) {
		switch (((BasicType *)param[0]->type)->getBasicType()) {
		case BT_INT:
			return new FloatNode((float)fabs(((IntNode *)param[0])->value), "");
		case BT_FLOAT:
			return new FloatNode((float)fabs(((FloatNode *)param[0])->value), "");
		case BT_CHAR:
			return new FloatNode((float)fabs(((CharNode *)param[0])->value), "");
		}
		return NULL;
	}
	__declspec(dllexport) VarNode *power(int n, VarNode *param[]) {
		switch (((BasicType *)param[0]->type)->getBasicType()) {
		case BT_INT:
			return new FloatNode((float)pow(((IntNode *)param[0])->value, 2), "");
		case BT_FLOAT:
			return new FloatNode((float)pow(((FloatNode *)param[0])->value, 2), "");
		case BT_CHAR:
			return new FloatNode((float)pow(((CharNode *)param[0])->value, 2), "");
		}
		return NULL;
	}
	__declspec(dllexport) VarNode *logar(int n, VarNode *param[]) {
		switch (((BasicType *)param[0]->type)->getBasicType()) {
		case BT_INT:
			return new FloatNode((float)log2(((IntNode *)param[0])->value), "");
		case BT_FLOAT:
			return new FloatNode((float)log2(((FloatNode *)param[0])->value), "");
		case BT_CHAR:
			return new FloatNode((float)log2(((CharNode *)param[0])->value), "");
		}
		return NULL;
	}
}
