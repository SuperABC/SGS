#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include "syntax.h"

using std::cin;
using std::cout;
using std::endl;

SgsLex l = SgsLex();
SgsSyntax s = SgsSyntax();

void main() {
	std::ifstream fin("test.sgs");

	string input, tmp;
	while (!fin.eof()) {
		getline(fin, tmp);
		input += tmp + '\n';
	}

	l.input(input.c_str())->parse();
	s.input(l.strId, l.output)->parse();
}