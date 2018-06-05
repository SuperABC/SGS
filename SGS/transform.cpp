#include "transform.h"

sgs_backend::SType* transformType(sgs::VarType* type, sgs_backend::Context& context) {
    if (!type) return nullptr;
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
        const auto left = transformExpr(op->getLeft(), context, env);
        sgs_backend::Expression* right = nullptr;
        const auto lt = dynamic_cast<sgs_backend::SBasicType*>(left->getResType());
        sgs_backend::SBasicType* rt = nullptr;
        if (op->getOp() != SGS_OP_NOT && op->getOp() != SGS_OP_NEG) {
            right = transformExpr(op->getRight(), context, env);
            rt = dynamic_cast<sgs_backend::SBasicType*>(right->getResType());
        }
        switch (op->getOp()) {
        case SGS_OP_PLUS: {
            if (left->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE ||
                right->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE) {
                throw std::exception("invalid operator type in add binary operator expression");
            }
            if (lt->getBasicType() == rt->getBasicType()) {
                return new sgs_backend::BinopExp(sgs_backend::ADD, left, right, context);
            }
            if (lt->getBasicType() == sgs_backend::BasicType::FLOAT || rt->getBasicType() == sgs_backend::BasicType::FLOAT) {
                throw std::exception("implicit casting from int to float is forbidden.");
            }
            return new sgs_backend::BinopExp(sgs_backend::ADD, left, right, context);
        }
        case SGS_OP_MINUS: {
            if (left->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE ||
                right->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE) {
                throw std::exception("invalid operator type in sub binary operator expression");
            }
            
            if (lt->getBasicType() == rt->getBasicType()) {
                return new sgs_backend::BinopExp(sgs_backend::SUB, left, right, context);
            }
            if (lt->getBasicType() == sgs_backend::BasicType::FLOAT || rt->getBasicType() == sgs_backend::BasicType::FLOAT) {
                throw std::exception("implicit casting from int to float is forbidden.");
            }
            return new sgs_backend::BinopExp(sgs_backend::SUB, left, right, context);
        }
        case SGS_OP_NEG: {
            if (left->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE) {
                throw std::exception("invalid operator type in add binary operator expression");
            }
            switch (lt->getBasicType()) {
            case sgs_backend::BasicType::INTEGER:
                return new sgs_backend::BinopExp(sgs_backend::SUB, sgs_backend::getLiteral(0, context), left, context);
            case sgs_backend::BasicType::FLOAT:
                return new sgs_backend::BinopExp(sgs_backend::SUB, sgs_backend::getLiteral(0.0, context), left, context);
            case sgs_backend::BasicType::BOOLEAN:
                return new sgs_backend::BinopExp(sgs_backend::SUB, sgs_backend::getLiteral(0, context), left, context);
            case sgs_backend::BasicType::CHAR:
                return new sgs_backend::BinopExp(sgs_backend::SUB, sgs_backend::getLiteral('\0', context), left, context);
            default: ;
                throw std::exception("unrecogized basic type");
            }
        }
        case SGS_OP_MULTY: {
            if (left->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE ||
                right->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE) {
                throw std::exception("invalid operator type in mul binary operator expression");
            }
            if (lt->getBasicType() == rt->getBasicType()) {
                return new sgs_backend::BinopExp(sgs_backend::MUL, left, right, context);
            }
            if (lt->getBasicType() == sgs_backend::BasicType::FLOAT || rt->getBasicType() == sgs_backend::BasicType::FLOAT) {
                throw std::exception("implicit casting from int to float is forbidden.");
            }
            return new sgs_backend::BinopExp(sgs_backend::MUL, left, right, context);
        }
        case SGS_OP_DIVIDE: {
            if (left->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE ||
                right->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE) {
                throw std::exception("invalid operator type in div binary operator expression");
            }
            if (lt->getBasicType() == rt->getBasicType()) {
                return new sgs_backend::BinopExp(sgs_backend::DIV, left, right, context);
            }
            if (lt->getBasicType() == sgs_backend::BasicType::FLOAT || rt->getBasicType() == sgs_backend::BasicType::FLOAT) {
                throw std::exception("implicit casting from int to float is forbidden.");
            }
            return new sgs_backend::BinopExp(sgs_backend::DIV, left, right, context);
        }
        case SGS_OP_MOD: {
            if (left->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE ||
                right->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE) {
                throw std::exception("invalid operator type in mod binary operator expression");
            }
            if (lt->getBasicType() != sgs_backend::BasicType::INTEGER || rt->getBasicType() != sgs_backend::BasicType::INTEGER) {
                throw std::exception("only integer can use mod operator");
            }
            return new sgs_backend::BinopExp(sgs_backend::MOD, left, right, context);
        }
        case SGS_OP_AND: {
            if (left->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE ||
                right->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE) {
                throw std::exception("invalid operator type in and binary operator expression");
            }
            if (lt->getBasicType() != sgs_backend::BasicType::BOOLEAN || rt->getBasicType() != sgs_backend::BasicType::BOOLEAN) {
                throw std::exception("only boolean can use and operator");
            }
            return new sgs_backend::BinopExp(sgs_backend::AND, left, right, context);
        }
        case SGS_OP_OR: {
            if (left->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE ||
                right->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE) {
                throw std::exception("invalid operator type in or binary operator expression");
            }
            if (lt->getBasicType() != sgs_backend::BasicType::BOOLEAN || rt->getBasicType() != sgs_backend::BasicType::BOOLEAN) {
                throw std::exception("only boolean can use or operator");
            }
            return new sgs_backend::BinopExp(sgs_backend::OR, left, right, context);
        }
        case SGS_OP_NOR: {
            if (left->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE ||
                right->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE) {
                throw std::exception("invalid operator type in nor binary operator expression");
            }
            if (lt->getBasicType() != sgs_backend::BasicType::BOOLEAN || rt->getBasicType() != sgs_backend::BasicType::BOOLEAN) {
                throw std::exception("only boolean can use nor operator");
            }
            return new sgs_backend::UniopExp(sgs_backend::NOT,
                new sgs_backend::BinopExp(sgs_backend::OR,
                        left, right,
                    context), context);
        }
        case SGS_OP_SMALLER: {
            if (left->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE ||
                right->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE) {
                throw std::exception("invalid operator type in less binary operator expression");
            }
            if (lt->getBasicType() == rt->getBasicType()) {
                return new sgs_backend::BinopExp(sgs_backend::LT, left, right, context);
            }
            if (lt->getBasicType() == sgs_backend::BasicType::FLOAT || rt->getBasicType() == sgs_backend::BasicType::FLOAT) {
                throw std::exception("implicit casting from int to float is forbidden.");
            }
            return new sgs_backend::BinopExp(sgs_backend::LT, left, right, context);
        }
        case SGS_OP_GREATER: {
            if (left->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE ||
                right->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE) {
                throw std::exception("invalid operator type in greater binary operator expression");
            }
            if (lt->getBasicType() == rt->getBasicType()) {
                return new sgs_backend::BinopExp(sgs_backend::GT, left, right, context);
            }
            if (lt->getBasicType() == sgs_backend::BasicType::FLOAT || rt->getBasicType() == sgs_backend::BasicType::FLOAT) {
                throw std::exception("implicit casting from int to float is forbidden.");
            }
            return new sgs_backend::BinopExp(sgs_backend::GT, left, right, context);
        }
        case SGS_OP_NOT: {
            if (left->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE) {
                throw std::exception("invalid operator type in not unique operator expression");
            }
            return new sgs_backend::UniopExp(sgs_backend::NOT, left, context);
        }
        case SGS_OP_EQUAL: {
            if (left->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE ||
                right->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE) {
                throw std::exception("invalid operator type in EQ binary operator expression");
            }
            if (lt->getBasicType() == rt->getBasicType()) {
                return new sgs_backend::BinopExp(sgs_backend::EQ, left, right, context);
            }
            if (lt->getBasicType() == sgs_backend::BasicType::FLOAT || rt->getBasicType() == sgs_backend::BasicType::FLOAT) {
                throw std::exception("implicit casting from int to float is forbidden.");
            }
            return new sgs_backend::BinopExp(sgs_backend::EQ, left, right, context);
        }
        default:
            throw std::exception("unexpected operator");
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
        if (auto type = env->find(id->getName())) {
            return new sgs_backend::IdExp(id->getName(), type);
        } 
        throw std::exception((string("Identifier is not found :") + id->getName()).c_str());
    }
    case sgs::ET_VISIT: {
        const auto visit = dynamic_cast<sgs::VisitExp*>(expr);
        const auto array = transformExpr(visit->getArray(), context, env);
        const auto index = transformExpr(visit->getIndex(), context, env);
        if (array->getResType()->getLevel() != sgs_backend::Types::ARRAY_TYPE) {
            throw std::exception("visit expression : expecting type array");
        }
        if (index->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE) {
            throw std::exception("visit expression : expecting integer index");
        }
        const auto bt = dynamic_cast<sgs_backend::SBasicType*>(index->getResType());
        if (bt->getBasicType() != sgs_backend::BasicType::INTEGER) {
            throw std::exception("visit expression : expecting integer index");
        }
        return new sgs_backend::VisitExp(array, index);
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
            const auto param = transformExpr(call->getParam()[i], context, env);
            const auto t1 = transformType(proto->getParam()[i].first, context);
            const auto t2 = param->getResType();
            if (!sgs_backend::sameType(t1, t2)) {
                throw std::exception("call expression : parameter type mismatch");
            }
            exp.push_back(param);
        }
        string name = proto->getName();
        if (name == "print an int") name = "printNum";
        if (name == "print a number") name = "printFloat";
        if (name == "print a str") name = "printStr";
        if (name == "read an int") name = "readNum";
        if (name == "read a number") name = "readFloat";
        return new sgs_backend::CallExp(name, exp, transformType(proto->getReturnType(), context));
    }
    case sgs::ET_ACCESS: {
        const auto access = dynamic_cast<sgs::AccessExp*>(expr);
        const auto obj = transformExpr(access->getObject(), context, env);
        if (obj->getResType()->getLevel() != sgs_backend::Types::TUPLE_TYPE) {
            throw std::exception("access expression : Class type is expected");
        }
        const auto ct = dynamic_cast<sgs_backend::STupleType*>(obj->getResType());
        size_t i = 0;
        for (; i < ct->getTypes().size(); i++) {
            if (ct->getTypes()[i].first == access->getMember()) {
                break;
            }
        }
        if (i == ct->getTypes().size()) {
            throw std::exception((ct->getName() + " has no member " + access->getMember()).c_str());
        }
        return new sgs_backend::AccessExp(obj, access->getMember());
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
        // type check
        const auto proto = call->getFunction();
        if (proto->getParam().size() != call->getParam().size()) {
            throw std::exception("call expression : paramater number mismatch");
        }
        vector<sgs_backend::Expression*> exp;
        for (size_t i = 0; i < proto->getParam().size(); i++) {
            const auto param = transformExpr(call->getParam()[i], context, env);
            const auto t1 = transformType(proto->getParam()[i].first, context);
            const auto t2 = param->getResType();
            if (!sgs_backend::sameType(t1, t2)) {
                throw std::exception("call expression : parameter type mismatch");
            }
            exp.push_back(param);
        }
        string name = proto->getName();
        if (name == "print an int") name = "printNum";
        if (name == "print a number") name = "printFloat";
        if (name == "print a str") name = "printStr";
        if (name == "read an int") name = "readNum";
        if (name == "read a number") name = "readFloat";
        return new sgs_backend::ExpStmt(new sgs_backend::CallExp(name, exp, transformType(proto->getReturnType(), context)));
    }
    case sgs::ST_IF: {
        const auto ifs = dynamic_cast<sgs::IfStmt*>(stmt);
        auto cond = transformExpr(ifs->getCond(), context, env);
        if (cond->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE) {
            throw std::exception("unexpected type at if.cond");
        } 
        const auto ct = dynamic_cast<sgs_backend::SBasicType*>(cond->getResType());
        if (ct->getBasicType() != sgs_backend::BasicType::BOOLEAN) {
            cond = new sgs_backend::UniopExp(sgs_backend::NOT, new sgs_backend::UniopExp(sgs_backend::NOT, cond, context), context);}
        const auto pass = transformStmt(ifs->getTaken(), context, env);
        const auto fail = transformStmt(ifs->getUntaken(), context, env);
        return new sgs_backend::IfStmt(cond, pass, fail);
    }
    case sgs::ST_WHILE: {
        const auto whs = dynamic_cast<sgs::WhileStmt*>(stmt);
        auto cond = transformExpr(whs->getCondition(), context, env);
        if (cond->getResType()->getLevel() != sgs_backend::Types::BASIC_TYPE) {
            throw std::exception("unexpected type at while.cond");
        }
        const auto ct = dynamic_cast<sgs_backend::SBasicType*>(cond->getResType());
        if (ct->getBasicType() != sgs_backend::BasicType::BOOLEAN) {
            cond = new sgs_backend::UniopExp(sgs_backend::NOT, new sgs_backend::UniopExp(sgs_backend::NOT, cond, context), context);
        }
        const auto body = transformStmt(whs->getBody(), context, env);
        return new sgs_backend::WhileStmt(cond, dynamic_cast<sgs_backend::BlockStmt*>(body));
    }
    case sgs::ST_RETURN: {
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
        Env* new_env = new Env(env);
        for (auto i : block->getContent()) {
            if (i->astType == sgs::AT_VARDEF) {
                res.emplace_back(dynamic_cast<sgs_backend::Statement*>(transformAST(i, context, new_env)));
            } else {
                res.emplace_back(transformStmt(dynamic_cast<sgs::Statement*>(i), context, new_env));
            }
        }
        return new sgs_backend::BlockStmt(res);
    }
    default:
        throw std::exception("What the fuck??? at statement transform");
    }
}

