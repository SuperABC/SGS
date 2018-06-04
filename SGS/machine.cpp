#define _CRT_SECURE_NO_WARNINGS
#include "machine.h"

using namespace sgs;
ArrayNode::ArrayNode(VarType *t, int length, string n) :
    VarNode(new ArrayType(t, length), n), content(vector<VarNode *>(length)) {
    for (auto &e : content) {
        switch (t->getVarType()) {
        case VT_BASIC:
            switch (((BasicType *)t)->getBasicType()) {
            case BT_INT:
                e = new IntNode("");
                break;
            case BT_FLOAT:
                e = new FloatNode("");
                break;
            case BT_BOOL:
                e = new BoolNode("");
                break;
            case BT_STRING:
                e = new StrNode("");
                break;
            default:
                break;
            }
            break;
        case VT_ARRAY:
            e = new ArrayNode(((ArrayType *)t)->getEleType(), ((ArrayType *)t)->getLength(), "");
            break;
        case VT_CLASS:
            e = new ClassNode(((ClassType *)t)->getEle(), ((ClassType *)t)->getName(), "");
            break;
        default:
            break;
        }
    }
}
ClassNode::ClassNode(vector <std::pair<VarType *, string>> ele, string cn, string n) :
    VarNode(new ClassType(cn, ele), n), content(vector<VarNode *>(ele.size())) {
    for (auto e : content) {
        // TODO
    }
}

string nameReform(string input) {
    string ret;
    for (unsigned int i = 0; i < input.length(); i++) {
        if (input[i] != ' ')ret.push_back(input[i]);
        else {
            ret.push_back(input[++i] & ~0x20);
        }
    }
    return ret;
}

SgsMachine::SgsMachine() {
    initModule();
}
SgsMachine::~SgsMachine() {

}
void SgsMachine::initModule() {
    loadDlls();
}
void SgsMachine::loadDlls() {
    dllList.push_back(LoadLibrary("Function.dll"));
}
void SgsMachine::addSymbol(VarNode *var) {
    string name = var->name;
    int index = 0;
    for (auto c : name)index += c;
    index = index % 256;
    Symbol *symbol = new Symbol(var);
    symbol->next = table[index];
    table[index] = symbol;
}
VarNode *SgsMachine::findSymbol(string name) {
    int index = 0;
    for (auto c : name)index += c;
    index = index % 256;
    Symbol *tmp = table[index];
    while (tmp) {
        if (tmp->var->name == name) {
            return tmp->var;
        }
    }
    return nullptr;
}
void SgsMachine::removeLocal(string local, bool del) {
    int index = 0;
    for (auto c : local)index += c;
    index = index % 256;
    Symbol *tmp = table[index];
    Symbol *pre = nullptr;
    while (tmp) {
        if (tmp->var->name == local) {
            if (pre) {
                pre->next = tmp->next;
                if (del)delete tmp;
                break;
            } else {
                table[index] = tmp->next;
                if (del)delete tmp;
                break;
            }
        } else {
            pre = tmp;
            tmp = tmp->next;
        }
    }
}

