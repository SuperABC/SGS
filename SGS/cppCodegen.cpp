#include "cppCodegen.h"

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
    default:
        break;
    }
}

VAR_TYPE translateArrayType(sgs::VarType *stmtVar, std::ofstream &fout) {
    auto* arrayVar = dynamic_cast<sgs::ArrayType *>(stmtVar);
    switch (arrayVar->getEleType()->getVarType()) {
    case VAR_TYPE::VT_BASIC:
    {
        translateBasicType(arrayVar->getEleType(), fout);
        break;
    }
    case VAR_TYPE::VT_ARRAY:
    {
        translateBasicType((sgs::ArrayType *)(arrayVar->getEleType())->getVarType(), fout);
        return VAR_TYPE::VT_ARRAY;
    }
    case VAR_TYPE::VT_CLASS:
    {
        return VAR_TYPE::VT_CLASS;
    }
    default:break;
    }
    return VAR_TYPE::VT_BASIC;
}
void translateClassType(sgs::VarType *stmtVar, std::ofstream &fout) {
    auto* classVar = dynamic_cast<sgs::ClassType *>(stmtVar);
    fout << "class " << classVar->getName() << "{" << std::endl;
    cppDepth++;
    fout << "public:" << std::endl;
    int count = classVar->getEle().size();
    vector <std::pair<VarType *, string>> classElements = classVar->getEle();
    for (int i = 0; i < count; ++i) {
        sgs::VarType *currentMember = classElements[i].first;
        switch (currentMember->getVarType()) {
        case VAR_TYPE::VT_BASIC: translateBasicType(currentMember, fout); break;
        case VAR_TYPE::VT_ARRAY:
        {
            VAR_TYPE type = translateArrayType(currentMember, fout);
            string currentName = classElements[i].second;
            if (VAR_TYPE::VT_BASIC == type) {
                fout << currentName << "[]";
            } else if (VAR_TYPE::VT_ARRAY == type) {
                fout << currentName << "[][]";
            } else if (VAR_TYPE::VT_CLASS == type) {
                fout << cppTab(cppDepth) << dynamic_cast<sgs::ClassType *>(currentMember)->getName() << ' ';
                fout << currentName << "[]";
            }
            break;
        }
        case VAR_TYPE::VT_CLASS: translateClassType(currentMember, fout); break;
        default:break;
        }
        fout << classElements[i].second << ";" << std::endl;
    }
    fout << "};" << std::endl;
    cppDepth--;
    return;
}
void translateOpSwitchCase(SGSOPERATOR OP, std::ofstream &fout) {
    switch (OP) {
    case SGS_OP_PLUS:
        fout << " + ";
        break;
    case SGS_OP_PLUSPLUS:
        fout << "++";
        break;
    case SGS_OP_EQPLUS:
        fout << " += ";
        break;
    case SGS_OP_MINUS:
        fout << " - ";
        break;
    case SGS_OP_MINUSMINUS:
        fout << "--";
        break;
    case SGS_OP_EQMINUS:
        fout << " -= ";
        break;
    case SGS_OP_NEG:
        fout << "-";
        break;
    case SGS_OP_MULTY:
        fout << " * ";
        break;
    case SGS_OP_EQMULTY:
        fout << " *= ";
        break;
    case SGS_OP_DIVIDE:
        fout << " / ";
        break;
    case SGS_OP_EQDIVIDE:
        fout << " /= ";
        break;
    case SGS_OP_MOD:
        fout << " % ";
        break;
    case SGS_OP_EQMOD:
        fout << " %= ";
        break;
    case SGS_OP_AND:
        fout << " & ";
        break;
    case SGS_OP_ANDAND:
        fout << " && ";
        break;
    case SGS_OP_EQAND:
        fout << " &= ";
        break;
    case SGS_OP_OR:
        fout << " | ";
        break;
    case SGS_OP_OROR:
        fout << " || ";
        break;
    case SGS_OP_EQOR:
        fout << " |= ";
        break;
    case SGS_OP_NOR:
        fout << " ^ ";
        break;
    case SGS_OP_EQNOR:
        fout << " ^= ";
        break;
    case SGS_OP_INVERSE:
        fout << "~";
        break;
    case SGS_OP_EQINVERSE:
        fout << " ~= ";
        break;
    case SGS_OP_LBRACE:
        fout << "{";
        break;
    case SGS_OP_RBRACE:
        fout << "}";
        break;
    case SGS_OP_LPARENTHESIS:
        fout << "(";
        break;
    case SGS_OP_RPARENTHESIS:
        fout << ")";
        break;
    case SGS_OP_LBRAKET:
        fout << "[";
        break;
    case SGS_OP_RBRAKET:
        fout << "]";
        break;
    case SGS_OP_SEMI:
        fout << "SEMI";
        break;
    case SGS_OP_COMMA:
        fout << ",";
        break;
    case SGS_OP_DOT:
        fout << ".";
        break;
    case SGS_OP_SMALLER:
        fout << " < ";
        break;
    case SGS_OP_NSMALLER:
        fout << " <= ";
        break;
    case SGS_OP_GREATER:
        fout << " > ";
        break;
    case SGS_OP_NGREATER:
        fout << " >= ";
        break;
    case SGS_OP_NOT:
        fout << " !";
        break;
    case SGS_OP_NOTEQ:
        fout << " != ";
        break;
    case SGS_OP_EQUAL:
        fout << " == ";
        break;
    case SGS_OP_QUERY:
        fout << "QUERY";
        break;
    case SGS_OP_QUOT:
        fout << "\'";
        break;
    case SGS_OP_DBQUOT:
        fout << "\"";
        break;
    case SGS_OP_CROSS:
        fout << "CROSS";
        break;
    default:
        break;
    }
}
void translateOpExp(sgs::Expression *stmtExp, std::ofstream &fout) {
    sgs::OpExp *opExp = dynamic_cast<sgs::OpExp *>(stmtExp);
    fout << "(";
    translateExpType(opExp->getLeft(), fout);
    translateOpSwitchCase(opExp->getOp(), fout);
    translateExpType(opExp->getRight(), fout);
    fout << ")";
}
void translateLiteralExp(sgs::Expression *stmtExp, std::ofstream &fout) {
    sgs::LiteralExp *literalExp = dynamic_cast<sgs::LiteralExp *>(stmtExp);
    switch (literalExp->getType()->getVarType()) {
    case VAR_TYPE::VT_BASIC:
    {
        auto*basicLiteralExp = dynamic_cast<sgs::BasicType *>(literalExp->getType());
        switch (basicLiteralExp->getBasicType()) {
        case BASIC_TYPE::BT_INT:
        {
            auto*intLiteralExp = dynamic_cast<sgs::IntLiteral *>(literalExp);
            fout << intLiteralExp->getValue();
            break;
        }
        case BASIC_TYPE::BT_FLOAT:
        {
            sgs::FloatLiteral *floatLiteralExp = dynamic_cast<sgs::FloatLiteral *>(literalExp);
            fout << floatLiteralExp->getValue();
            break;
        }
        case BASIC_TYPE::BT_BOOL:
        {
            auto* boolLiteralExp = dynamic_cast<sgs::BoolLiteral *>(literalExp);
            if (boolLiteralExp->getValue())
                fout << "true";
            else
                fout << "false";
            break;
        }
        case BASIC_TYPE::BT_STRING:
        {
            sgs::StrLiteral *strLiteralExp = dynamic_cast<sgs::StrLiteral *>(literalExp);
            fout << "\"" << strLiteralExp->getValue() << "\"";
            break;
        }
        default:
            break;
        }
        break;
    }
    case VAR_TYPE::VT_ARRAY:
    {
        /*sgs::ArrayLiteral *arrayLiteralExp = (sgs::ArrayLiteral *)literalExp;
        std::cout << ASTTab(depth) << "Array value:" << std::endl;
        depth++;
        vector<Expression *>arrayConent = arrayLiteralExp->getValue();
        int count = arrayConent.size();
        for (int i = 0; i < count; ++i) {
        std::cout << ASTTab(depth) << "No. " << i << " content" << std::endl;
        depth++; dealWithExpType(arrayConent[i]); depth--;
        }
        depth--;
        break;*/
    }
    case VAR_TYPE::VT_CLASS:
    {
        sgs::ClassLiteral *classLiteralExp = dynamic_cast<sgs::ClassLiteral *>(literalExp);
        vector<Expression *>classConent = classLiteralExp->getValue();
        int count = classConent.size();
        fout << "(";
        for (int i = 0; i < count; ++i) {
            translateExpType(classConent[i], fout);
            if (i != count - 1)
                fout << ", ";
            else
                fout << ")";
        }
        break;
    }
    default:
        break;
    }
}
void translateIdExp(sgs::Expression *stmtExp, std::ofstream &fout) {
    sgs::IdExp *idExp = dynamic_cast<sgs::IdExp *>(stmtExp);
    fout << removeSpace(idExp->getName());
    return;
}
void translateVisitExp(sgs::Expression *stmtExp, std::ofstream &fout) {
    auto* visitExp = dynamic_cast<sgs::VisitExp *>(stmtExp);
    translateExpType(visitExp->getArray(), fout);
    fout << "[";
    translateExpType(visitExp->getIndex(), fout);
    fout << "]";
    return;
}
void translateCallExp(sgs::Expression *stmtExp, std::ofstream &fout) {
    auto* callExp = dynamic_cast<sgs::CallExp *>(stmtExp);
    fout << removeSpace(callExp->getFunction()->getName()) << "(";
    int count = callExp->getParam().size();
    vector <Expression *> paramList = callExp->getParam();
    if (count == 0) {
        fout << ")";
        return;
    }
    for (int i = 0; i < count; ++i) {
        sgs::Expression *currentParameter = paramList[i];
        translateExpType(currentParameter, fout);
        if (i == count - 1) {
            fout << ")";
        } else {
            fout << ", ";
        }
    }
}
void translateAccessExp(sgs::Expression *stmtExp, std::ofstream &fout) {
    auto* accessExp = dynamic_cast<sgs::AccessExp *>(stmtExp);
    translateExpType(accessExp->getObject(), fout);
    fout << ".";
    fout << removeSpace(accessExp->getMember());
}
void translateAssignStmt(sgs::Statement *stmtStmt, std::ofstream &fout) {
    auto*AssignStmt = dynamic_cast<sgs::AssignStmt *>(stmtStmt);
    fout << cppTab(cppDepth);
    translateExpType(AssignStmt->getLeft(), fout);
    fout << " = ";
    translateExpType(AssignStmt->getRight(), fout);
    fout << ";" << std::endl;
}
void translateCallStmt(sgs::Statement *stmtStmt, std::ofstream &fout) {
    sgs::CallStmt *callStmt = dynamic_cast<sgs::CallStmt *>(stmtStmt);
    fout << cppTab(cppDepth) << removeSpace(callStmt->getFunction()->getName()) << "(";
    int count = callStmt->getParam().size();
    vector <Expression *> paramList = callStmt->getParam();
    for (int i = 0; i < count; ++i) {
        sgs::Expression *currentParameter = paramList[i];
        translateExpType(currentParameter, fout);
        if (i == count - 1)
            fout << ");" << std::endl;
        else
            fout << ", ";
    }
}
void translateBlockStmt(sgs::Statement *stmtStmt, std::ofstream &fout) {
    sgs::BlockStmt *blockStmt = dynamic_cast<sgs::BlockStmt *>(stmtStmt);
    translateAST(blockStmt->getContent(), fout);
}
void translateIfStmt(sgs::Statement *stmtStmt, std::ofstream &fout) {
    sgs::IfStmt *IfStmt = dynamic_cast<sgs::IfStmt *>(stmtStmt);
    fout << cppTab(cppDepth) << "if(";
    translateExpType(IfStmt->getCond(), fout);
    fout << "){" << std::endl;
    cppDepth++;
    translateBlockStmt(IfStmt->getTaken(), fout);
    cppDepth--;
    fout << cppTab(cppDepth) << "}" << std::endl;

    fout << cppTab(cppDepth) << "else{" << std::endl;
    cppDepth++;
    translateBlockStmt(IfStmt->getUntaken(), fout);
    cppDepth--;
    fout << cppTab(cppDepth) << "}" << std::endl;

}
void translateWhileStmt(sgs::Statement *stmtStmt, std::ofstream &fout) {
    auto*whileStmt = dynamic_cast<sgs::WhileStmt *>(stmtStmt);
    fout << cppTab(cppDepth) << "while(";
    translateExpType(whileStmt->getCondition(), fout);
    fout << "){" << std::endl;
    cppDepth++;
    translateBlockStmt(whileStmt->getBody(), fout);
    cppDepth--;
    fout << cppTab(cppDepth) << "}" << std::endl;
}
void translateVarType(sgs::AST *s, enum conditionUseVarType choice, std::ofstream &fout) {
    switch (choice) {
    case conditionUseVarType::VARDEF: //VARDEF
    {
        auto* currentStmt = dynamic_cast<sgs::VarDef *>(s);
        string currentName = removeSpace(currentStmt->getName());
        switch (currentStmt->getDecType()->getVarType()) {
        case VAR_TYPE::VT_BASIC:
        {
            translateBasicType(currentStmt->getDecType(), fout);
            fout << currentName;
            break;
        }
        case VAR_TYPE::VT_ARRAY:
        {
            VAR_TYPE type = translateArrayType(currentStmt->getDecType(), fout);
            if (VAR_TYPE::VT_BASIC == type) {
                fout << currentName << "[" << dynamic_cast<sgs::ArrayType *>(currentStmt->getDecType())->getLength() << "]";
            } else if (VAR_TYPE::VT_ARRAY == type) {
                fout << currentName << "[][]";
            } else if (VAR_TYPE::VT_CLASS == type) {
                fout << dynamic_cast<sgs::ClassType *>(currentStmt->getDecType())->getName() << ' ';
                fout << currentName << "[" << dynamic_cast<sgs::ArrayType *>(currentStmt->getDecType())->getLength() << "]";
            }
            break;
        }
        case VAR_TYPE::VT_CLASS:
        {
            fout << cppTab(cppDepth) << removeSpace(dynamic_cast<sgs::ClassType *>(currentStmt->getDecType())->getName()) << ' ' << currentName;
            break;
        }
        default:break;
        }
        fout << ";" << std::endl;
        break;
    }
    case conditionUseVarType::CLASS:
    {
        auto*currentStmt = dynamic_cast<sgs::ClassDef *>(s);
        translateClassType(currentStmt->getDecType(), fout);
        break;
    }
    case conditionUseVarType::EXP:
    {
        auto*currentStmt = dynamic_cast<sgs::Expression *>(s);
        // switch (currentStmt->getResType()->getVarType()) {
        // case VAR_TYPE::VT_BASIC: printBasicType(currentStmt->getResType()); break;
        // case VAR_TYPE::VT_ARRAY: printArrayType(currentStmt->getResType()); break;
        // case VAR_TYPE::VT_CLASS: printClassType(currentStmt->getResType()); break;
        // default:break;
        // }
        break;
    }
    case conditionUseVarType::FUNC:
    {
        auto* currentStmt = dynamic_cast<sgs::FuncDef *>(s);
        if (nullptr == currentStmt->getProto()->getReturnType()) {
            fout << "void ";
            break;
        }
        switch (currentStmt->getProto()->getReturnType()->getVarType()) {
        case VAR_TYPE::VT_BASIC: translateBasicType(currentStmt->getProto()->getReturnType(), fout); break;
        case VAR_TYPE::VT_ARRAY: translateArrayType(currentStmt->getProto()->getReturnType(), fout); break;
        case VAR_TYPE::VT_CLASS: fout << (dynamic_cast<sgs::ClassDef *>(currentStmt)->getDecType())->getName(); break;
        default:break;
        }
        break;
    }
    case conditionUseVarType::PROTO:
    {
        auto* currentStmt = dynamic_cast<sgs::FuncProto *>(s);
        if (nullptr == currentStmt->getReturnType()) {
            fout << "void ";
            break;
        }
        switch (currentStmt->getReturnType()->getVarType()) {
        case VAR_TYPE::VT_BASIC: translateBasicType(currentStmt->getReturnType(), fout); break;
        case VAR_TYPE::VT_ARRAY: translateArrayType(currentStmt->getReturnType(), fout); break;
        case VAR_TYPE::VT_CLASS: fout << (dynamic_cast<sgs::ClassDef *>(currentStmt)->getDecType())->getName(); break;
        default:break;
        }
        break;
    }
    default:
        break;
    }

}
void translateExpType(sgs::AST *s, std::ofstream &fout) {
    auto*currentStmt = dynamic_cast<sgs::Expression *>(s);
    switch (currentStmt->getExpType()) {
    case EXP_TYPE::ET_OP:
    {
        translateOpExp(currentStmt, fout);
        break;
    }
    case EXP_TYPE::ET_LITERAL:
    {
        translateLiteralExp(currentStmt, fout);
        break;
    }
    case EXP_TYPE::ET_IDENT:
    {
        translateIdExp(currentStmt, fout);
        break;
    }
    case EXP_TYPE::ET_VISIT:
    {
        translateVisitExp(currentStmt, fout);
        break;
    }
    case EXP_TYPE::ET_CALL:
    {
        translateCallExp(currentStmt, fout);
        break;
    }
    case EXP_TYPE::ET_ACCESS:
    {
        translateAccessExp(currentStmt, fout);
        break;
    }
    default:break;
    }
}
void translateStmtType(sgs::AST *s, std::ofstream &fout) {
    auto*currentStmt = dynamic_cast<sgs::Statement *>(s);
    switch (currentStmt->getStmtType()) {
    case STMT_TYPE::ST_ASSIGN:
    {
        translateAssignStmt(currentStmt, fout);
        break;
    }
    case STMT_TYPE::ST_CALL:
    {
        translateCallStmt(currentStmt, fout);
        break;
    }
    case STMT_TYPE::ST_IF:
    {
        translateIfStmt(currentStmt, fout);
        break;
    }
    case STMT_TYPE::ST_WHILE:
    {
        translateWhileStmt(currentStmt, fout);
        break;
    }
    case STMT_TYPE::ST_RETURN: fout << cppTab(cppDepth) << "return result;" << std::endl; break;
    case STMT_TYPE::ST_BREAK: fout << "break;" << std::endl; break;
    case STMT_TYPE::ST_CONTINUE: fout << cppTab(cppDepth) << "continue;" << std::endl; break;
    case STMT_TYPE::ST_BLOCK:
    {
        translateAssignStmt(currentStmt, fout);
        break;
    }
    default:break;
    }
}
void translateFuncDefType(sgs::AST *s, std::ofstream &fout) {
    auto*currentStmt = dynamic_cast<sgs::FuncDef *>(s);
    translateFuncProtoType(currentStmt->getProto(), fout);
    fout << "{" << std::endl;
    cppDepth++;
    translateVarType(s, FUNC, fout);
    fout << "result;" << std::endl;
    translateBlockStmt(currentStmt->getBody(), fout);
    fout << "}" << std::endl;
    cppDepth--;
}
void translateFuncProtoType(sgs::AST *s, std::ofstream &fout) {
    auto*currentStmt = dynamic_cast<sgs::FuncProto *>(s);
    translateVarType(currentStmt, conditionUseVarType::PROTO, fout);
    fout << removeSpace(currentStmt->getName()) << '(';
    int count = currentStmt->getParam().size();
    vector <std::pair<VarType *, string>> paramList = currentStmt->getParam();
    if (paramList.empty()) {
        fout << ")";
        return;
    }
    for (int i = 0; i < count; ++i) {
        sgs::VarType *currentParameter = paramList[i].first;
        string currentName = paramList[i].second;
        switch (currentParameter->getVarType()) {
        case VAR_TYPE::VT_BASIC:
        {
            translateBasicType(currentParameter, fout);
            fout << currentName;
            break;
        }
        case VAR_TYPE::VT_ARRAY:
        {
            VAR_TYPE type = translateArrayType(currentParameter, fout);
            if (VAR_TYPE::VT_BASIC == type) {
                fout << currentName << "[]";
            } else if (VAR_TYPE::VT_ARRAY == type) {
                fout << currentName << "[][]";
            } else if (VAR_TYPE::VT_CLASS == type) {
                fout << dynamic_cast<sgs::ClassType *>(currentParameter)->getName() << ' ';
                fout << currentName << "[]";
            }
			break;
        }
        case VAR_TYPE::VT_CLASS:
        {
            fout << dynamic_cast<sgs::ClassType *>(currentParameter)->getName() << ' ';
            break;
        }
        default:break;
        }
        if (i == count - 1)
            fout << ")";
        else
            fout << ", ";
    }
}
void translateAST(vector<sgs::AST *>stmts, std::ofstream &fout) {
    for (auto& stmt : stmts) {
        switch (stmt->astType) {
        case AT_VARDEF:
        {
            auto* currentStmt = dynamic_cast<sgs::VarDef *>(stmt);
            translateVarType(currentStmt, VARDEF, fout);
            break;
        }
        case AT_CLASS:
        {
            auto* currentStmt = dynamic_cast<sgs::ClassDef *>(stmt);
            translateVarType(currentStmt, CLASS, fout);
            break;
        }
        case AT_EXP:
        {
            sgs::Expression *currentStmt = dynamic_cast<sgs::Expression *>(stmt);
            translateExpType(currentStmt, fout);
            //translateVarType(currentStmt, EXP, fout);
            break;
        }
        case AT_STMT:
        {
            sgs::Statement *currentStmt = dynamic_cast<sgs::Statement *>(stmt);
            translateStmtType(currentStmt, fout);
            break;
        }
        case AT_FUNC:
        {
            sgs::FuncDef *currentStmt = dynamic_cast<sgs::FuncDef *>(stmt);
            translateFuncDefType(currentStmt, fout);
            break;
        }
        case AT_PROTO:
        {
            sgs::FuncProto *currentStmt = dynamic_cast<sgs::FuncProto *>(stmt);
            translateFuncProtoType(currentStmt, fout);
            fout << ";" << std::endl;
            break;
        }
        default:
            break;
        }
    }
    return;
}
void translateToCPP(vector<sgs::AST *> stmts, const std::string& filename) {
    std::ofstream fout(filename);
    fout << "#include <iostream>" << std::endl;
    fout << "#include <string>" << std::endl;
    fout << "using std::string;" << std::endl;
    unsigned int loopNum;
    for (loopNum = 0; loopNum < stmts.size(); ++loopNum) {
        switch (stmts[loopNum]->astType) {
        case AT_CLASS:
        {
            sgs::ClassDef *currentStmt = dynamic_cast<sgs::ClassDef *>(stmts[loopNum]);
            translateVarType(currentStmt, CLASS, fout);
            break;
        }
        case AT_FUNC:
        {
            sgs::FuncDef *currentStmt = dynamic_cast<sgs::FuncDef *>(stmts[loopNum]);
            translateFuncDefType(currentStmt, fout);
            break;
        }
        case AT_PROTO:
        {
            // sgs::FuncProto *currentStmt = dynamic_cast<sgs::FuncProto *>(stmts[loopNum]);
            // translateFuncProtoType(currentStmt, fout);
            // fout << ";" << std::endl;
            break;
        }
        default:
            break;
        }
    }
    fout << "int main() {" << std::endl;
    cppDepth++;
    for (loopNum = 0; loopNum < stmts.size(); ++loopNum) {
        switch (stmts[loopNum]->astType) {
        case AT_VARDEF:
        {
            sgs::VarDef *currentStmt = dynamic_cast<sgs::VarDef *>(stmts[loopNum]);
            translateVarType(currentStmt, VARDEF, fout);
            break;
        }
        case AT_EXP:
        {
            sgs::Expression *currentStmt = dynamic_cast<sgs::Expression *>(stmts[loopNum]);
            translateExpType(currentStmt, fout);
            //translateVarType(currentStmt, EXP, fout);
            break;
        }
        case AT_STMT:
        {
            sgs::Statement *currentStmt = dynamic_cast<sgs::Statement *>(stmts[loopNum]);
            translateStmtType(currentStmt, fout);
            break;
        }
        default:
            break;
        }
    }
    fout << cppTab(cppDepth) << "return 0;" << std::endl;
    fout << "}";
    cppDepth--;
    fout.close();
}