sgs_backend::AST* transformAST(sgs::AST* ast, sgs_backend::Context& context, Env* env) {
    switch (ast->astType) {
    case sgs::AT_VARDEF: {
        const auto varDef = dynamic_cast<sgs::VarDef*>(ast);
        env->getBindings()[varDef->getName()] = transformType(varDef->getDecType(), context);
        if (env->getParent()) { // local variable
            return new sgs_backend::VarDefStmt(transformType(varDef->getDecType(), context), nullptr, varDef->getName());
        } else { // global variable
            return new sgs_backend::GlobalVarDef(varDef->getName(), transformType(varDef->getDecType(), context), nullptr);
        }
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
        auto retTy = transformType(proto->getReturnType(), context);
        if (!retTy) {
            retTy = context.getIntType();
        }
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
        Env* new_env = new Env(env);
        new_env->getBindings()["result"] = retTy;
        for (auto&& x : proto->getParam()) {
            new_env->getBindings()[x.second] = x.first;
        }
        const auto block = dynamic_cast<sgs_backend::BlockStmt*>(transformAST(func->getBody(), context, new_env));
        block->getContent().insert(block->getContent().begin(), new sgs_backend::VarDefStmt(retTy, nullptr, "result"));
        block->getContent().push_back(new sgs_backend::ReturnStmt(new sgs_backend::IdExp("result", retTy)));
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
    Env* globalEnv = new Env(nullptr);
    Env* mainEnv = new Env(globalEnv);
    vector<sgs_backend::Statement*> mainStmt;
    for (auto&& x : cont1) {
        switch (x->astType) {
        case sgs::AT_CLASS:
        case sgs::AT_PROTO:
        case sgs::AT_VARDEF:
        case sgs::AT_FUNC:
            content.push_back(transformAST(x, context, globalEnv));
            break;
        case sgs::AT_STMT:
        case sgs::AT_EXP:
            mainStmt.push_back(dynamic_cast<sgs_backend::Statement*>(transformAST(x, context, mainEnv)));
            break;
        default:
            throw std::exception("what the fuck?? : at transform");
        }
    }
    mainStmt.push_back(new sgs_backend::ReturnStmt(sgs_backend::getLiteral(0, context)));
    content.push_back(new sgs_backend::FuncDef(mainProto, new sgs_backend::BlockStmt(mainStmt)));
    return content;
}
