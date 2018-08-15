#include "generator.h"

using namespace sgs;

string removeSpace(const string& input) {
	string output = input;
	for (auto& x : output) {
		if (x == ' ') x = '_';
	}
	return output;
}
void translateBasicType(sgs::VarType *stmtVar, std::ofstream &fout) {
	auto* basicVar = dynamic_cast<sgs::BasicType *>(stmtVar);
	switch (basicVar->getBasicType()) {
	case BASIC_TYPE::BT_INT:    fout << cppTab(cppDepth) << "int "; break;
	case BASIC_TYPE::BT_FLOAT:  fout << cppTab(cppDepth) << "float "; break;
	case BASIC_TYPE::BT_BOOL:   fout << cppTab(cppDepth) << "bool "; break;
	case BASIC_TYPE::BT_CHAR:   fout << cppTab(cppDepth) << "char "; break;
	case BASIC_TYPE::BT_STRING: fout << cppTab(cppDepth) << "string "; break;
	default: break;
	}
}