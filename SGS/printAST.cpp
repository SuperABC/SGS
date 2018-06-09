#include "printAST.h"

using namespace sgs;

static int depth = 0;
void printTab(const string& output, int depth) {
    for (int i = 0; i < depth; ++i)
        std::cout << "    ";
    std::cout << "|-";
    std::cout << output << std::endl;
}

void printBasicType(sgs::VarType *stmtVar) {
    sgs::BasicType *basicVar = dynamic_cast<sgs::BasicType *>(stmtVar);
    switch (basicVar->getBasicType()) {
    case BASIC_TYPE::BT_INT:    std::cout << ASTTab(depth) << "basicType: BT_INT" << std::endl; break;
    case BASIC_TYPE::BT_FLOAT:  std::cout << ASTTab(depth) << "basicType: BT_FLOAT" << std::endl; break;
    case BASIC_TYPE::BT_BOOL:   std::cout << ASTTab(depth) << "basicType: BT_BOOL" << std::endl; break;
    case BASIC_TYPE::BT_CHAR:   std::cout << ASTTab(depth) << "basicType: BT_CHAR" << std::endl; break;
    case BASIC_TYPE::BT_STRING: std::cout << ASTTab(depth) << "basicType: BT_STRING" << std::endl; break;
    default:
        break;
    }
}

