#include "transform.h"

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

sgs_backend::Expression* transformExpr(sgs::Expression* expr, sgs_backend::Context& context,
                                       Env* env) {
    switch (expr->getExpType()) {
    case sgs::ET_OP: {
        const auto op = dynamic_cast<sgs::OpExp*>(expr);

        switch (op->getOp()) {
        case SGS_OP_PLUS: {
            if (op->getLeft()->getResType()->getVarType() != sgs::VT_BASIC
                || op->getRight()->getResType()->getVarType() != sgs::VT_BASIC) {
                throw std::exception("invalid operator type in add binary operator expression");
            }
            const auto lt = dynamic_cast<sgs::BasicType*>(op->getLeft());
            const auto rt = dynamic_cast<sgs::BasicType*>(op->getRight());
            if (lt->getBasicType() == rt->getBasicType()) {
                return new sgs_backend::BinopExp(sgs_backend::ADD,
                                                 transformExpr(op->getLeft(), context, env),
                                                 transformExpr(op->getRight(), context, env),
                                                 context
                );
            }
            if (lt->getBasicType() == sgs::BT_FLOAT || rt->getBasicType() == sgs::BT_FLOAT) {
                throw std::exception("implicit casting from int to float is forbidden.");
            }
            return new sgs_backend::BinopExp(sgs_backend::ADD,
                                             transformExpr(op->getLeft(), context, env),
                                             transformExpr(op->getRight(), context, env),
                                             context
            );
        }
        case SGS_OP_MINUS: {
            if (op->getLeft()->getResType()->getVarType() != sgs::VT_BASIC
                || op->getRight()->getResType()->getVarType() != sgs::VT_BASIC) {
                throw std::exception("invalid operator type in sub binary operator expression");
            }
            const auto lt = dynamic_cast<sgs::BasicType*>(op->getLeft());
            const auto rt = dynamic_cast<sgs::BasicType*>(op->getRight());
            if (lt->getBasicType() == rt->getBasicType()) {
                return new sgs_backend::BinopExp(sgs_backend::SUB,
                                                 transformExpr(op->getLeft(), context, env),
                                                 transformExpr(op->getRight(), context, env),
                                                 context
                );
            }
            if (lt->getBasicType() == sgs::BT_FLOAT || rt->getBasicType() == sgs::BT_FLOAT) {
                throw std::exception("implicit casting from int to float is forbidden.");
            }
            return new sgs_backend::BinopExp(sgs_backend::SUB,
                                             transformExpr(op->getLeft(), context, env),
                                             transformExpr(op->getRight(), context, env),
                                             context
            );
        }
        case SGS_OP_NEG: {
            if (op->getLeft()->getResType()->getVarType() != sgs::VT_BASIC) {
                throw std::exception("invalid operator type in neg unique operator expression");
            }
            const auto lt = dynamic_cast<sgs::BasicType*>(op->getLeft()->getResType());
            auto* res = transformExpr(op->getLeft(), context, env);
            const auto rlt = dynamic_cast<sgs_backend::SBasicType*>(res->getResType());
            switch (rlt->getBasicType()) {
            case sgs_backend::BasicType::INTEGER:
                return new sgs_backend::BinopExp(sgs_backend::SUB, sgs_backend::getLiteral(0, context), res, context);
            case sgs_backend::BasicType::FLOAT:
                return new sgs_backend::BinopExp(sgs_backend::SUB, sgs_backend::getLiteral(0.0, context), res, context);
            case sgs_backend::BasicType::BOOLEAN:
                return new sgs_backend::BinopExp(sgs_backend::SUB, sgs_backend::getLiteral(false, context), res,
                                                 context);
            case sgs_backend::BasicType::CHAR:
                return new sgs_backend::BinopExp(sgs_backend::SUB, sgs_backend::getLiteral('\0', context), res,
                                                 context);
            default: ;
                throw std::exception("unrecogized basic type");
            }
        }
        case SGS_OP_MULTY: {
            if (op->getLeft()->getResType()->getVarType() != sgs::VT_BASIC
                || op->getRight()->getResType()->getVarType() != sgs::VT_BASIC) {
                throw std::exception("invalid operator type in mult binary operator expression");
            }
            const auto lt = dynamic_cast<sgs::BasicType*>(op->getLeft());
            const auto rt = dynamic_cast<sgs::BasicType*>(op->getRight());
            if (lt->getBasicType() == rt->getBasicType()) {
                return new sgs_backend::BinopExp(sgs_backend::MUL,
                                                 transformExpr(op->getLeft(), context, env),
                                                 transformExpr(op->getRight(), context, env),
                                                 context
                );
            }
            if (lt->getBasicType() == sgs::BT_FLOAT || rt->getBasicType() == sgs::BT_FLOAT) {
                throw std::exception("implicit casting from int to float is forbidden.");
            }
            return new sgs_backend::BinopExp(sgs_backend::MUL,
                                             transformExpr(op->getLeft(), context, env),
                                             transformExpr(op->getRight(), context, env),
                                             context
            );
        }
        case SGS_OP_DIVIDE: {
            if (op->getLeft()->getResType()->getVarType() != sgs::VT_BASIC
                || op->getRight()->getResType()->getVarType() != sgs::VT_BASIC) {
                throw std::exception("invalid operator type in divide binary operator expression");
            }
            const auto lt = dynamic_cast<sgs::BasicType*>(op->getLeft());
            const auto rt = dynamic_cast<sgs::BasicType*>(op->getRight());
            if (lt->getBasicType() == rt->getBasicType()) {
                return new sgs_backend::BinopExp(sgs_backend::DIV,
                                                 transformExpr(op->getLeft(), context, env),
                                                 transformExpr(op->getRight(), context, env),
                                                 context
                );
            }
            if (lt->getBasicType() == sgs::BT_FLOAT || rt->getBasicType() == sgs::BT_FLOAT) {
                throw std::exception("implicit casting from int to float is forbidden.");
            }
            return new sgs_backend::BinopExp(sgs_backend::DIV,
                                             transformExpr(op->getLeft(), context, env),
                                             transformExpr(op->getRight(), context, env),
                                             context
            );
        }
        case SGS_OP_MOD: {
            if (op->getLeft()->getResType()->getVarType() != sgs::VT_BASIC
                || op->getRight()->getResType()->getVarType() != sgs::VT_BASIC) {
                throw std::exception("invalid operator type in mod binary operator expression");
            }
            const auto lt = dynamic_cast<sgs::BasicType*>(op->getLeft());
            const auto rt = dynamic_cast<sgs::BasicType*>(op->getRight());
            if (lt->getBasicType() != sgs::BT_INT || rt->getBasicType() != sgs::BT_INT) {
                throw std::exception("both handside of mod operator must be integer");
            }
            return new sgs_backend::BinopExp(sgs_backend::MOD,
                                             transformExpr(op->getLeft(), context, env),
                                             transformExpr(op->getRight(), context, env),
                                             context);
        }
        case SGS_OP_AND: {
            if (op->getLeft()->getResType()->getVarType() != sgs::VT_BASIC
                || op->getRight()->getResType()->getVarType() != sgs::VT_BASIC) {
                throw std::exception("invalid operator type in and binary operator expression");
            }
            const auto lt = dynamic_cast<sgs::BasicType*>(op->getLeft());
            const auto rt = dynamic_cast<sgs::BasicType*>(op->getRight());
            if (lt->getBasicType() != sgs::BT_BOOL || rt->getBasicType() != sgs::BT_BOOL) {
                throw std::exception("both handside of and operator must be integer");
            }
            return new sgs_backend::BinopExp(sgs_backend::AND,
                                             transformExpr(op->getLeft(), context, env),
                                             transformExpr(op->getRight(), context, env),
                                             context);
        }
        case SGS_OP_OR: {
            if (op->getLeft()->getResType()->getVarType() != sgs::VT_BASIC
                || op->getRight()->getResType()->getVarType() != sgs::VT_BASIC) {
                throw std::exception("invalid operator type in or binary operator expression");
            }
            const auto lt = dynamic_cast<sgs::BasicType*>(op->getLeft());
            const auto rt = dynamic_cast<sgs::BasicType*>(op->getRight());
            if (lt->getBasicType() != sgs::BT_BOOL || rt->getBasicType() != sgs::BT_BOOL) {
                throw std::exception("both handside of or operator must be integer");
            }
            return new sgs_backend::BinopExp(sgs_backend::OR,
                                             transformExpr(op->getLeft(), context, env),
                                             transformExpr(op->getRight(), context, env),
                                             context);
        }
        case SGS_OP_NOR: {
            if (op->getLeft()->getResType()->getVarType() != sgs::VT_BASIC
                || op->getRight()->getResType()->getVarType() != sgs::VT_BASIC) {
                throw std::exception("invalid operator type in nor binary operator expression");
            }
            const auto lt = dynamic_cast<sgs::BasicType*>(op->getLeft());
            const auto rt = dynamic_cast<sgs::BasicType*>(op->getRight());
            if (lt->getBasicType() != sgs::BT_BOOL || rt->getBasicType() != sgs::BT_BOOL) {
                throw std::exception("both handside of nor operator must be integer");
            }
            return new sgs_backend::UniopExp(sgs_backend::NOT,
                new sgs_backend::BinopExp(sgs_backend::OR,
                    transformExpr(
                        op->getLeft(), context,
                        env),
                    transformExpr(
                        op->getRight(), context,
                        env),
                    context), context);
        }
            // case SGS_OP_INVERSE: break;
            // case SGS_OP_EQINVERSE: break;
        case SGS_OP_SMALLER: {
            if (op->getLeft()->getResType()->getVarType() != sgs::VT_BASIC
                || op->getRight()->getResType()->getVarType() != sgs::VT_BASIC) {
                throw std::exception("invalid operator type in less binary operator expression");
            }
            const auto lt = dynamic_cast<sgs::BasicType*>(op->getLeft());
            const auto rt = dynamic_cast<sgs::BasicType*>(op->getRight());
            if (lt->getBasicType() == rt->getBasicType()) {
                return new sgs_backend::BinopExp(sgs_backend::LT,
                                                 transformExpr(op->getLeft(), context, env),
                                                 transformExpr(op->getRight(), context, env),
                                                 context
                );
            }
            if (lt->getBasicType() == sgs::BT_FLOAT || rt->getBasicType() == sgs::BT_FLOAT) {
                throw std::exception("implicit casting from int to float is forbidden.");
            }
            return new sgs_backend::BinopExp(sgs_backend::LT,
                                             transformExpr(op->getLeft(), context, env),
                                             transformExpr(op->getRight(), context, env),
                                             context
            );
        }
        case SGS_OP_GREATER: {
            if (op->getLeft()->getResType()->getVarType() != sgs::VT_BASIC
                || op->getRight()->getResType()->getVarType() != sgs::VT_BASIC) {
                throw std::exception("invalid operator type in greater binary operator expression");
            }
            const auto lt = dynamic_cast<sgs::BasicType*>(op->getLeft());
            const auto rt = dynamic_cast<sgs::BasicType*>(op->getRight());
            if (lt->getBasicType() == rt->getBasicType()) {
                return new sgs_backend::BinopExp(sgs_backend::GT,
                                                 transformExpr(op->getLeft(), context, env),
                                                 transformExpr(op->getRight(), context, env),
                                                 context
                );
            }
            if (lt->getBasicType() == sgs::BT_FLOAT || rt->getBasicType() == sgs::BT_FLOAT) {
                throw std::exception("implicit casting from int to float is forbidden.");
            }
            return new sgs_backend::BinopExp(sgs_backend::GT,
                                             transformExpr(op->getLeft(), context, env),
                                             transformExpr(op->getRight(), context, env),
                                             context
            );
        }
        case SGS_OP_NOT: {
            if (op->getLeft()->getResType()->getVarType() != sgs::VT_BASIC) {
                throw std::exception("invalid operator type in not unique operator expression");
            }
            return new sgs_backend::UniopExp(sgs_backend::NOT,
                                             transformExpr(op->getLeft(), context, env),
                                             context);
        }
        case SGS_OP_EQUAL: {
            if (op->getLeft()->getResType()->getVarType() != sgs::VT_BASIC
                || op->getRight()->getResType()->getVarType() != sgs::VT_BASIC) {
                throw std::exception("invalid operator type in divide binary operator expression");
            }
            const auto lt = dynamic_cast<sgs::BasicType*>(op->getLeft());
            const auto rt = dynamic_cast<sgs::BasicType*>(op->getRight());
            if (lt->getBasicType() == rt->getBasicType()) {
                return new sgs_backend::BinopExp(
                    sgs_backend::DIV,
                    transformExpr(op->getLeft(), context, env),
                    transformExpr(op->getRight(), context, env),
                    context
                );
            }
            if (lt->getBasicType() == sgs::BT_FLOAT || rt->getBasicType() == sgs::BT_FLOAT) {
                throw std::exception("implicit casting from int to float is forbidden.");
            }
            return new sgs_backend::BinopExp(
                sgs_backend::DIV,
                transformExpr(op->getLeft(), context, env),
                transformExpr(op->getRight(), context, env),
                context
            );
        }
        default:
            throw std::exception("unexpected operator");;
        }
    }
    case sgs::ET_LITERAL: {
        const auto lit = dynamic_cast<sgs::LiteralExp*>(expr);
        switch (lit->getType()->getVarType()) { 
        case sgs::VT_BASIC: {
            const auto bt = dynamic_cast<sgs::BasicType*>(lit->getType());
            switch (bt->getBasicType()) {
            case sgs::BT_INT: {
                return sgs_backend::getLiteral(dynamic_cast<sgs::IntLiteral*>(lit)->getValue(), context);
            }
            case sgs::BT_FLOAT: {
                return sgs_backend::getLiteral(dynamic_cast<sgs::FloatLiteral*>(lit)->getValue(), context);
            }
            case sgs::BT_BOOL: {
                return sgs_backend::getLiteral(dynamic_cast<sgs::BoolLiteral*>(lit)->getValue(), context);
            }
            // case sgs::BT_CHAR: {
            //     // return sgs_backend::getLiteral(dynamic_cast<sgs::CharLiteral*>(lit)->getValue(), context);
            // }
            case sgs::BT_STRING: {
                return new sgs_backend::ConstString(dynamic_cast<sgs::StrLiteral*>(lit)->getValue());
            }
            default: 
                throw std::exception("unknown basic type when constructing literal");
            }
        }
        case sgs::VT_ARRAY: {
            // TODO ?
        }
        case sgs::VT_CLASS: {
            // TODO ?
        }
        default: 
            throw std::exception("unsupported literal type");
        }
    }
    case sgs::ET_IDENT: {
        const auto id = dynamic_cast<sgs::IdExp*>(expr);
        return new sgs_backend::IdExp(id->getName(), transformType(id->getResType(), context));
    }
    case sgs::ET_VISIT: {
        const auto visit = dynamic_cast<sgs::VisitExp*>(expr);
        if (visit->getArray()->getResType()->getVarType() != sgs::VT_ARRAY) {
            throw std::exception("visit expression : expecting type array");
        }
        if (visit->getIndex()->getResType()->getVarType() != sgs::VT_BASIC) {
            throw std::exception("visit expression : expecting integer index");
        }
        const auto bt = dynamic_cast<sgs::BasicType*>(visit->getIndex()->getResType());
        if (bt->getBasicType() != sgs::BT_INT) {
            throw std::exception("visit expression : expecting integer index");
        }
        return new sgs_backend::VisitExp(
            transformExpr(visit->getArray(), context, env),
            transformExpr(visit->getIndex(), context, env)
        );
    }
    case sgs::ET_CALL: {
        const auto call = dynamic_cast<sgs::CallExp*>(expr);
        // type check
        const auto proto = call->getFunction();
        if (proto->getParam().size() != call->getParam().size()) {
            throw std::exception("call expression : paramater number mismatch");
        }
        vector<sgs_backend::Expression*> exp;
        for (size_t i = 0; i < proto->getParam().size(); i++) {
            const auto t1 = transformType(proto->getParam()[i].first, context);
            const auto t2 = transformType(call->getParam()[i]->getResType(), context);
            if (!sgs_backend::sameType(t1, t2)) {
                throw std::exception("call expression : parameter type mismatch");
            }
            exp.push_back(transformExpr(call->getParam()[i], context, env));
        }
        return new sgs_backend::CallExp(proto->getName(), {}, transformType(proto->getReturnType(), context));
    }
    case sgs::ET_ACCESS: {
        const auto access = dynamic_cast<sgs::AccessExp*>(expr);
        const auto obj = access->getObject();
        if (obj->getResType()->getVarType() != sgs::VT_CLASS) {
            throw std::exception("access expression : Class type is expected");
        }
        const auto ct = dynamic_cast<sgs::ClassType*>(obj->getResType());
        size_t i = 0;
        for (; i < ct->getEle().size(); i++) {
            if (ct->getEle()[i].second == access->getMember()) {
                break;
            }
        }
        if (i == ct->getEle().size()) {
            throw std::exception((ct->getName() + " has no member " + access->getMember()).c_str());
        }
        return new sgs_backend::AccessExp(
            transformExpr(obj, context, env),
            access->getMember()
        );
    }
    default: 
        throw std::exception("transform exp : what the fuck?");
    }
}

