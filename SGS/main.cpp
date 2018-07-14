#define _CRT_SECURE_NO_WARNINGS
#include "machine.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "cppgen.h"

using std::string;

static SgsLex l = SgsLex();
static SgsSyntax s = SgsSyntax();
static SgsMachine m = SgsMachine();

void compile(const string& filename) {
    std::ifstream fin(filename + ".sgs");
    string input, tmp;
    while (!fin.eof()) {
        getline(fin, tmp);
        input += tmp + '\n';
    }
    fin.close();
    l.input(input.c_str())->parse();
    s.input(l.strId, l.output)->parse();

    bool success = true;
    if (s.msgList.size()) {
        for (auto msg : s.msgList) {
            std::cout << msg.getMsg();
            if (msg.getLevel() == MT_ERROR)success = false;
        }
    }
    if (success) {
        m.input(s.stmts, s.classList, s.funcList)->execute();
        //for (auto msg : m.msgList)std::cout << msg.getMsg();
    }
}

int main(int argc, char* argv[]) {
    string input, tmp;

	compile("test");
    return 0;
}