void printArrayType(sgs::VarType *stmtVar) {
    sgs::ArrayType *arrayVar = dynamic_cast<sgs::ArrayType *>(stmtVar);
    std::cout << ASTTab(depth) << "decType: VT_ARRAY" << std::endl;
    std::cout << ASTTab(depth) << "length:  " << arrayVar->getLength() << std::endl;
    depth++;
    switch (arrayVar->getEleType()->getVarType()) {
    case VAR_TYPE::VT_BASIC: printBasicType(arrayVar->getEleType()); break;
    case VAR_TYPE::VT_ARRAY: printArrayType(arrayVar->getEleType()); break;
    case VAR_TYPE::VT_CLASS: printClassType(arrayVar->getEleType()); break;
    default:break;
    }
    depth--;
    return;
}
void printClassType(sgs::VarType *stmtVar) {
    sgs::ClassType *classVar = dynamic_cast<sgs::ClassType *>(stmtVar);
    std::cout << ASTTab(depth) << "decType: VT_CLASS" << std::endl;
    std::cout << ASTTab(depth) << "className:  " << classVar->getName() << std::endl;
    std::cout << ASTTab(depth) << "classMember: " << std::endl;
    depth++;
    int count = classVar->getEle().size();
    vector <std::pair<VarType *, string>> classElements = classVar->getEle();
    for (int i = 0; i < count; ++i) {
        sgs::VarType *currentMember = classElements[i].first;
        std::cout << ASTTab(depth) << "No." << (i + 1) << " Member:" << std::endl;
        std::cout << ASTTab(depth) << "memberName:" + classElements[i].second << std::endl;
        depth++;
        switch (currentMember->getVarType()) {
        case VAR_TYPE::VT_BASIC: printBasicType(currentMember); break;
        case VAR_TYPE::VT_ARRAY: printArrayType(currentMember); break;
        case VAR_TYPE::VT_CLASS: printClassType(currentMember); break;
        default:break;
        }
        depth--;
    }
    depth--;
    return;
}
void opSwitchCase(SGSOPERATOR OP) {
    switch (OP) {
    case SGS_OP_PLUS:
        std::cout << ASTTab(depth) << "PLUS" << std::endl;
        break;
    case SGS_OP_PLUSPLUS:
        std::cout << ASTTab(depth) << "PLUSPLUS" << std::endl;
        break;
    case SGS_OP_EQPLUS:
        std::cout << ASTTab(depth) << "EQPLUS" << std::endl;
        break;
    case SGS_OP_MINUS:
        std::cout << ASTTab(depth) << "MINUS" << std::endl;
        break;
    case SGS_OP_MINUSMINUS:
        std::cout << ASTTab(depth) << "MINUSMINUS" << std::endl;
        break;
    case SGS_OP_EQMINUS:
        std::cout << ASTTab(depth) << "EQMINUS" << std::endl;
        break;
    case SGS_OP_NEG:
        std::cout << ASTTab(depth) << "NEG" << std::endl;
        break;
    case SGS_OP_MULTY:
        std::cout << ASTTab(depth) << "MULTY" << std::endl;
        break;
    case SGS_OP_EQMULTY:
        std::cout << ASTTab(depth) << "EQMULTY" << std::endl;
        break;
    case SGS_OP_DIVIDE:
        std::cout << ASTTab(depth) << "DIVIDE" << std::endl;
        break;
    case SGS_OP_EQDIVIDE:
        std::cout << ASTTab(depth) << "EQDIVIDE" << std::endl;
        break;
    case SGS_OP_MOD:
        std::cout << ASTTab(depth) << "MOD" << std::endl;
        break;
    case SGS_OP_EQMOD:
        std::cout << ASTTab(depth) << "EQMOD" << std::endl;
        break;
    case SGS_OP_AND:
        std::cout << ASTTab(depth) << "AND" << std::endl;
        break;
    case SGS_OP_ANDAND:
        std::cout << ASTTab(depth) << "ANDAND" << std::endl;
        break;
    case SGS_OP_EQAND:
        std::cout << ASTTab(depth) << "EQAND" << std::endl;
        break;
    case SGS_OP_OR:
        std::cout << ASTTab(depth) << "OR" << std::endl;
        break;
    case SGS_OP_OROR:
        std::cout << ASTTab(depth) << "OROR" << std::endl;
        break;
    case SGS_OP_EQOR:
        std::cout << ASTTab(depth) << "EQOR" << std::endl;
        break;
    case SGS_OP_NOR:
        std::cout << ASTTab(depth) << "NOR" << std::endl;
        break;
    case SGS_OP_EQNOR:
        std::cout << ASTTab(depth) << "EQNOR" << std::endl;
        break;
    case SGS_OP_INVERSE:
        std::cout << ASTTab(depth) << "INVERSE" << std::endl;
        break;
    case SGS_OP_EQINVERSE:
        std::cout << ASTTab(depth) << "EQINVERSE" << std::endl;
        break;
    case SGS_OP_LBRACE:
        std::cout << ASTTab(depth) << "LBRACE" << std::endl;
        break;
    case SGS_OP_RBRACE:
        std::cout << ASTTab(depth) << "RBRACE" << std::endl;
        break;
    case SGS_OP_LPARENTHESIS:
        std::cout << ASTTab(depth) << "LPARENTHESIS" << std::endl;
        break;
    case SGS_OP_RPARENTHESIS:
        std::cout << ASTTab(depth) << "RPARENTHESIS" << std::endl;
        break;
    case SGS_OP_LBRAKET:
        std::cout << ASTTab(depth) << "LBRAKET" << std::endl;
        break;
    case SGS_OP_RBRAKET:
        std::cout << ASTTab(depth) << "RBRAKET" << std::endl;
        break;
    case SGS_OP_SEMI:
        std::cout << ASTTab(depth) << "SEMI" << std::endl;
        break;
    case SGS_OP_COMMA:
        std::cout << ASTTab(depth) << "COMMA" << std::endl;
        break;
    case SGS_OP_DOT:
        std::cout << ASTTab(depth) << "DOT" << std::endl;
        break;
    case SGS_OP_SMALLER:
        std::cout << ASTTab(depth) << "SMALLER" << std::endl;
        break;
    case SGS_OP_NSMALLER:
        std::cout << ASTTab(depth) << "NSMALLER" << std::endl;
        break;
    case SGS_OP_GREATER:
        std::cout << ASTTab(depth) << "GREATER" << std::endl;
        break;
    case SGS_OP_NGREATER:
        std::cout << ASTTab(depth) << "NGREATER" << std::endl;
        break;
    case SGS_OP_NOT:
        std::cout << ASTTab(depth) << "NOT" << std::endl;
        break;
    case SGS_OP_NOTEQ:
        std::cout << ASTTab(depth) << "NOTEQ" << std::endl;
        break;
    case SGS_OP_EQUAL:
        std::cout << ASTTab(depth) << "EQUAL" << std::endl;
        break;
    case SGS_OP_QUERY:
        std::cout << ASTTab(depth) << "QUERY" << std::endl;
        break;
    case SGS_OP_QUOT:
        std::cout << ASTTab(depth) << "QUOT" << std::endl;
        break;
    case SGS_OP_DBQUOT:
        std::cout << ASTTab(depth) << "DBQUOT" << std::endl;
        break;
    case SGS_OP_CROSS:
        std::cout << ASTTab(depth) << "CROSS" << std::endl;
        break;
    default:
        break;
    }
}
void printOpExp(sgs::Expression *stmtExp) {
    sgs::OpExp *opExp = (sgs::OpExp *)stmtExp;
    std::cout << ASTTab(depth) << "opExpOperator:" << std::endl;
    depth++; opSwitchCase(opExp->getOp()); depth--;
    std::cout << ASTTab(depth) << "LeftExp:" << std::endl;
    depth++; dealWithExpType(opExp->getLeft()); depth--;
    std::cout << ASTTab(depth) << "RightExp:" << std::endl;
    depth++; dealWithExpType(opExp->getRight()); depth--;
}
void printLiteralExp(sgs::Expression *stmtExp) {
    sgs::LiteralExp *literalExp = dynamic_cast<sgs::LiteralExp *>(stmtExp);
    switch (literalExp->getType()->getVarType()) {
    case VAR_TYPE::VT_BASIC:
    {
        sgs::BasicType *basicLiteralExp = dynamic_cast<sgs::BasicType *>(literalExp->getType());
        switch (basicLiteralExp->getBasicType()) {
        case BASIC_TYPE::BT_INT:
        {
            sgs::IntLiteral *intLiteralExp = dynamic_cast<sgs::IntLiteral *>(literalExp);
            std::cout << ASTTab(depth) << "int value:" << intLiteralExp->getValue() << std::endl;
            break;
        }
        case BASIC_TYPE::BT_FLOAT:
        {
            sgs::FloatLiteral *floatLiteralExp = dynamic_cast<sgs::FloatLiteral *>(literalExp);
            std::cout << ASTTab(depth) << "float value:" << floatLiteralExp->getValue() << std::endl;
            break;
        }
        case BASIC_TYPE::BT_BOOL:
        {
            sgs::BoolLiteral *boolLiteralExp = dynamic_cast<sgs::BoolLiteral *>(literalExp);
            std::cout << ASTTab(depth) << "Bool value:" << boolLiteralExp->getValue() << std::endl;
            break;
        }
        case BASIC_TYPE::BT_STRING:
        {
            sgs::StrLiteral *strLiteralExp = dynamic_cast<sgs::StrLiteral *>(literalExp);
            std::cout << ASTTab(depth) << "String value:" << strLiteralExp->getValue() << std::endl;
            break;
        }
        default:
            break;
        }
        break;
    }
    case VAR_TYPE::VT_ARRAY:
    {
        sgs::ArrayLiteral *arrayLiteralExp = dynamic_cast<sgs::ArrayLiteral *>(literalExp);
        std::cout << ASTTab(depth) << "Array value:" << std::endl;
        depth++;
        vector<Expression *>arrayConent = arrayLiteralExp->getValue();
        int count = arrayConent.size();
        for (int i = 0; i < count; ++i) {
            std::cout << ASTTab(depth) << "No. " << i << " content" << std::endl;
            depth++; dealWithExpType(arrayConent[i]); depth--;
        }
        depth--;
        break;
    }
    case VAR_TYPE::VT_CLASS:
    {
        sgs::ClassLiteral *classLiteralExp = dynamic_cast<sgs::ClassLiteral *>(literalExp);
        std::cout << ASTTab(depth) << "Class value:" << std::endl;
        depth++;
        vector<Expression *>classConent = classLiteralExp->getValue();
        int count = classConent.size();
        for (int i = 0; i < count; ++i) {
            std::cout << ASTTab(depth) << "content " << i << std::endl;
            depth++; dealWithExpType(classConent[i]); depth--;
        }
        depth--;
        break;
    }
    default:
        break;
    }
}
void printIdExp(sgs::Expression *stmtExp) {
    sgs::IdExp *idExp = dynamic_cast<sgs::IdExp *>(stmtExp);
    std::cout << ASTTab(depth) << "name:" << idExp->getName() << std::endl;
    return;
}
void printVisitExp(sgs::Expression *stmtExp) {
    sgs::VisitExp *visitExp = dynamic_cast<sgs::VisitExp *>(stmtExp);
    std::cout << ASTTab(depth) << "array name: " << std::endl;
    depth++; dealWithExpType(visitExp->getArray()); depth--;
    std::cout << ASTTab(depth) << "index: " << std::endl;
    depth++; dealWithExpType(visitExp->getIndex()); depth--;
    return;
}
void printCallExp(sgs::Expression *stmtExp) {
    sgs::CallExp *callExp = dynamic_cast<sgs::CallExp *>(stmtExp);
    std::cout << ASTTab(depth) << "function" << std::endl;
    depth++; dealWithFuncProtoType(callExp->getFunction()); depth--;
    depth++;
    int count = callExp->getParam().size();
    vector <Expression *> paramList = callExp->getParam();
    for (int i = 0; i < count; ++i) {
        sgs::Expression *currentParameter = paramList[i];
        std::cout << ASTTab(depth) << "No." << (i + 1) << " parameter:" << std::endl;
        depth++; dealWithExpType(currentParameter); depth--;
    }
    depth--;
}
void printAccessExp(sgs::Expression *stmtExp) {
    sgs::AccessExp *accessExp = dynamic_cast<sgs::AccessExp *>(stmtExp);
    std::cout << ASTTab(depth) << "object:" << std::endl;
    depth++; dealWithExpType(accessExp->getObject()); depth--;
    std::cout << ASTTab(depth) << "member" << std::endl;
    depth++; std::cout << ASTTab(depth) << accessExp->getMember() << std::endl; depth--;
}
void printAssignStmt(sgs::Statement *stmtStmt) {
    sgs::AssignStmt *AssignStmt = dynamic_cast<sgs::AssignStmt *>(stmtStmt);
    std::cout << ASTTab(depth) << "Left Expression:" << std::endl;
    depth++; dealWithExpType(AssignStmt->getLeft()); depth--;
    std::cout << ASTTab(depth) << "Right Expression:" << std::endl;
    depth++; dealWithExpType(AssignStmt->getRight()); depth--;
}
void printCallStmt(sgs::Statement *stmtStmt) {
    sgs::CallStmt *callStmt = dynamic_cast<sgs::CallStmt *>(stmtStmt);
    std::cout << ASTTab(depth) << "function" << std::endl;
    depth++;
    dealWithFuncProtoType(callStmt->getFunction());
    depth--;
    int count = callStmt->getParam().size();
    vector <Expression *> paramList = callStmt->getParam();
    for (int i = 0; i < count; ++i) {
        sgs::Expression *currentParameter = paramList[i];
        std::cout << ASTTab(depth) << "No." << (i + 1) << " parameter:" << std::endl;
        depth++;
        dealWithExpType(currentParameter);
        depth--;
    }
}
void printBlockStmt(sgs::Statement *stmtStmt) {
    sgs::BlockStmt *blockStmt = dynamic_cast<sgs::BlockStmt *>(stmtStmt);
    std::cout << ASTTab(depth) << "block content" << std::endl;
    depth++;
    printAST(blockStmt->getContent());
    depth--;
}
void printIfStmt(sgs::Statement *stmtStmt) {
    sgs::IfStmt *IfStmt = dynamic_cast<sgs::IfStmt *>(stmtStmt);
    std::cout << ASTTab(depth) << "If condition:" << std::endl;
    depth++;
    dealWithExpType(IfStmt->getCond());
    depth--;
    std::cout << ASTTab(depth) << "taken:" << std::endl;
    depth++;
    printBlockStmt(IfStmt->getTaken());
    depth--;
    std::cout << ASTTab(depth) << "untaken:" << std::endl;
    depth++;
	if(IfStmt->getUntaken())printBlockStmt(IfStmt->getUntaken());
    depth--;
}
void printWhileStmt(sgs::Statement *stmtStmt) {
    sgs::WhileStmt *whileStmt = dynamic_cast<sgs::WhileStmt *>(stmtStmt);
    std::cout << ASTTab(depth) << "condition:" << std::endl;
    depth++;
    dealWithExpType(whileStmt->getCondition());
    depth--;
    std::cout << ASTTab(depth) << "body" << std::endl;
    depth++;
    printBlockStmt(whileStmt->getBody());
    depth--;
}
void dealWithVarType(sgs::AST *s, enum conditionUseVarType choice) {
    switch (choice) {
    case conditionUseVarType::VARDEF: //VARDEF
    {
        sgs::VarDef *currentStmt = dynamic_cast<sgs::VarDef *>(s);
        std::cout << ASTTab(depth) << "newVarName: " + currentStmt->getName() << std::endl;
        depth++;
        switch (currentStmt->getDecType()->getVarType()) {
        case VAR_TYPE::VT_BASIC: printBasicType(currentStmt->getDecType()); break;
        case VAR_TYPE::VT_ARRAY: printArrayType(currentStmt->getDecType()); break;
        case VAR_TYPE::VT_CLASS: printClassType(currentStmt->getDecType()); break;
        default:break;
        }
        depth--;
        break;
    }
    case conditionUseVarType::CLASS:
    {
        sgs::ClassDef *currentStmt = dynamic_cast<sgs::ClassDef *>(s);
        std::cout << ASTTab(depth) << "className: " + currentStmt->getDecType()->getName() << std::endl;
        depth++;
        printClassType(currentStmt->getDecType());
        depth--;
        break;
    }
    case conditionUseVarType::EXP:
    {
        sgs::Expression *currentStmt = dynamic_cast<sgs::Expression *>(s);
        //std::cout << "	expRes:" << std::endl;
        depth++;
        // switch (currentStmt->getResType()->getVarType()) {
        // case VAR_TYPE::VT_BASIC: printBasicType(currentStmt->getResType()); break;
        // case VAR_TYPE::VT_ARRAY: printArrayType(currentStmt->getResType()); break;
        // case VAR_TYPE::VT_CLASS: printClassType(currentStmt->getResType()); break;
        // default:break;
        // }
        depth--;
        break;
    }
    case conditionUseVarType::FUNC:
    {
        // sgs::FuncDef *currentStmt = dynamic_cast<sgs::FuncDef *>(s);
        break;
    }
    case conditionUseVarType::PROTO:
    {
        sgs::FuncProto *currentStmt = dynamic_cast<sgs::FuncProto *>(s);
        if (nullptr == currentStmt->getReturnType()) {
            std::cout << ASTTab(depth) << "return type is void" << std::endl;
            break;
        }
        switch (currentStmt->getReturnType()->getVarType()) {
        case VAR_TYPE::VT_BASIC: printBasicType(currentStmt->getReturnType()); break;
        case VAR_TYPE::VT_ARRAY: printArrayType(currentStmt->getReturnType()); break;
        case VAR_TYPE::VT_CLASS: printClassType(currentStmt->getReturnType()); break;
        default:break;
        }
        break;
    }
    default:
        break;
    }

}
void dealWithExpType(sgs::AST *s) {
    sgs::Expression *currentStmt = dynamic_cast<sgs::Expression *>(s);
    switch (currentStmt->getExpType()) {
    case EXP_TYPE::ET_OP:
    {
        std::cout << ASTTab(depth) << "expType: ET_OP" << std::endl;
        depth++;
        printOpExp(currentStmt);
        depth--;
        break;
    }
    case EXP_TYPE::ET_LITERAL:
    {
        std::cout << ASTTab(depth) << "expType: ET_LITERAL" << std::endl;
        depth++;
        printLiteralExp(currentStmt);
        depth--;
        break;
    }
    case EXP_TYPE::ET_IDENT:
    {
        std::cout << ASTTab(depth) << "expType: ET_IDENT" << std::endl;
        depth++;
        printIdExp(currentStmt);
        depth--;
        break;
    }
    case EXP_TYPE::ET_VISIT:
    {
        std::cout << ASTTab(depth) << "expType: ET_VISIT" << std::endl;
        depth++;
        printVisitExp(currentStmt);
        depth--;
        break;
    }
    case EXP_TYPE::ET_CALL:
    {
        std::cout << ASTTab(depth) << "expType: ET_CALL" << std::endl;
        depth++;
        printCallExp(currentStmt);
        depth--;
        break;
    }
    case EXP_TYPE::ET_ACCESS:
    {
        std::cout << ASTTab(depth) << "expType: ET_ACCESS" << std::endl;
        depth++;
        printAccessExp(currentStmt);
        depth--;
        break;
    }
    default:break;
    }
}
void dealWithStmtType(sgs::AST *s) {
    sgs::Statement *currentStmt = dynamic_cast<sgs::Statement *>(s);
    switch (currentStmt->getStmtType()) {
    case STMT_TYPE::ST_ASSIGN:
    {
        std::cout << ASTTab(depth) << "stmtType: ST_ASSIGN" << std::endl;
        depth++;
        printAssignStmt(currentStmt);
        depth--;
        break;
    }
    case STMT_TYPE::ST_CALL:
    {
        std::cout << ASTTab(depth) << "stmtType: ST_CALL" << std::endl;
        depth++;
        printCallStmt(currentStmt);
        depth--;
        break;
    }
    case STMT_TYPE::ST_IF:
    {
        std::cout << ASTTab(depth) << "stmtType: ST_IF" << std::endl;
        depth++;
        printIfStmt(currentStmt);
        depth--;
        break;
    }
    case STMT_TYPE::ST_WHILE:
    {
        std::cout << ASTTab(depth) << "stmtType: ST_WHILE" << std::endl;
        depth++;
        printWhileStmt(currentStmt);
        depth--;
        break;
    }
    case STMT_TYPE::ST_RETURN: std::cout << ASTTab(depth) << "stmtType: ST_RETURN" << std::endl; break;
    case STMT_TYPE::ST_BREAK: std::cout << ASTTab(depth) << "stmtType: ST_BREAK" << std::endl; break;
    case STMT_TYPE::ST_CONTINUE: std::cout << ASTTab(depth) << "stmtType: ST_CONTINUE" << std::endl; break;
    case STMT_TYPE::ST_BLOCK:
    {
        std::cout << ASTTab(depth) << "stmtType: ST_BLOCK" << std::endl;
        depth++;
        printBlockStmt(currentStmt);
        depth--;
        break;
    }
    default:break;
    }
}
void dealWithFuncDefType(sgs::AST *s) {
    sgs::FuncDef *currentStmt = dynamic_cast<sgs::FuncDef *>(s);
    std::cout << ASTTab(depth) << "proto:" << std::endl;
    depth++;
    dealWithFuncProtoType(currentStmt->getProto());
    depth--;
    std::cout << ASTTab(depth) << "body:" << std::endl;
    depth++;
    printBlockStmt(currentStmt->getBody());
    depth--;
}
void dealWithFuncProtoType(sgs::AST *s) {
    sgs::FuncProto *currentStmt = dynamic_cast<sgs::FuncProto *>(s);
    std::cout << ASTTab(depth) << "name:" << std::endl;
    depth++;
    std::cout << ASTTab(depth) << currentStmt->getName() << std::endl;
    depth--;
    std::cout << ASTTab(depth) << "parameters:" << std::endl;
    depth++;
    int count = currentStmt->getParam().size();
    vector <std::pair<VarType *, string>> paramList = currentStmt->getParam();
    for (int i = 0; i < count; ++i) {
        sgs::VarType *currentParameter = paramList[i].first;
        std::cout << ASTTab(depth) << "No." << (i + 1) << " parameter:" << std::endl;
        std::cout << ASTTab(depth) << "parameterName:" + paramList[i].second << std::endl;
        depth++;
        switch (currentParameter->getVarType()) {
        case VAR_TYPE::VT_BASIC: printBasicType(currentParameter); break;
        case VAR_TYPE::VT_ARRAY: printArrayType(currentParameter); break;
        case VAR_TYPE::VT_CLASS: printClassType(currentParameter); break;
        default:break;
        }
        depth--;
    }
    depth--;
    std::cout << ASTTab(depth) << "return value type:" << std::endl;
    depth++;
    dealWithVarType(currentStmt, conditionUseVarType::PROTO);
    depth--;
}
void printAST(vector<sgs::AST *> stmts) {
    for (unsigned int loopNum = 0; loopNum < stmts.size(); ++loopNum) {
        std::cout << ASTTab(depth) << "No." << loopNum << " statement is parsed" << std::endl;
        std::cout << ASTTab(depth) << "--------------result---------------" << std::endl;
        switch (stmts[loopNum]->astType) {
        case AT_VARDEF:
        {
            sgs::VarDef *currentStmt = dynamic_cast<sgs::VarDef *>(stmts[loopNum]);
            std::cout << ASTTab(depth) << "astType: AT_VARDEF" << std::endl;
            depth++; dealWithVarType(currentStmt, VARDEF); depth--;
            std::cout << std::endl;
            break;
        }
        case AT_CLASS:
        {
            sgs::ClassDef *currentStmt = dynamic_cast<sgs::ClassDef *>(stmts[loopNum]);
            std::cout << ASTTab(depth) << "astType: AT_CLASS" << std::endl;
            depth++; dealWithVarType(currentStmt, CLASS); depth--;
            std::cout << std::endl;
            break;
        }
        case AT_EXP:
        {
            sgs::Expression *currentStmt = dynamic_cast<sgs::Expression *>(stmts[loopNum]);
            std::cout << ASTTab(depth) << "astType: AT_EXP" << std::endl;
            depth++;
            dealWithExpType(currentStmt);
            dealWithVarType(currentStmt, EXP);
            depth--;
            std::cout << std::endl;
            break;
        }
        case AT_STMT:
        {
            sgs::Statement *currentStmt = dynamic_cast<sgs::Statement *>(stmts[loopNum]);
            std::cout << ASTTab(depth) << "astType: AT_STMT" << std::endl;
            depth++;
            dealWithStmtType(currentStmt);
            depth--;
            std::cout << std::endl;
            break;
        }
        case AT_FUNC:
        {
            sgs::FuncDef *currentStmt = dynamic_cast<sgs::FuncDef *>(stmts[loopNum]);
            std::cout << ASTTab(depth) << "astType: AT_FUNC" << std::endl;
            depth++; dealWithFuncDefType(currentStmt); depth--;
            std::cout << std::endl;
            break;
        }
        case AT_PROTO:
        {
            sgs::FuncProto *currentStmt = dynamic_cast<sgs::FuncProto *>(stmts[loopNum]);
            std::cout << ASTTab(depth) << "astType: AT_PROTO" << std::endl;
            depth++;  dealWithFuncProtoType(currentStmt); depth--;
            std::cout << std::endl;
            break;
        }
        default:
            break;
        }
    }
    return;
}