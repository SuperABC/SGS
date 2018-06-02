#define _CRT_SECURE_NO_WARNINGS
#include "machine.h"
#include "printAST.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#define SGS_INTERPRETER_OFF

using std::cin;
using std::cout;
using std::endl;
using std::string;
using namespace sgs;

SgsLex l = SgsLex();
SgsSyntax s = SgsSyntax();
SgsMachine m = SgsMachine();

int main() {
	std::ifstream fin(string("test.sgs"));

	string input, tmp;
	while (!fin.eof()) {
		getline(fin, tmp);
		input += tmp + '\n';
	}

	l.input(input.c_str())->parse();
	s.input(l.strId, l.output)->parse();
#ifndef SGS_INTERPRETER_OFF
	bool success = true;
	if (s.msgList.size()) {
		for (auto msg : s.msgList) {
			std::cout << msg.getMsg();
			if (msg.getLevel() == MT_ERROR)success = false;
		}
	}
	if (success) {
		m.input(s.stmts, s.classList, s.funcList)->execute();
		for (auto msg : m.msgList)std::cout << msg.getMsg();
	}
#else
	printAST(s.stmts);
#endif
	// system((char *)"pause");
    string temp; cin >> temp;
    return 0;
}