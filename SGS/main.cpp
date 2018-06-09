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
#include <llvm/Support/CommandLine.h>

#define SGS_INTERPRETER_OFF

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
    sgs_backend::recursiveDelete(res);
#endif
}

using namespace llvm;

cl::opt<string> OutputFilename("o", cl::desc("Generate executable file with llvm and clang facilities"), cl::value_desc("output filename"));
cl::opt<bool> GenerateIR("emit-ir", cl::desc("To generate LLVM IR"));
cl::opt<bool> GenerateDot("emit-dot", cl::desc("To generate dot file"));
cl::opt<bool> GenerateCpp("emit-cpp", cl::desc("To generate cpp file"));
cl::opt<bool> GeneratePng("emit-png", cl::desc("To generate png file by dot"));
cl::opt<bool> PrintAst("emit-ast", cl::desc("Print abstract syntax tree in console"));
cl::opt<bool> Execute("i", cl::desc("Just-in-time execute with lli"));
cl::opt<string> InputFilename(cl::Positional, cl::desc("<input file>"), cl::Required);

string getBanner() {
    string res;
    res += "  _____  _____  _____   \n";
    res += " / ____|/ ____|/ ____|  \n";
    res += "| (___ | |  __| (___    \n";
    res += " \\___ \\| | |_ |\\___ \\   Compiler v0.0.1 \n";
    res += " ____) | |__| |____) |  \n";
    res += "|_____/ \\_____|_____/  use -help to find more usage \n";
    return res;
}

void genExecutable(const string& input, const string& output) {
    string res1 = "llvm-as " + input + ".ll -o " + input + ".bc";
    system(res1.c_str());
    string res2 = "llc " + input + ".bc -o " + input + ".s";
    system(res2.c_str());
    string res3 = "clang " + input + ".s -o " + output;
    system(res3.c_str());
    string res4 = "del " + input + ".s"; // clean up
    system(res4.c_str());
    string res5 = "del " + input + ".bc";
    system(res5.c_str());
}

void generatePng(const string& filename) {
    string cmd = "dot -Tpng " + filename + ".gv -o " + filename + ".png";
    system(cmd.c_str());
}

void cleanUpDot(const string& filename) {
    string cmd = "del " + filename + ".gv";
    system(cmd.c_str());
}

void cleanUpIR(const string& filename) {
    string cmd = "del " + filename + ".ll";
    system(cmd.c_str());
}

void execute(const string& filename) {
    string cmd = "lli " + filename + ".ll";
    system(cmd.c_str());
}

string getNameWithoutExt(const string& filename) {
    string filenameWithoutExt;
    size_t i = 0;
    int flag = -1;
    for (; i < filename.length(); i++) {
        if (filename[i] == '.') flag = i;
    }
    if (flag == -1) filenameWithoutExt = filename;
    else filenameWithoutExt = filename.substr(0, flag);
    return filenameWithoutExt;
}

int main(int argc, char* argv[]) 
{
    cl::ParseCommandLineOptions(argc, argv);
    std::cout << getBanner() << std::endl;

    string inputFilename = InputFilename.getValue();
    string inputFilenameWithoutExt = getNameWithoutExt(inputFilename);

    std::ifstream fin("test7.sgs");
    
    if (!fin.good()) {
        std::cerr << "Cannot open file : " + inputFilename << std::endl;
        exit(1);
    }

    string input, tmp;
    while (!fin.eof()) {
        getline(fin, tmp);
        input += tmp + '\n';
    }
    fin.close();
    l.input(input.c_str())->parse();
    s.input(l.strId, l.output)->parse();

    sgs_backend::Context context;
    sgs_backend::Content content;
    try {
        content = transform(s.stmts, context);
    } catch (const std::exception& exc) {
        std::cerr << exc.what() << std::endl; // error
        exit(1); 
    }

    if (GenerateCpp) {
        std::cout << "Generated CPP file : " + inputFilenameWithoutExt + ".cpp" << std::endl;
        translateToCPP(s.stmts, inputFilenameWithoutExt + ".cpp");
    }

    if (PrintAst) {
        printAST(s.stmts);
    }

    if (GenerateDot) {
        std::cout << "Generated DOT file : " + inputFilenameWithoutExt + ".gv" << std::endl;
        sgs_backend::printContentInDot(content, inputFilenameWithoutExt + ".gv");
    }

    if (GeneratePng) { 
        std::cout << "Generated PNG file : " + inputFilenameWithoutExt + ".png" << std::endl;
        if (!GenerateDot) {
            sgs_backend::printContentInDot(content, inputFilenameWithoutExt + ".gv");
            generatePng(inputFilenameWithoutExt);
            cleanUpDot(inputFilenameWithoutExt);
        } else {
            generatePng(inputFilenameWithoutExt);
        }
    }

    sgs_backend::totalTranslation(content, inputFilenameWithoutExt + ".ll");

    if (Execute) {
        std::cout << "Executing : " + inputFilenameWithoutExt + ".ll" << std::endl;
        execute(inputFilenameWithoutExt);
    }

    if (!OutputFilename.empty()) {
        std::cout << "Output file : " + OutputFilename << std::endl;
        genExecutable(inputFilenameWithoutExt, OutputFilename);
    }

    if (!GenerateIR) {
        cleanUpIR(inputFilenameWithoutExt);
    } else if(!Execute) {
        std::cout << "Generated LLVM IR : " + inputFilenameWithoutExt + ".ll" << std::endl;
    }

    return 0;
}