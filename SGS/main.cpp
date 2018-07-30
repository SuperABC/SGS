#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "machine.h"
#include "cppgen.h"

#define FILE_SGS
//#define INPUT_SGS

using std::string;
using sgs::Lexeme;
using sgs::Syntax;
using sgs::Machine;

static Lexeme l = Lexeme();
static Syntax s = Syntax();
static Machine m = Machine();

#ifdef FILE_SGS
void compile(const string& filename) {
    std::ifstream fin(filename + ".sgs");
    string input, tmp;
    while (!fin.eof()) {
        getline(fin, tmp);
        input += tmp + '\n';
    }
    fin.close();
    l.input(input.data())->parse();
    s.input(l.strId, l.output)->parse();

    bool success = true;
    if (l.msgList.size() + s.msgList.size()) {
		for (auto msg : l.msgList) {
			std::cout << msg.getMsg();
			if (msg.getLevel() == MT_ERROR)success = false;
		}
        for (auto msg : s.msgList) {
            std::cout << msg.getMsg();
            if (msg.getLevel() == MT_ERROR)success = false;
        }
    }
    if (success) {
        m.input(s.stmts, s.classList, s.funcList)->execute();
    }
}
#endif
#ifdef INPUT_SGS
void compile(const string& input) {
	l.input(input.data())->parse();
	s.input(l.strId, l.output)->parse();

	bool success = true;
	if (l.msgList.size() + s.msgList.size()) {
		for (auto msg : l.msgList) {
			std::cout << msg.getMsg();
			if (msg.getLevel() == MT_ERROR)success = false;
		}
		for (auto msg : s.msgList) {
			std::cout << msg.getMsg();
			if (msg.getLevel() == MT_ERROR)success = false;
		}
	}
	if (success) {
		m.input(s.stmts, s.classList, s.funcList)->execute();
	}
}
#endif

int main(int argc, char* argv[]) {
    string input;

#ifdef FILE_SGS
	compile("test");
#endif
#ifdef INPUT_SGS
	while (true) {
		getline(std::cin, input);
		compile(input);
	}
#endif

	system("pause");
    return 0;
}