SgsMachine *SgsMachine::input(vector<AST *> s,
    vector<ClassType *> c, vector<FuncProto *> f) {
    stmts = s;
    classList = c;
    for (auto d : f) {
        funcList.emplace_back(d, nullptr);
    }
    return this;
}
void SgsMachine::environment(void *env) {

}
void SgsMachine::execute() {
    for (auto s : stmts)step(s);
}
void SgsMachine::step(AST *s) {
    switch (s->astType) {
    case AT_VARDEF:
        declare(s);
        break;
    case AT_CLASS:
        structure(s);
        break;
    case AT_STMT:
        statement(s);
        break;
    case AT_PROTO:
        prototype(s);
        break;
    case AT_FUNC:
        definition(s);
        break;
    default:
        break;
    }
}
void SgsMachine::declare(AST *s) {
    VarDef *dec = (VarDef *)s;
    VarNode *tmp = nullptr;
    switch (dec->getDecType()->getVarType()) {
    case sgs::VT_BASIC:
        switch (((BasicType *)dec->getDecType())->getBasicType()) {
        case BT_INT:
            tmp = new IntNode(dec->getName());
            break;
        case BT_FLOAT:
            tmp = new FloatNode(dec->getName());
            break;
        case BT_BOOL:
            tmp = new BoolNode(dec->getName());
            break;
        case BT_STRING:
            tmp = new StrNode(dec->getName());
            break;
        case BT_CHAR: break;
        default:;
        }
        break;
    case sgs::VT_ARRAY:
        tmp = new ArrayNode(((ArrayType *)dec->getDecType())->getEleType(),
            ((ArrayType *)dec->getDecType())->getLength(), dec->getName());
        break;
    case sgs::VT_CLASS:
        tmp = new ClassNode(((ClassType *)dec->getDecType())->getEle(),
            ((ClassType *)dec->getDecType())->getName(), dec->getName());
        break;
    }
    addSymbol(tmp);
    stack.push(tmp->name);
}
void SgsMachine::structure(AST *s) {

}
void SgsMachine::statement(AST *s) { //suspend.
    Statement *stmt = (Statement *)s;
    switch (stmt->getStmtType()) {
    case ST_ASSIGN:
    {
        AssignStmt *assign = (AssignStmt *)stmt;
        assignValue(getPointer(assign->getLeft()), expValue(assign->getRight()));
        break;
    }
    case ST_CALL:
    {
        CallStmt *call = (CallStmt *)stmt;
        callFunc(call->getFunction(), call->getParam());
        break;
    }
    case ST_IF:
    {
        IfStmt *branch = (IfStmt *)stmt;
        if (getBool(expValue(branch->getCond()))) {
            exeBlock(branch->getTaken());
        } else {
            exeBlock(branch->getUntaken());
        }
        break;
    }
    case ST_WHILE:
    {
        WhileStmt *loop = (WhileStmt *)stmt;
        while (getBool(expValue(loop->getCondition()))) {
            exeBlock(loop->getBody());
        }
        break;
    }
    case ST_RETURN:
        break;
    case ST_BREAK:
        break;
    case ST_CONTINUE:
        break;
    default:
        break;
    }
}
void SgsMachine::prototype(AST *s) {

}
void SgsMachine::definition(AST *s) {
    FuncDef *def = (FuncDef *)s;
    for (auto &dec : funcList) {
        if (def->getProto()->getName() == dec.first->getName())
            dec.second = def;
    }
}

