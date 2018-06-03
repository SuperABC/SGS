#pragma once
#include "ast.h"
#include <vector>
#include "syntax.h"
#include "machine.h"
using std::vector;

sgs_backend::SType* transformType(sgs::VarType* type, sgs_backend::Context& context) {
    switch (type->getVarType()) {
    case sgs::VT_BASIC: {
        const auto bt = dynamic_cast<sgs::BasicType*>(type);
        switch (bt->getBasicType()) {
        case sgs::BT_INT: return context.getIntType();
        case sgs::BT_FLOAT: return context.getFloatType();
        case sgs::BT_BOOL: return context.getBoolType();
        case sgs::BT_CHAR: return context.getCharType();
        default:
            assert(false);
        }
    }
    case sgs::VT_ARRAY: {
        const auto at = dynamic_cast<sgs::ArrayType*>(type);
        return context.getArrayType(transformType(at->getEleType(), context), at->getLength());
    }
    case sgs::VT_CLASS: {
        const auto ct = dynamic_cast<sgs::ClassType*>(type);
        vector<std::pair<string, sgs_backend::SType*>> tps;
        for (auto&& x : ct->getEle()) {
            tps.emplace_back(x.second, transformType(x.first, context));
        }
        return context.getTupleType(tps, ct->getName());
    }
    default: ;
    }
    std::cerr << "What the fuck" << std::endl;
    return nullptr;
}

sgs_backend::Expression* transformExpr(sgs::Expression* expr, sgs_backend::Context& context) {
    switch (expr->getExpType()) {
    case sgs::ET_OP: {
        const auto op = dynamic_cast<sgs::OpExp*>(expr);
        switch (op->getOp()) {
        case SGS_OP_PLUS: {
            if (op->getLeft()->getResType()->getVarType() != sgs::VT_BASIC
                || op->getRight()->getResType()->getVarType() != sgs::VT_BASIC) {
                throw std::exception("invalid operator type in add binary operator expression");
            }
            const auto lt = dynamic_cast<sgs::BasicType*>(expr);
            const auto rt = dynamic_cast<sgs::BasicType*>(expr);
            if (lt->getBasicType() == rt->getBasicType()) {
                return new sgs_backend::BinopExp(sgs_backend::ADD,
                    transformExpr(op->getLeft(), context),
                    transformExpr(op->getRight(), context), 
                    context
                    );
            }
        }
        case SGS_OP_MINUS: break;
        case SGS_OP_NEG: break;
        case SGS_OP_MULTY: break;
        case SGS_OP_DIVIDE: break;
        case SGS_OP_MOD: break;
        case SGS_OP_AND: break;
        case SGS_OP_OR: break;
        case SGS_OP_NOR: break;
        case SGS_OP_EQNOR: break;
        case SGS_OP_INVERSE: break;
        case SGS_OP_EQINVERSE: break;
        case SGS_OP_SMALLER: break;
        case SGS_OP_GREATER: break;
        case SGS_OP_NOT: break;
        case SGS_OP_EQUAL: break;
        default: 
            // too hard ...
            ;
        }
    }
    case sgs::ET_LITERAL: {
        
    }
    case sgs::ET_IDENT: {
        
    }
    case sgs::ET_VISIT: {
        
    }
    case sgs::ET_CALL: {
        
    }
    case sgs::ET_ACCESS: {
        
    }
    default: ;
    }
    std::cerr << "What the fuck" << std::endl;
    return nullptr;
}

sgs_backend::Statement* transformStmt(sgs::Statement* stmt, sgs_backend::Context& context) {
    switch (stmt->getStmtType()) {
    case sgs::ST_ASSIGN: break;
    case sgs::ST_CALL: break;
    case sgs::ST_IF: break;
    case sgs::ST_WHILE: break;
    case sgs::ST_RETURN: break;
    case sgs::ST_BREAK: break;
    case sgs::ST_CONTINUE: break;
    case sgs::ST_BLOCK: break;
    default: ;
    }
    std::cerr << "What the fuck" << std::endl;
    return nullptr;
}

sgs_backend::AST* transformAST(sgs::AST* ast, sgs_backend::Context& context) {
    switch (ast->astType) {
    case sgs::AT_TYPEDEF: break;
    case sgs::AT_CLASS: break;
    case sgs::AT_EXP: break;
    case sgs::AT_STMT: break;
    case sgs::AT_PROTO: break;
    case sgs::AT_FUNC: break;
    default: ;
    }
    std::cerr << "What the fuck" << std::endl;
    return nullptr;
}

sgs_backend::Content transform(vector<sgs::AST*>& cont1, sgs_backend::Context& context) {
    sgs_backend::Content content;
    for (auto&& x : cont1) {
        content.push_back(transformAST(x, context));
    }
    return content;
}