sgs_backend::Statement* transformStmt(sgs::Statement* stmt, sgs_backend::Context& context, Env* env) {
    switch (stmt->getStmtType()) {
    case sgs::ST_ASSIGN: {
        const auto ass = dynamic_cast<sgs::AssignStmt*>(stmt);
        return new sgs_backend::AssignStmt(
            transformExpr(ass->getLeft(), context, env),
            transformExpr(ass->getRight(), context, env)
        );
    }
    case sgs::ST_CALL: {
        const auto call = dynamic_cast<sgs::CallStmt*>(stmt);
        const auto proto = call->getFunction();
        if (proto->getParam().size() != call->getParam().size()) {
            throw std::exception("call expression : paramater number mismatch");
        }
        string name = proto->getName();
        if (name == "print an int") name = "printNum";
        if (name == "print a number") name = "printFloat";
        if (name == "print a boolean") name = "printNum";
        // if (name == "")
        vector<sgs_backend::Expression*> exp;
        for (size_t i = 0; i < proto->getParam().size(); i++) {
            const auto t1 = transformType(proto->getParam()[i].first, context);
            const auto t2 = transformType(call->getParam()[i]->getResType(), context);
            if (!sgs_backend::sameType(t1, t2)) {
                throw std::exception("call expression : parameter type mismatch");
            }
            exp.push_back(transformExpr(call->getParam()[i], context, env));
        }
        return new sgs_backend::ExpStmt(new sgs_backend::CallExp(name, {}, transformType(proto->getReturnType(), context)));
    }
    case sgs::ST_IF: {
        const auto ifs = dynamic_cast<sgs::IfStmt*>(stmt);
        if (ifs->getCond()->getResType()->getVarType() != sgs::VT_BASIC) {
            throw std::exception("unexpected type at if.cond");
        } 
        auto cond = transformExpr(ifs->getCond(), context, env);
        const auto ct = dynamic_cast<sgs_backend::SBasicType*>(cond->getResType());
        if (ct->getBasicType() != sgs_backend::BasicType::BOOLEAN) {
            cond = new sgs_backend::UniopExp(sgs_backend::NOT, new sgs_backend::UniopExp(sgs_backend::NOT, cond, context), context);}
        const auto pass = transformStmt(ifs->getTaken(), context, env);
        const auto fail = transformStmt(ifs->getUntaken(), context, env);
        return new sgs_backend::IfStmt(cond, pass, fail);
    }
    case sgs::ST_WHILE: {
        const auto whs = dynamic_cast<sgs::WhileStmt*>(stmt);
        if (whs->getCondition()->getResType()->getVarType() != sgs::VT_BASIC) {
            throw std::exception("unexpected type at while.cond");
        }
        auto cond = transformExpr(whs->getCondition(), context, env);
        const auto ct = dynamic_cast<sgs_backend::SBasicType*>(cond->getResType());
        if (ct->getBasicType() != sgs_backend::BasicType::BOOLEAN) {
            cond = new sgs_backend::UniopExp(sgs_backend::NOT, new sgs_backend::UniopExp(sgs_backend::NOT, cond, context), context);
        }
        const auto body = transformStmt(whs->getBody(), context, env);
        return new sgs_backend::WhileStmt(cond, dynamic_cast<sgs_backend::BlockStmt*>(body));
    }
    case sgs::ST_RETURN: {
        const auto ret = dynamic_cast<sgs::ReturnStmt*>(stmt);
        const auto val = env->find("ret");
        return new sgs_backend::ReturnStmt(new sgs_backend::IdExp("ret", val));
    }
    case sgs::ST_BREAK: {
        return new sgs_backend::BreakStmt();
    }
    case sgs::ST_CONTINUE: {
        return new sgs_backend::ContinueStmt();
    }
    case sgs::ST_BLOCK: {
        const auto block = dynamic_cast<sgs::BlockStmt*>(stmt);
        vector<sgs_backend::Statement*> res;
        for (auto i : block->getContent()) {
            res.emplace_back(transformStmt(dynamic_cast<sgs::Statement*>(i), context, env));
        }
        return new sgs_backend::BlockStmt(res);
    }
    default:
        throw std::exception("What the fuck??? at statement transform");
        ;
    }
}