void SgsMachine::assignValue(VarNode *left, VarNode *right) {
    if (left == nullptr || right == nullptr)return;
    switch (left->type->getVarType()) {
    case sgs::VT_BASIC:
        switch (((BasicType *)left->type)->getBasicType()) {
        case BT_INT:
            if (((BasicType *)right->type)->getBasicType() == BT_INT)
                ((IntNode *)left)->value = (int)((IntNode *)right)->value;
            else if (((BasicType *)right->type)->getBasicType() == BT_FLOAT)
                ((IntNode *)left)->value = (int)((FloatNode *)right)->value;
            else if (((BasicType *)right->type)->getBasicType() == BT_BOOL)
                ((IntNode *)left)->value = (int)((BoolNode *)right)->value;
            else error("", VE_TYPEMISMATCH);
            break;
        case BT_FLOAT:
            if (((BasicType *)right->type)->getBasicType() == BT_INT)
                ((FloatNode *)left)->value = (float)((IntNode *)right)->value;
            else if (((BasicType *)right->type)->getBasicType() == BT_FLOAT)
                ((FloatNode *)left)->value = (float)((FloatNode *)right)->value;
            else if (((BasicType *)right->type)->getBasicType() == BT_BOOL)
                ((FloatNode *)left)->value = (float)((BoolNode *)right)->value;
            else error("", VE_TYPEMISMATCH);
            break;
        case BT_BOOL:
            if (((BasicType *)right->type)->getBasicType() == BT_INT)
                ((BoolNode *)left)->value = (bool)((IntNode *)right)->value;
            else if (((BasicType *)right->type)->getBasicType() == BT_FLOAT)
                ((BoolNode *)left)->value = (bool)((FloatNode *)right)->value;
            else if (((BasicType *)right->type)->getBasicType() == BT_BOOL)
                ((BoolNode *)left)->value = (bool)((BoolNode *)right)->value;
            else error("", VE_TYPEMISMATCH);
            break;
        case BT_STRING:
            if (((BasicType *)right->type)->getBasicType() == BT_INT) {
                char *buffer = (char *)malloc((int)log10(((IntNode *)right)->value) + 2);
                sprintf(buffer, "%d", ((IntNode *)right)->value);
                ((StrNode *)left)->value = buffer;
            } else if (((BasicType *)right->type)->getBasicType() == BT_FLOAT) {
                char *buffer = (char *)malloc((int)log10(((FloatNode *)right)->value) + 2);
                sprintf(buffer, "%f", ((FloatNode *)right)->value);
                ((StrNode *)left)->value = buffer;
            } else if (((BasicType *)right->type)->getBasicType() == BT_BOOL) {
                if (((BoolNode *)right)->value)((StrNode *)left)->value = "true";
                else ((StrNode *)left)->value = "false";
            } else if (((BasicType *)right->type)->getBasicType() == BT_STRING) {
                ((StrNode *)left)->value = ((StrNode *)right)->value;
            } else error("", VE_TYPEMISMATCH);
            break;
        case BT_CHAR: break;
        default:;
        }
        break;
    case sgs::VT_ARRAY:
        if (right->type->getVarType() == sgs::VT_ARRAY &&
            ((ArrayType *)left->type)->getEleType() == ((ArrayType *)right->type)->getEleType())
            ((ArrayNode *)left)->content = ((ArrayNode *)right)->content;
        else error("", VE_TYPEMISMATCH);
        break;
    case sgs::VT_CLASS:
        if (right->type->getVarType() == VT_CLASS &&
            ((ClassType *)left->type)->getName() == ((ClassType *)right->type)->getName())
            ((ClassNode *)left)->content = ((ClassNode *)right)->content;
        else error("", VE_TYPEMISMATCH);
        break;
    }
}
VarNode *SgsMachine::callFunc(FuncProto *proto, vector<Expression *> paras) {
    string name = proto->getName();
    SGSFUNC tmp;
    for (const auto& func : funcList) {
        if (name == func.first->getName()) {
            if (func.second) {
                addSymbol(new VarNode(func.first->getReturnType(), "result"));
                for (unsigned int i = 0; i < paras.size(); i++) {
                    VarNode *temp2 = new VarNode(
                        func.first->getParam()[i].first, func.first->getParam()[i].second);
                    assignValue(temp2, expValue(paras[i]));
                    addSymbol(temp2);
                }
                exeBlock(func.second->getBody());
                VarNode *res = findSymbol("result");
                removeLocal("result", false);
                return res;
            }
        }
    }
    for (auto dll : dllList) {
        if ((tmp = (SGSFUNC)GetProcAddress(dll, nameReform(name).data()))) {
            vector<VarNode *> list;
            for (auto exp : paras)list.push_back(expValue(exp));
            return tmp(list);
        }
    }
    return nullptr;
}
void SgsMachine::exeBlock(BlockStmt *block) {
    stack.push("");
    for (auto s : block->getContent())step(s);
    while (stack.top() != "") {
        removeLocal(stack.top());
        stack.pop();
    }
    stack.pop();
}
VarNode *SgsMachine::getPointer(Expression *e) {
    VarNode *ret;
    switch (e->getExpType()) {
    case ET_IDENT:
        ret = findSymbol(((IdExp *)e)->getName());
        if (ret)return ret;
        else {
            error(((IdExp *)e)->getName().data(), VE_NOID);
            return nullptr;
        }
    case ET_VISIT:
        return arrayElement(e);
    case ET_ACCESS:
        return classAttrib(e);
    default:
        return nullptr;
    }
}
VarNode *SgsMachine::expValue(Expression *e) {
    switch (e->getExpType()) {
    case ET_OP:
        return binCalc(((OpExp *)e)->getOp(), ((OpExp *)e)->getLeft(), ((OpExp *)e)->getRight());
    case ET_LITERAL:
        switch (((LiteralExp *)e)->getType()->getVarType()) {
        case sgs::VT_BASIC:
            switch (((BasicType *)((LiteralExp *)e)->getType())->getBasicType()) {
            case BT_INT:
                return new IntNode(((IntLiteral *)e)->getValue(), "");
            case BT_FLOAT:
                return new FloatNode(((FloatLiteral *)e)->getValue(), "");
            case BT_BOOL:
                return new BoolNode(((BoolLiteral *)e)->getValue(), "");
            case BT_STRING:
                return new StrNode(((StrLiteral *)e)->getValue().data(), "");
            default:
                return nullptr;
            }
        case sgs::VT_ARRAY:
        {
            ArrayNode *ret = new ArrayNode(((ArrayType *)((ArrayLiteral *)e)->getType())->getEleType(),
                ((ArrayType *)((ArrayLiteral *)e)->getType())->getLength(), "");
            for (unsigned int i = 0; i < ((ArrayLiteral *)e)->getValue().size(); i++) {
                ret->content[i] = expValue(((ArrayLiteral *)e)->getValue()[i]);
            }
            return ret;
        }
        case sgs::VT_CLASS:
        {
            ClassNode *ret = new ClassNode(((ClassType *)((ClassLiteral *)e)->getType())->getEle(),
                ((ClassType *)((ClassLiteral *)e)->getType())->getName(), "");
            for (unsigned int i = 0; i < ((ClassLiteral *)e)->getValue().size(); i++) {
                ret->content[i] = expValue(((ClassLiteral *)e)->getValue()[i]);
            }
            return ret;
        }
        default:
            return nullptr;
        }
    case ET_CALL:
        return callFunc(((CallExp *)e)->getFunction(), ((CallExp *)e)->getParam());
    case ET_IDENT:
        return findSymbol(((IdExp *)e)->getName());
    case ET_VISIT:
        return arrayElement(e);
    case ET_ACCESS:
        return classAttrib(e);
    default:
        return nullptr;
    }
}
VarNode *SgsMachine::binCalc(SGSOPERATOR op, Expression *a, Expression *b) {
    VarNode *v2 = expValue(a);
    VarNode *v1 = expValue(b);
    switch (op) {
    case SGS_OP_PLUS:
        if (v1->type->getVarType() == sgs::VT_BASIC &&
            v2->type->getVarType() == sgs::VT_BASIC) {
            if (((BasicType *)v1->type)->getBasicType() == BT_INT &&
                ((BasicType *)v2->type)->getBasicType() == BT_INT) {
                return new IntNode(((IntNode *)v1)->value + ((IntNode *)v2)->value, "");
            } else if (((BasicType *)v1->type)->getBasicType() == BT_FLOAT &&
                ((BasicType *)v2->type)->getBasicType() == BT_FLOAT) {
                return new FloatNode(((FloatNode *)v1)->value + ((FloatNode *)v2)->value, "");
            } else if (((BasicType *)v1->type)->getBasicType() == BT_STRING &&
                ((BasicType *)v2->type)->getBasicType() == BT_STRING) {
                return new StrNode((((StrNode *)v1)->value + ((StrNode *)v2)->value).data(), "");
            }
        }
        break;
    case SGS_OP_MINUS:
        if (v1->type->getVarType() == sgs::VT_BASIC &&
            v2->type->getVarType() == sgs::VT_BASIC) {
            if (((BasicType *)v1->type)->getBasicType() == BT_INT &&
                ((BasicType *)v2->type)->getBasicType() == BT_INT) {
                return new IntNode(((IntNode *)v1)->value - ((IntNode *)v2)->value, "");
            } else if (((BasicType *)v1->type)->getBasicType() == BT_FLOAT &&
                ((BasicType *)v2->type)->getBasicType() == BT_FLOAT) {
                return new FloatNode(((FloatNode *)v1)->value - ((FloatNode *)v2)->value, "");
            }
        }
        break;
    case SGS_OP_MULTY:
        if (v1->type->getVarType() == sgs::VT_BASIC &&
            v2->type->getVarType() == sgs::VT_BASIC) {
            if (((BasicType *)v1->type)->getBasicType() == BT_INT &&
                ((BasicType *)v2->type)->getBasicType() == BT_INT) {
                return new IntNode(((IntNode *)v1)->value * ((IntNode *)v2)->value, "");
            } else if (((BasicType *)v1->type)->getBasicType() == BT_FLOAT &&
                ((BasicType *)v2->type)->getBasicType() == BT_FLOAT) {
                return new FloatNode(((FloatNode *)v1)->value * ((FloatNode *)v2)->value, "");
            }
        }
        break;
    case SGS_OP_DIVIDE:
        if (v1->type->getVarType() == sgs::VT_BASIC &&
            v2->type->getVarType() == sgs::VT_BASIC) {
            if (((BasicType *)v1->type)->getBasicType() == BT_INT &&
                ((BasicType *)v2->type)->getBasicType() == BT_INT) {
                return new IntNode(((IntNode *)v1)->value / ((IntNode *)v2)->value, "");
            } else if (((BasicType *)v1->type)->getBasicType() == BT_FLOAT &&
                ((BasicType *)v2->type)->getBasicType() == BT_FLOAT) {
                return new FloatNode(((FloatNode *)v1)->value / ((FloatNode *)v2)->value, "");
            }
        }
        break;
    case SGS_OP_MOD:
        if (v1->type->getVarType() == sgs::VT_BASIC &&
            v2->type->getVarType() == sgs::VT_BASIC) {
            if (((BasicType *)v1->type)->getBasicType() == BT_INT &&
                ((BasicType *)v2->type)->getBasicType() == BT_INT) {
                return new IntNode(((IntNode *)v1)->value % ((IntNode *)v2)->value, "");
            }
        }
        break;
    case SGS_OP_AND:
        if (v1->type->getVarType() == sgs::VT_BASIC &&
            v2->type->getVarType() == sgs::VT_BASIC) {
            if (((BasicType *)v1->type)->getBasicType() == BT_INT &&
                ((BasicType *)v2->type)->getBasicType() == BT_INT) {
                return new IntNode(((IntNode *)v1)->value & ((IntNode *)v2)->value, "");
            } else if (((BasicType *)v1->type)->getBasicType() == BT_BOOL &&
                ((BasicType *)v2->type)->getBasicType() == BT_BOOL) {
                return new BoolNode(((BoolNode *)v1)->value & ((BoolNode *)v2)->value, "");
            }
        }
        break;
    case SGS_OP_ANDAND:
        if (v1->type->getVarType() == sgs::VT_BASIC &&
            v2->type->getVarType() == sgs::VT_BASIC) {
            if (((BasicType *)v1->type)->getBasicType() == BT_INT &&
                ((BasicType *)v2->type)->getBasicType() == BT_INT) {
                return new BoolNode(((IntNode *)v1)->value && ((IntNode *)v2)->value, "");
            } else if (((BasicType *)v1->type)->getBasicType() == BT_FLOAT &&
                ((BasicType *)v2->type)->getBasicType() == BT_FLOAT) {
                return new BoolNode(((FloatNode *)v1)->value && ((FloatNode *)v2)->value, "");
            } else if (((BasicType *)v1->type)->getBasicType() == BT_BOOL &&
                ((BasicType *)v2->type)->getBasicType() == BT_BOOL) {
                return new BoolNode(((BoolNode *)v1)->value && ((BoolNode *)v2)->value, "");
            }
        }
        break;
    case SGS_OP_OR:
        if (v1->type->getVarType() == sgs::VT_BASIC &&
            v2->type->getVarType() == sgs::VT_BASIC) {
            if (((BasicType *)v1->type)->getBasicType() == BT_INT &&
                ((BasicType *)v2->type)->getBasicType() == BT_INT) {
                return new IntNode(((IntNode *)v1)->value | ((IntNode *)v2)->value, "");
            } else if (((BasicType *)v1->type)->getBasicType() == BT_BOOL &&
                ((BasicType *)v2->type)->getBasicType() == BT_BOOL) {
                return new BoolNode(((BoolNode *)v1)->value | ((BoolNode *)v2)->value, "");
            }
        }
        break;
    case SGS_OP_OROR:
        if (v1->type->getVarType() == sgs::VT_BASIC &&
            v2->type->getVarType() == sgs::VT_BASIC) {
            if (((BasicType *)v1->type)->getBasicType() == BT_INT &&
                ((BasicType *)v2->type)->getBasicType() == BT_INT) {
                return new BoolNode(((IntNode *)v1)->value || ((IntNode *)v2)->value, "");
            } else if (((BasicType *)v1->type)->getBasicType() == BT_FLOAT &&
                ((BasicType *)v2->type)->getBasicType() == BT_FLOAT) {
                return new BoolNode(((FloatNode *)v1)->value || ((FloatNode *)v2)->value, "");
            } else if (((BasicType *)v1->type)->getBasicType() == BT_BOOL &&
                ((BasicType *)v2->type)->getBasicType() == BT_BOOL) {
                return new BoolNode(((BoolNode *)v1)->value || ((BoolNode *)v2)->value, "");
            }
        }
        break;
    case SGS_OP_NOR:
        if (v1->type->getVarType() == sgs::VT_BASIC &&
            v2->type->getVarType() == sgs::VT_BASIC) {
            if (((BasicType *)v1->type)->getBasicType() == BT_INT &&
                ((BasicType *)v2->type)->getBasicType() == BT_INT) {
                return new IntNode(((IntNode *)v1)->value ^ ((IntNode *)v2)->value, "");
            } else if (((BasicType *)v1->type)->getBasicType() == BT_BOOL &&
                ((BasicType *)v2->type)->getBasicType() == BT_BOOL) {
                return new BoolNode(((BoolNode *)v1)->value ^ ((BoolNode *)v2)->value, "");
            }
        }
        break;
    case SGS_OP_SMALLER:
        if (v1->type->getVarType() == sgs::VT_BASIC &&
            v2->type->getVarType() == sgs::VT_BASIC) {
            if (((BasicType *)v1->type)->getBasicType() == BT_INT &&
                ((BasicType *)v2->type)->getBasicType() == BT_INT) {
                return new BoolNode(((IntNode *)v1)->value < ((IntNode *)v2)->value, "");
            } else if (((BasicType *)v1->type)->getBasicType() == BT_FLOAT &&
                ((BasicType *)v2->type)->getBasicType() == BT_FLOAT) {
                return new BoolNode(((FloatNode *)v1)->value < ((FloatNode *)v2)->value, "");
            }
        }
        break;
    case SGS_OP_NSMALLER:
        if (v1->type->getVarType() == sgs::VT_BASIC &&
            v2->type->getVarType() == sgs::VT_BASIC) {
            if (((BasicType *)v1->type)->getBasicType() == BT_INT &&
                ((BasicType *)v2->type)->getBasicType() == BT_INT) {
                return new BoolNode(((IntNode *)v1)->value >= ((IntNode *)v2)->value, "");
            } else if (((BasicType *)v1->type)->getBasicType() == BT_FLOAT &&
                ((BasicType *)v2->type)->getBasicType() == BT_FLOAT) {
                return new BoolNode(((FloatNode *)v1)->value >= ((FloatNode *)v2)->value, "");
            }
        }
        break;
    case SGS_OP_GREATER:
        if (v1->type->getVarType() == sgs::VT_BASIC &&
            v2->type->getVarType() == sgs::VT_BASIC) {
            if (((BasicType *)v1->type)->getBasicType() == BT_INT &&
                ((BasicType *)v2->type)->getBasicType() == BT_INT) {
                return new BoolNode(((IntNode *)v1)->value > ((IntNode *)v2)->value, "");
            } else if (((BasicType *)v1->type)->getBasicType() == BT_FLOAT &&
                ((BasicType *)v2->type)->getBasicType() == BT_FLOAT) {
                return new BoolNode(((FloatNode *)v1)->value > ((FloatNode *)v2)->value, "");
            }
        }
        break;
    case SGS_OP_NGREATER:
        if (v1->type->getVarType() == sgs::VT_BASIC &&
            v2->type->getVarType() == sgs::VT_BASIC) {
            if (((BasicType *)v1->type)->getBasicType() == BT_INT &&
                ((BasicType *)v2->type)->getBasicType() == BT_INT) {
                return new BoolNode(((IntNode *)v1)->value <= ((IntNode *)v2)->value, "");
            } else if (((BasicType *)v1->type)->getBasicType() == BT_FLOAT &&
                ((BasicType *)v2->type)->getBasicType() == BT_FLOAT) {
                return new BoolNode(((FloatNode *)v1)->value <= ((FloatNode *)v2)->value, "");
            }
        }
        break;
    case SGS_OP_NOTEQ:
        if (v1->type->getVarType() == sgs::VT_BASIC &&
            v2->type->getVarType() == sgs::VT_BASIC) {
            if (((BasicType *)v1->type)->getBasicType() == BT_INT &&
                ((BasicType *)v2->type)->getBasicType() == BT_INT) {
                return new BoolNode(((IntNode *)v1)->value != ((IntNode *)v2)->value, "");
            } else if (((BasicType *)v1->type)->getBasicType() == BT_FLOAT &&
                ((BasicType *)v2->type)->getBasicType() == BT_FLOAT) {
                return new BoolNode(((FloatNode *)v1)->value != ((FloatNode *)v2)->value, "");
            } else if (((BasicType *)v1->type)->getBasicType() == BT_BOOL &&
                ((BasicType *)v2->type)->getBasicType() == BT_BOOL) {
                return new BoolNode(((BoolNode *)v1)->value != ((BoolNode *)v2)->value, "");
            }
        }
        break;
    case SGS_OP_EQUAL:
        if (v1->type->getVarType() == sgs::VT_BASIC &&
            v2->type->getVarType() == sgs::VT_BASIC) {
            if (((BasicType *)v1->type)->getBasicType() == BT_INT &&
                ((BasicType *)v2->type)->getBasicType() == BT_INT) {
                return new BoolNode(((IntNode *)v1)->value == ((IntNode *)v2)->value, "");
            } else if (((BasicType *)v1->type)->getBasicType() == BT_FLOAT &&
                ((BasicType *)v2->type)->getBasicType() == BT_FLOAT) {
                return new BoolNode(((FloatNode *)v1)->value == ((FloatNode *)v2)->value, "");
            } else if (((BasicType *)v1->type)->getBasicType() == BT_BOOL &&
                ((BasicType *)v2->type)->getBasicType() == BT_BOOL) {
                return new BoolNode(((BoolNode *)v1)->value == ((BoolNode *)v2)->value, "");
            }
        }
        break;
    default:
        break;
    }
    return nullptr;
}
VarNode *SgsMachine::arrayElement(Expression *e) {
    return ((ArrayNode *)expValue(((VisitExp *)e)->getArray()))->content[
        ((IntNode *)expValue(((VisitExp *)e)->getIndex()))->value];
}
VarNode *SgsMachine::classAttrib(Expression *e) {
    return ((ClassNode *)expValue(((AccessExp *)e)->getObject()))->operator[](
        ((AccessExp *)e)->getMember());
}
VarType *SgsMachine::checkExp(sgs::Expression *e) {
    return nullptr;
}

int SgsMachine::getInt(VarNode *val) {
    return ((IntNode *)val)->value;
}
float SgsMachine::getFloat(VarNode *val) {
    return ((FloatNode *)val)->value;
}
bool SgsMachine::getBool(VarNode *val) {
    return ((BoolNode *)val)->value;
}
const char *SgsMachine::getStr(VarNode *val) {
    return ((StrNode *)val)->value.data();
}

void SgsMachine::clearMem() {
    macMem.clear();
}
void SgsMachine::error(const char *inst, SGSVMERROR type) {
    switch (type) {
    case VE_DIVBYZERO:
        msgList.emplace_back(string("Division by zero"), MT_ERROR);
        break;
    case VE_NOID:
        msgList.emplace_back(string("Can't find ") + inst, MT_ERROR);
        break;
    case VE_TYPEMISMATCH:
        msgList.emplace_back(string("Can't have type casting."), MT_ERROR);
        break;
    case VE_BROKEN:
        msgList.emplace_back(string("Virtual machine is broken."), MT_ERROR);
        break;
    }
}