#include "printAST.h"

int depth = 0;
void printTab(const string& output, int depth) {
    for (int i = 0; i < depth; ++depth)
        std::cout << "    ";
    std::cout << "|-";
    std::cout << output << std::endl;
}
void printBasicType(sgs::VarType *stmtVar) {
    sgs::BasicType *basicVar = dynamic_cast<sgs::BasicType *>(stmtVar);
    switch (basicVar->getBasicType()) {
    case BASIC_TYPE::BT_INT:    std::cout << Tab(depth) << "basicType: BT_INT" << std::endl; break;
    case BASIC_TYPE::BT_FLOAT:  std::cout << Tab(depth) << "basicType: BT_FLOAT" << std::endl; break;
    case BASIC_TYPE::BT_BOOL:   std::cout << Tab(depth) << "basicType: BT_BOOL" << std::endl; break;
    case BASIC_TYPE::BT_CHAR:   std::cout << Tab(depth) << "basicType: BT_CHAR" << std::endl; break;
    case BASIC_TYPE::BT_STRING: std::cout << Tab(depth) << "basicType: BT_STRING" << std::endl; break;
    default:
        break;
    }
    return;
}
void printArrayType(sgs::VarType *stmtVar) {
    sgs::ArrayType *arrayVar = (sgs::ArrayType *)stmtVar;
    std::cout << Tab(depth) << "decType: VT_ARRAY" << std::endl;
    std::cout << Tab(depth) << "length:  " << arrayVar->getLength() << std::endl;
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
    sgs::ClassType *classVar = (sgs::ClassType *)stmtVar;
    std::cout << Tab(depth) << "decType: VT_CLASS" << std::endl;
    std::cout << Tab(depth) << "className:  " << classVar->getName() << std::endl;
    std::cout << Tab(depth) << "classMember: " << std::endl;
    depth++;
    int count = classVar->getEle().size();
    vector <std::pair<VarType *, string>> classElements = classVar->getEle();
    for (int i = 0; i < count; ++i) {
        sgs::VarType *currentMember = classElements[i].first;
        std::cout << Tab(depth) << "No." << (i + 1) << " Member:" << std::endl;
        std::cout << Tab(depth) << "memberName:" + classElements[i].second << std::endl;
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
        std::cout << Tab(depth) << "PLUS" << std::endl;
        break;
    case SGS_OP_PLUSPLUS:
        std::cout << Tab(depth) << "PLUSPLUS" << std::endl;
        break;
    case SGS_OP_EQPLUS:
        std::cout << Tab(depth) << "EQPLUS" << std::endl;
        break;
    case SGS_OP_MINUS:
        std::cout << Tab(depth) << "MINUS" << std::endl;
        break;
    case SGS_OP_MINUSMINUS:
        std::cout << Tab(depth) << "MINUSMINUS" << std::endl;
        break;
    case SGS_OP_EQMINUS:
        std::cout << Tab(depth) << "EQMINUS" << std::endl;
        break;
    case SGS_OP_NEG:
        std::cout << Tab(depth) << "NEG" << std::endl;
        break;
    case SGS_OP_MULTY:
        std::cout << Tab(depth) << "MULTY" << std::endl;
        break;
    case SGS_OP_EQMULTY:
        std::cout << Tab(depth) << "EQMULTY" << std::endl;
        break;
    case SGS_OP_DIVIDE:
        std::cout << Tab(depth) << "DIVIDE" << std::endl;
        break;
    case SGS_OP_EQDIVIDE:
        std::cout << Tab(depth) << "EQDIVIDE" << std::endl;
        break;
    case SGS_OP_MOD:
        std::cout << Tab(depth) << "MOD" << std::endl;
        break;
    case SGS_OP_EQMOD:
        std::cout << Tab(depth) << "EQMOD" << std::endl;
        break;
    case SGS_OP_AND:
        std::cout << Tab(depth) << "AND" << std::endl;
        break;
    case SGS_OP_ANDAND:
        std::cout << Tab(depth) << "ANDAND" << std::endl;
        break;
    case SGS_OP_EQAND:
        std::cout << Tab(depth) << "EQAND" << std::endl;
        break;
    case SGS_OP_OR:
        std::cout << Tab(depth) << "OR" << std::endl;
        break;
    case SGS_OP_OROR:
        std::cout << Tab(depth) << "OROR" << std::endl;
        break;
    case SGS_OP_EQOR:
        std::cout << Tab(depth) << "EQOR" << std::endl;
        break;
    case SGS_OP_NOR:
        std::cout << Tab(depth) << "NOR" << std::endl;
        break;
    case SGS_OP_EQNOR:
        std::cout << Tab(depth) << "EQNOR" << std::endl;
        break;
    case SGS_OP_INVERSE:
        std::cout << Tab(depth) << "INVERSE" << std::endl;
        break;
    case SGS_OP_EQINVERSE:
        std::cout << Tab(depth) << "EQINVERSE" << std::endl;
        break;
    case SGS_OP_LBRACE:
        std::cout << Tab(depth) << "LBRACE" << std::endl;
        break;
    case SGS_OP_RBRACE:
        std::cout << Tab(depth) << "RBRACE" << std::endl;
        break;
    case SGS_OP_LPARENTHESIS:
        std::cout << Tab(depth) << "LPARENTHESIS" << std::endl;
        break;
    case SGS_OP_RPARENTHESIS:
        std::cout << Tab(depth) << "RPARENTHESIS" << std::endl;
        break;
    case SGS_OP_LBRAKET:
        std::cout << Tab(depth) << "LBRAKET" << std::endl;
        break;
    case SGS_OP_RBRAKET:
        std::cout << Tab(depth) << "RBRAKET" << std::endl;
        break;
    case SGS_OP_SEMI:
        std::cout << Tab(depth) << "SEMI" << std::endl;
        break;
    case SGS_OP_COMMA:
        std::cout << Tab(depth) << "COMMA" << std::endl;
        break;
    case SGS_OP_DOT:
        std::cout << Tab(depth) << "DOT" << std::endl;
        break;
    case SGS_OP_SMALLER:
        std::cout << Tab(depth) << "SMALLER" << std::endl;
        break;
    case SGS_OP_NSMALLER:
        std::cout << Tab(depth) << "NSMALLER" << std::endl;
        break;
    case SGS_OP_GREATER:
        std::cout << Tab(depth) << "GREATER" << std::endl;
        break;
    case SGS_OP_NGREATER:
        std::cout << Tab(depth) << "NGREATER" << std::endl;
        break;
    case SGS_OP_NOT:
        std::cout << Tab(depth) << "NOT" << std::endl;
        break;
    case SGS_OP_NOTEQ:
        std::cout << Tab(depth) << "NOTEQ" << std::endl;
        break;
    case SGS_OP_EQUAL:
        std::cout << Tab(depth) << "EQUAL" << std::endl;
        break;
    case SGS_OP_QUERY:
        std::cout << Tab(depth) << "QUERY" << std::endl;
        break;
    case SGS_OP_QUOT:
        std::cout << Tab(depth) << "QUOT" << std::endl;
        break;
    case SGS_OP_DBQUOT:
        std::cout << Tab(depth) << "DBQUOT" << std::endl;
        break;
    case SGS_OP_CROSS:
        std::cout << Tab(depth) << "CROSS" << std::endl;
        break;
    default:
        break;
    }
}
void printOpExp(sgs::Expression *stmtExp) {
    sgs::OpExp *opExp = (sgs::OpExp *)stmtExp;
    std::cout << Tab(depth) << "opExpOperator:" << std::endl;
    depth++; opSwitchCase(opExp->getOp()); depth--;
    std::cout << Tab(depth) << "LeftExp:" << std::endl;
    depth++; dealWithExpType(opExp->getLeft()); depth--;
    std::cout << Tab(depth) << "RightExp:" << std::endl;
    depth++; dealWithExpType(opExp->getRight()); depth--;
}
void printLiteralExp(sgs::Expression *stmtExp) {
    sgs::LiteralExp *literalExp = (sgs::LiteralExp *)stmtExp;
    switch (literalExp->getType()->getVarType()) {
    case VAR_TYPE::VT_BASIC:
    {
        sgs::BasicType *basicLiteralExp = (sgs::BasicType *)literalExp->getType();
        switch (basicLiteralExp->getBasicType()) {
        case BASIC_TYPE::BT_INT:
        {
            sgs::IntLiteral *intLiteralExp = (sgs::IntLiteral *)literalExp;
            std::cout << Tab(depth) << "int value:" << intLiteralExp->getValue() << std::endl;
            break;
        }
        case BASIC_TYPE::BT_FLOAT:
        {
            sgs::FloatLiteral *floatLiteralExp = (sgs::FloatLiteral *)literalExp;
            std::cout << Tab(depth) << "float value:" << floatLiteralExp->getValue() << std::endl;
            break;
        }
        case BASIC_TYPE::BT_BOOL:
        {
            sgs::BoolLiteral *boolLiteralExp = (sgs::BoolLiteral *)literalExp;
            std::cout << Tab(depth) << "Bool value:" << boolLiteralExp->getValue() << std::endl;
            break;
        }
        case BASIC_TYPE::BT_STRING:
        {
            sgs::StrLiteral *strLiteralExp = (sgs::StrLiteral *)literalExp;
            std::cout << Tab(depth) << "String value:" << strLiteralExp->getValue() << std::endl;
            break;
        }
        default:
            break;
        }
        break;
    }
    case VAR_TYPE::VT_ARRAY:
    {
        sgs::ArrayLiteral *arrayLiteralExp = (sgs::ArrayLiteral *)literalExp;
        std::cout << Tab(depth) << "Array value:" << std::endl;
        depth++;
        vector<Expression *>arrayConent = arrayLiteralExp->getValue();
        int count = arrayConent.size();
        for (int i = 0; i < count; ++i) {
            std::cout << Tab(depth) << "No. " << i << " content" << std::endl;
            depth++; dealWithExpType(arrayConent[i]); depth--;
        }
        depth--;
        break;
    }
    case VAR_TYPE::VT_CLASS:
    {
        sgs::ClassLiteral *classLiteralExp = (sgs::ClassLiteral *)literalExp;
        std::cout << Tab(depth) << "Class value:" << std::endl;
        depth++;
        vector<Expression *>classConent = classLiteralExp->getValue();
        int count = classConent.size();
        for (int i = 0; i < count; ++i) {
            std::cout << Tab(depth) << "content " << i << std::endl;
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
    sgs::IdExp *idExp = (sgs::IdExp *)stmtExp;
    std::cout << Tab(depth) << "name:" << idExp->getName() << std::endl;
    return;
}
void printVisitExp(sgs::Expression *stmtExp) {
    sgs::VisitExp *visitExp = (sgs::VisitExp *)stmtExp;
    std::cout << Tab(depth) << "array name: " << std::endl;
    depth++; dealWithExpType(visitExp->getArray()); depth--;
    std::cout << Tab(depth) << "index: " << std::endl;
    depth++; dealWithExpType(visitExp->getIndex()); depth--;
    return;
}
void printCallExp(sgs::Expression *stmtExp) {
    sgs::CallExp *callExp = (sgs::CallExp *)stmtExp;
    std::cout << Tab(depth) << "function" << std::endl;
    depth++; dealWithFuncProtoType(callExp->getFunction()); depth--;
    depth++;
    int count = callExp->getParam().size();
    vector <Expression *> paramList = callExp->getParam();
    for (int i = 0; i < count; ++i) {
        sgs::Expression *currentParameter = paramList[i];
        std::cout << Tab(depth) << "No." << (i + 1) << " parameter:" << std::endl;
        depth++; dealWithExpType(currentParameter); depth--;
    }
    depth--;
}
void printAccessExp(sgs::Expression *stmtExp) {
    sgs::AccessExp *accessExp = (sgs::AccessExp *)stmtExp;
    std::cout << Tab(depth) << "object:" << std::endl;
    depth++; dealWithExpType(accessExp->getObject()); depth--;
    std::cout << Tab(depth) << "member" << std::endl;
    depth++; std::cout << Tab(depth) << accessExp->getMember() << std::endl; depth--;
}
void printAssignStmt(sgs::Statement *stmtStmt) {
    sgs::AssignStmt *AssignStmt = (sgs::AssignStmt *)stmtStmt;
    std::cout << Tab(depth) << "Left Expression:" << std::endl;
    depth++; dealWithExpType(AssignStmt->getLeft()); depth--;
    std::cout << Tab(depth) << "Right Expression:" << std::endl;
    depth++; dealWithExpType(AssignStmt->getRight()); depth--;
}
void printCallStmt(sgs::Statement *stmtStmt) {
    sgs::CallStmt *callStmt = (sgs::CallStmt *)stmtStmt;
    std::cout << Tab(depth) << "function" << std::endl;
    depth++;
    dealWithFuncProtoType(callStmt->getFunction());
    depth--;
    int count = callStmt->getParam().size();
    vector <Expression *> paramList = callStmt->getParam();
    for (int i = 0; i < count; ++i) {
        sgs::Expression *currentParameter = paramList[i];
        std::cout << Tab(depth) << "No." << (i + 1) << " parameter:" << std::endl;
        depth++;
        dealWithExpType(currentParameter);
        depth--;
    }
}
void printBlockStmt(sgs::Statement *stmtStmt) {
    sgs::BlockStmt *blockStmt = (sgs::BlockStmt *)stmtStmt;
    std::cout << Tab(depth) << "block content" << std::endl;
    depth++;
    printAST(blockStmt->getContent());
    depth--;
}
void printIfStmt(sgs::Statement *stmtStmt) {
    sgs::IfStmt *IfStmt = (sgs::IfStmt *)stmtStmt;
    std::cout << Tab(depth) << "If condition:" << std::endl;
    depth++;
    dealWithExpType(IfStmt->getCond());
    depth--;
    std::cout << Tab(depth) << "taken:" << std::endl;
    depth++;
    printBlockStmt(IfStmt->getTaken());
    depth--;
    std::cout << Tab(depth) << "untaken:" << std::endl;
    depth++;
    printBlockStmt(IfStmt->getUntaken());
    depth--;
}
void printWhileStmt(sgs::Statement *stmtStmt) {
    sgs::WhileStmt *whileStmt = (sgs::WhileStmt *)stmtStmt;
    std::cout << Tab(depth) << "condition:" << std::endl;
    depth++;
    dealWithExpType(whileStmt->getCondition());
    depth--;
    std::cout << Tab(depth) << "body" << std::endl;
    depth++;
    printBlockStmt(whileStmt->getBody());
    depth--;
}
void dealWithVarType(sgs::AST *s, enum conditionUseVarType choice) {
    switch (choice) {
    case conditionUseVarType::TYPEDEF: //TYPEDEF
    {
        sgs::TypeDef *currentStmt = (sgs::TypeDef *)s;
        std::cout << Tab(depth) << "newVarName: " + currentStmt->getName() << std::endl;
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
        sgs::ClassDef *currentStmt = (sgs::ClassDef *)s;
        std::cout << Tab(depth) << "className: " + currentStmt->getDecType()->getName() << std::endl;
        depth++;
        printClassType(currentStmt->getDecType());
        depth--;
        break;
    }
    case conditionUseVarType::EXP:
    {
        sgs::Expression *currentStmt = (sgs::Expression *)s;
        //std::cout << "	expRes:" << std::endl;
        depth++;
        switch (currentStmt->getResType()->getVarType()) {
        case VAR_TYPE::VT_BASIC: printBasicType(currentStmt->getResType()); break;
        case VAR_TYPE::VT_ARRAY: printArrayType(currentStmt->getResType()); break;
        case VAR_TYPE::VT_CLASS: printClassType(currentStmt->getResType()); break;
        default:break;
        }
        depth--;
        break;
    }
    case conditionUseVarType::FUNC:
    {
        sgs::FuncDef *currentStmt = (sgs::FuncDef *)s;
        break;
    }
    case conditionUseVarType::PROTO:
    {
        sgs::FuncProto *currentStmt = (sgs::FuncProto *)s;
        if (nullptr == currentStmt->getReturnType()) {
            std::cout << Tab(depth) << "return type is void" << std::endl;
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
    sgs::Expression *currentStmt = (sgs::Expression *)s;
    switch (currentStmt->getExpType()) {
    case EXP_TYPE::ET_OP:
    {
        std::cout << Tab(depth) << "expType: ET_OP" << std::endl;
        depth++;
        printOpExp(currentStmt);
        depth--;
        break;
    }
    case EXP_TYPE::ET_LITERAL:
    {
        std::cout << Tab(depth) << "expType: ET_LITERAL" << std::endl;
        depth++;
        printLiteralExp(currentStmt);
        depth--;
        break;
    }
    case EXP_TYPE::ET_IDENT:
    {
        std::cout << Tab(depth) << "expType: ET_IDENT" << std::endl;
        depth++;
        printIdExp(currentStmt);
        depth--;
        break;
    }
    case EXP_TYPE::ET_VISIT:
    {
        std::cout << Tab(depth) << "expType: ET_VISIT" << std::endl;
        depth++;
        printVisitExp(currentStmt);
        depth--;
        break;
    }
    case EXP_TYPE::ET_CALL:
    {
        std::cout << Tab(depth) << "expType: ET_CALL" << std::endl;
        depth++;
        printCallExp(currentStmt);
        depth--;
        break;
    }
    case EXP_TYPE::ET_ACCESS:
    {
        std::cout << Tab(depth) << "expType: ET_ACCESS" << std::endl;
        depth++;
        printAccessExp(currentStmt);
        depth--;
        break;
    }
    default:break;
    }
}
void dealWithStmtType(sgs::AST *s) {
    sgs::Statement *currentStmt = (sgs::Statement *)s;
    switch (currentStmt->getStmtType()) {
    case STMT_TYPE::ST_ASSIGN:
    {
        std::cout << Tab(depth) << "stmtType: ST_ASSIGN" << std::endl;
        depth++;
        printAssignStmt(currentStmt);
        depth--;
        break;
    }
    case STMT_TYPE::ST_CALL:
    {
        std::cout << Tab(depth) << "stmtType: ST_CALL" << std::endl;
        depth++;
        printCallStmt(currentStmt);
        depth--;
        break;
    }
    case STMT_TYPE::ST_IF:
    {
        std::cout << Tab(depth) << "stmtType: ST_IF" << std::endl;
        depth++;
        printIfStmt(currentStmt);
        depth--;
        break;
    }
    case STMT_TYPE::ST_WHILE:
    {
        std::cout << Tab(depth) << "stmtType: ST_WHILE" << std::endl;
        depth++;
        printWhileStmt(currentStmt);
        depth--;
        break;
    }
    case STMT_TYPE::ST_RETURN: std::cout << Tab(depth) << "stmtType: ST_RETURN" << std::endl; break;
    case STMT_TYPE::ST_BREAK: std::cout << Tab(depth) << "stmtType: ST_BREAK" << std::endl; break;
    case STMT_TYPE::ST_CONTINUE: std::cout << Tab(depth) << "stmtType: ST_CONTINUE" << std::endl; break;
    case STMT_TYPE::ST_BLOCK:
    {
        std::cout << Tab(depth) << "stmtType: ST_BLOCK" << std::endl;
        depth++;
        printBlockStmt(currentStmt);
        depth--;
        break;
    }
    default:break;
    }
}
void dealWithFuncDefType(sgs::AST *s) {
    sgs::FuncDef *currentStmt = (sgs::FuncDef *)s;
    std::cout << Tab(depth) << "proto:" << std::endl;
    depth++;
    dealWithFuncProtoType(currentStmt->getProto());
    depth--;
    std::cout << Tab(depth) << "body:" << std::endl;
    depth++;
    printBlockStmt(currentStmt->getBody());
    depth--;
}
void dealWithFuncProtoType(sgs::AST *s) {
    sgs::FuncProto *currentStmt = (sgs::FuncProto *)s;
    std::cout << Tab(depth) << "name:" << std::endl;
    depth++;
    std::cout << Tab(depth) << currentStmt->getName() << std::endl;
    depth--;
    std::cout << Tab(depth) << "parameters:" << std::endl;
    depth++;
    int count = currentStmt->getParam().size();
    vector <std::pair<VarType *, string>> paramList = currentStmt->getParam();
    for (int i = 0; i < count; ++i) {
        sgs::VarType *currentParameter = paramList[i].first;
        std::cout << Tab(depth) << "No." << (i + 1) << " parameter:" << std::endl;
        std::cout << Tab(depth) << "parameterName:" + paramList[i].second << std::endl;
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
    std::cout << Tab(depth) << "return value type:" << std::endl;
    depth++;
    dealWithVarType(currentStmt, conditionUseVarType::PROTO);
    depth--;
}
void printAST(vector<sgs::AST *> stmts) {
    for (unsigned int loopNum = 0; loopNum < stmts.size(); ++loopNum) {
        std::cout << Tab(depth) << "No." << loopNum << " statement is parsed" << std::endl;
        std::cout << Tab(depth) << "--------------result---------------" << std::endl;
        switch (stmts[loopNum]->astType) {
        case AT_TYPEDEF:
        {
            sgs::TypeDef *currentStmt = (sgs::TypeDef *)stmts[loopNum];
            std::cout << Tab(depth) << "astType: AT_TYPEDEF" << std::endl;
            depth++; dealWithVarType(currentStmt, TYPEDEF); depth--;
            std::cout << std::endl;
            break;
        }
        case AT_CLASS:
        {
            sgs::ClassDef *currentStmt = (sgs::ClassDef *)stmts[loopNum];
            std::cout << Tab(depth) << "astType: AT_CLASS" << std::endl;
            depth++; dealWithVarType(currentStmt, CLASS); depth--;
            std::cout << std::endl;
            break;
        }
        case AT_EXP:
        {
            sgs::Expression *currentStmt = (sgs::Expression *)stmts[loopNum];
            std::cout << Tab(depth) << "astType: AT_EXP" << std::endl;
            depth++;
            dealWithExpType(currentStmt);
            dealWithVarType(currentStmt, EXP);
            depth--;
            std::cout << std::endl;
            break;
        }
        case AT_STMT:
        {
            sgs::Statement *currentStmt = (sgs::Statement *)stmts[loopNum];
            std::cout << Tab(depth) << "astType: AT_STMT" << std::endl;
            depth++;
            dealWithStmtType(currentStmt);
            depth--;
            std::cout << std::endl;
            break;
        }
        case AT_FUNC:
        {
            sgs::FuncDef *currentStmt = (sgs::FuncDef *)stmts[loopNum];
            std::cout << Tab(depth) << "astType: AT_FUNC" << std::endl;
            depth++; dealWithFuncDefType(currentStmt); depth--;
            std::cout << std::endl;
            break;
        }
        case AT_PROTO:
        {
            sgs::FuncProto *currentStmt = (sgs::FuncProto *)stmts[loopNum];
            std::cout << Tab(depth) << "astType: AT_PROTO" << std::endl;
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