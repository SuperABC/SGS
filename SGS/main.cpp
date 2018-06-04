#define _CRT_SECURE_NO_WARNINGS
#include "machine.h"
#include "printAST.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "cppCodegen.h"
#include "TypeSys.h"
#include "transform.h"
#include "codegen.h"
// #include <llvm/Support/CommandLine.h>

#define SGS_INTERPRETER_OFF

using std::cin;
using std::cout;
using std::endl;
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
    translateToCPP(s.stmts, filename + ".cpp");
    sgs_backend::Context context;
    const auto res = transform(s.stmts, context);
    sgs_backend::printContentInDot(res, filename + ".gv");
    sgs_backend::totalTranslation(res, filename + ".ll");
#endif
}

int main() 
{
    compile("test1");
    return 0;
}