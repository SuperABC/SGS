#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include "machine.h"

//#define SGS_COMPILE

using std::cin;
using std::cout;
using std::endl;

SgsLex l = SgsLex();
SgsSyntax s = SgsSyntax();
SgsMachine m = SgsMachine();

void main() {
	std::ifstream fin("test.sgs");

	string input, tmp;
	while (!fin.eof()) {
		getline(fin, tmp);
		input += tmp + '\n';
	}

	l.input(input.c_str())->parse();
	s.input(l.strId, l.output)->parse();
#ifndef SGS_COMPILE
	m.input(s.stmts, s.classList, s.funcList)->execute();
#endif

	system("pause");
}