sgs_backend::AST* transformAST(sgs::AST* ast, sgs_backend::Context& context, Env* env) {
    switch (ast->astType) {
    case sgs::AT_TYPEDEF: {
        const auto typeDef = dynamic_cast<sgs::TypeDef*>(ast);
        return new sgs_backend::TypeDef(transformType(typeDef->getDecType(), context), typeDef->getName());
    }
    case sgs::AT_CLASS: {
        const auto classDef = dynamic_cast<sgs::ClassDef*>(ast);
        return new sgs_backend::TypeDef(transformType(classDef->getDecType(), context),
                                        classDef->getDecType()->getName());
    }
    case sgs::AT_EXP: {
        return new sgs_backend::ExpStmt(transformExpr(dynamic_cast<sgs::Expression*>(ast), context, env));
    }
    case sgs::AT_STMT: {
        const auto stmt = dynamic_cast<sgs::Statement*>(ast);
        return transformStmt(stmt, context, env);
    }
    case sgs::AT_PROTO: {
        const auto proto = dynamic_cast<sgs::FuncProto*>(ast);
        const auto retTy = transformType(proto->getReturnType(), context);
        vector<std::pair<sgs_backend::SType*, string>> res;
        for (const auto& x : proto->getParam()) {
            res.emplace_back(std::make_pair(transformType(x.first, context), x.second));
        }
        return new sgs_backend::FuncProto(retTy, proto->getName(), res);
    }
    case sgs::AT_FUNC: {
        const auto func = dynamic_cast<sgs::FuncDef*>(ast);
        const auto proto = dynamic_cast<sgs_backend::FuncProto*>(transformAST(func->getProto(), context, env));
        const auto retTy = dynamic_cast<sgs_backend::FuncProto*>(proto)->getReturnType();
        Env* new_env = new Env(nullptr);
        new_env->getBindings()["ret"] = retTy;
        const auto block = dynamic_cast<sgs_backend::BlockStmt*>(transformAST(func->getBody(), context, new_env));
        block->getContent().insert(block->getContent().begin(), new sgs_backend::VarDefStmt(retTy, nullptr, "ret"));
        block->getContent().push_back(new sgs_backend::ReturnStmt(new sgs_backend::IdExp("ret", retTy)));
        return new sgs_backend::FuncDef(proto, block);
    }
    default:
        throw std::exception("What the fuck?? : at ast transform");
    }
}

sgs_backend::Content transform(vector<sgs::AST*>& cont1, sgs_backend::Context& context) {
    sgs_backend::Content content;
    // sgs_backend::FuncProto
    sgs_backend::FuncProto* mainProto = new sgs_backend::FuncProto(context.getIntType(), "main", {});
    vector<sgs_backend::Statement*> mainStmt;
    for (auto&& x : cont1) {
        switch (x->astType) {
        case sgs::AT_TYPEDEF:
        case sgs::AT_CLASS:
        case sgs::AT_PROTO:
        case sgs::AT_FUNC:
            content.push_back(transformAST(x, context, nullptr));
            break;
        case sgs::AT_STMT:
        case sgs::AT_EXP:
            mainStmt.push_back(dynamic_cast<sgs_backend::Statement*>(transformAST(x, context, nullptr)));
        default:
            throw std::exception("what the fuck?? : at transform");
        }
    }
    mainStmt.push_back(new sgs_backend::ReturnStmt(sgs_backend::getLiteral(0, context)));
    content.push_back(new sgs_backend::FuncDef(mainProto, new sgs_backend::BlockStmt(mainStmt)));
    return content;
}
