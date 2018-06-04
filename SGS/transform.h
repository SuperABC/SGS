#pragma once
#include "ast.h"
#include <vector>
#include "syntax.h"
#include "machine.h"
using std::vector;

class Env {
    Env* parent;
    std::map<string, sgs_backend::SType*> bindings;
public:
    Env(Env* parent) : parent(parent) {}

    sgs_backend::SType* find(const string& str) {
        Env* ptr = this;
        while (ptr) {
            if (ptr->getBindings().find(str) != ptr->getBindings().end()) {
                return ptr->getBindings()[str];
            }
            ptr = ptr->getParent();
        }
        return nullptr;
    }
    Env* getParent() const {
        return parent;
    }

    std::map<string, sgs_backend::SType*>& getBindings() {
        return bindings;
    }
};

sgs_backend::SType* transformType(sgs::VarType* type, sgs_backend::Context& context);

sgs_backend::Expression* transformExpr(sgs::Expression* expr, sgs_backend::Context& context,
                                       Env* env);

sgs_backend::Statement* transformStmt(sgs::Statement* stmt, sgs_backend::Context& context, Env* env);

sgs_backend::AST* transformAST(sgs::AST* ast, sgs_backend::Context& context, Env* env);

sgs_backend::Content transform(vector<sgs::AST*>& cont1, sgs_backend::Context& context);
