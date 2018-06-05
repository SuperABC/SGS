#include "ast.h"


sgs_backend::SType* sgs_backend::fromBasicType(BasicType tp, Context& context) {
	switch (tp) {
	case BasicType::INTEGER: return context.getIntType();
	case BasicType::FLOAT: return context.getFloatType();
	case BasicType::BOOLEAN: return context.getBoolType();
	case BasicType::CHAR: return context.getCharType();
	default:
		return nullptr;
	}
}

std::string sgs_backend::printType(SType* type, const string& name) {
	return name + " [label=\"" + typeToString(type) + "\"]\n";
}

std::string sgs_backend::printExpression(Expression* expr) {
	using std::to_string;
#define ADDR(PTR) to_string(size_t(PTR))
	const string addr = ADDR(expr);
	switch (expr->getExpType()) {
    case ET_UNIOP: {
        const auto temp = dynamic_cast<UniopExp*>(expr);
        string res;
        string op;
        // switch (temp->getOp()) {
        // case NOT: op = "not";
        // // case MINUS: op = "minus"
        // default: ;
        // }
        res += addr + " [label=\"Not\"]\n";
        res += addr + " -> " + ADDR(temp->getVal()) + " [label=\"val\"]\n";
        res += printExpression(temp->getVal());
        return res;
    }
	case ET_BINOP: {
		const auto temp = dynamic_cast<BinopExp*>(expr);
		string res;
		string op;
		switch (temp->getOp()) {
		case AND: op = "and"; break;
		case OR: op = "or"; break;
		case ADD: op = "add"; break;
		case SUB: op = "sub"; break;
		case MUL: op = "mul"; break;
		case DIV: op = "div"; break;
		case GT: op = "gt"; break;
		case LT: op = "lt"; break;
        case MOD: op = "mod"; break;
        case EQ: op = "eq"; break;
		default: op = "what the fuck?";
		}
		res += addr + " [label=\"BinaryOpExpression\"]\n";
		printCounter++;
		res += addr + " -> " + to_string(printCounter) + " [label=\"op\"]\n";
		res += to_string(printCounter) + " [label=\"" + op + "\"]\n";
		res += addr + " -> " + ADDR(temp->getLeft()) + " [label=\"lhs\"]\n";
		res += addr + " -> " + ADDR(temp->getRigth()) + " [label=\"rhs\"]\n";
		res += printExpression(temp->getLeft());
		res += printExpression(temp->getRigth());
		return res;
	}
	case ET_LITERAL: {
		const auto temp = dynamic_cast<LiteralExp*>(expr);
		string res;
		switch (temp->getBType()) {
		case BasicType::INTEGER: {
			const auto intlit = dynamic_cast<IntLiteral*>(temp);
			res += addr + " [label=\"IntLiteral:" + to_string(intlit->getValue()) + "\"]\n";
			break;
		}
		case BasicType::FLOAT:	{
			const auto floatlit = dynamic_cast<FloatLiteral*>(temp);
			res += addr + " [label=\"FloatLiteral:" + to_string(floatlit->getValue()) + "\"]\n";
			break;
		}
		case BasicType::BOOLEAN: {
			const auto boollit = dynamic_cast<BoolLiteral*>(temp);
			res += addr + " [label=\"BoolLiteral:" + (boollit->getValue() ? "True" : "False") + "\"]\n";
			break;
		}
		case BasicType::CHAR: {
			const auto charlit = dynamic_cast<CharLiteral*>(temp);
			res += addr + " [label=\"CharLiteral:" + to_string(charlit->getValue()) + "\"]\n";
			break;
		}
		default: ;
		}
		return res;
	}
	case ET_IDENT: {
		const auto ident = dynamic_cast<IdExp*>(expr);
		return addr + " [label=\"Id:" + ident->getName() + "\"]\n";
	}
	case ET_VISIT: {
		const auto visit = dynamic_cast<VisitExp*>(expr);
		string res;
		res += addr + " [label=\"VisitExpression\"]\n";
		res += addr + " -> " + ADDR(visit->getArray()) + " [label=\"array\"]\n";
		res += printExpression(visit->getArray());
		res += addr + " -> " + ADDR(visit->getIndex()) + " [label=\"index\"]\n";
		res += printExpression(visit->getIndex());
		return res;
 	}
	case ET_CALL: {
		const auto call = dynamic_cast<CallExp*>(expr);
		string res;
		res += addr + " [label=\"CallExpression\"]\n";
		printCounter++;
		res += addr + " -> " + to_string(printCounter) + " [label=\"function\"]\n";
		res += to_string(printCounter) + " [label=\"" + call->getFunction() + "\"]\n";
		for (size_t i = 0; i < call->getParam().size(); i++) {
			res += addr + " -> " + ADDR(call->getParam()[i]) + " [label=\"param " + to_string(i) + "\"]\n";
			res += printExpression(call->getParam()[i]);
		}
		return res;
	}
	case ET_ACCESS: {
		const auto access = dynamic_cast<AccessExp*>(expr);
		string res;
		res += addr + " [label=\"VisitExpression\"]\n";
		res += addr + " -> " + ADDR(access->getObject()) + " [label=\"object\"]\n";
		res += printExpression(access->getObject());
		printCounter++;
		res += addr + " -> " + to_string(printCounter) + " [label=\"member\"]\n";
		res += to_string(printCounter) + " [label=\"" + access->getMember() + "\"]\n";
		return res;
	}
	// case ET_UNIOP: break;
	case ET_CONSTR: {
		const auto constr = dynamic_cast<ConstString*>(expr);
		return addr + " [label=\"ConstString:" + constr->getStr() +"\"]\n";
	}
	default: 
		return "";
	}
#undef ADDR
}

std::string sgs_backend::printStatement(Statement* stmt) {
#define ADDR(PTR) to_string(size_t(PTR))
	using std::to_string;
	const string addr = ADDR(stmt);
	switch (stmt->getStmtType()) {
	case ST_ASSIGN: {
		const auto temp = dynamic_cast<AssignStmt*>(stmt);
		string res;
		res += addr + " [label=\"AssignStatement\"]\n";
		res += addr + " -> " + ADDR(temp->getLeft()) + " [label=\"lvalue\"]\n";
		res += addr + " -> " + ADDR(temp->getRigth()) + " [label=\"rvalue\"]\n";
		res += printExpression(temp->getLeft());
		res += printExpression(temp->getRigth());
		return res;
	}
	case ST_IF: {
		const auto temp = dynamic_cast<IfStmt*>(stmt);
		string res;
		res += addr + " [label=\"IfStatement\"]\n";
		res += addr + " -> " + ADDR(temp->getCond()) + " [label=\"cond\"]\n";
		res += addr + " -> " + ADDR(temp->getPass()) + " [label=\"pass\"]\n";
		res += addr + " -> " + ADDR(temp->getFail()) + " [label=\"fail\"]\n";
		res += printExpression(temp->getCond());
		res += printStatement(temp->getPass());
		res += printStatement(temp->getFail());
		return res; 
	}
	case ST_WHILE: {
		const auto temp = dynamic_cast<WhileStmt*>(stmt);
		string res;
		res += addr + " [label=\"WhileStatement\"]\n";
		res += addr + " -> " + ADDR(temp->getCond()) + " [label=\"cond\"]\n";
		res += addr + " -> " + ADDR(temp->getBody()) + " [label=\"body\"]\n";
		res += printExpression(temp->getCond());
		res += printStatement(temp->getBody());
		return res;
	}
	case ST_RETURN: {
		const auto temp = dynamic_cast<ReturnStmt*>(stmt);
		string res;
		res += addr + " [label=\"ReturnStatement\"]\n";
		res += addr + " -> " + ADDR(temp->getRetVal()) + " [label=\"retval\"]\n";
		res += printExpression(temp->getRetVal());
		return res;
	}
	case ST_BREAK: {
		return addr + " [label=\"BreakStatement\"]\n";
	}
	case ST_CONTINUE: {
		return addr + " [label=\"ContinueStatement\"]\n";
	}
	case ST_BLOCK: {
		const auto temp = dynamic_cast<BlockStmt*>(stmt);
		string res;
		res += addr + " [label=\"BlockStatement\"]\n";
		int counter = 0;
		for (auto&& x : temp->getContent()) {
			res += addr + " -> " + ADDR(x) + " [label=" + to_string(counter++) + "]\n";
		}
		for (auto&& x : temp->getContent()) {
			res += printStatement(x);
		}
		return res;
	}
	case ST_EXP: {
		const auto temp = dynamic_cast<ExpStmt*>(stmt);
		string res;
		res += addr + " [label=\"ExpStatement\"]\n";
		res += addr + " -> " + ADDR(temp->getExp()) + " [label=\"value\"]\n";
		res += printExpression(temp->getExp());
		return res;
	}
	case ST_VARDEF: {
		printCounter++;
		const auto temp = dynamic_cast<VarDefStmt*>(stmt);
		string res;
		res += addr + " [label=\"VarDefStatement\"]\n";
		res += addr + " -> " + to_string(printCounter) + " [label=\"name\"]\n";
		res += to_string(printCounter) + " [label=\"" + temp->getName() + "\"]\n";
		printCounter++;
		res += addr + " -> " + to_string(printCounter) + " [label=\"type\"]\n";
		res += printType(temp->getVarType(), to_string(printCounter));
		if (temp->getInitValue()) {
			res += addr + " -> " + ADDR(temp->getInitValue()) + " [label=\"initValue\"]\n";
			res += printExpression(temp->getInitValue());
		}
		return res;
	}
	default: 
		return "what the fuck?";
	}
#undef ADDR
}

std::string sgs_backend::printAST(AST* ast) {
	using std::to_string;
#define ADDR(PTR) to_string(size_t(PTR))
	const string addr = ADDR(ast);
	switch (ast->astType) {
	case AT_TYPEDEF: {
		const auto typeDef = dynamic_cast<TypeDef*>(ast);
		string res;
		res += addr + " [label=\"TypeDefinition\"]\n";
		printCounter++;
		res += addr + " -> " + to_string(printCounter) + " [label=\"type\"]\n";
		res += printType(typeDef->getDecType(), to_string(printCounter));
		return res;
	}
	case AT_GLBVARDEF: {
		const auto temp = dynamic_cast<GlobalVarDef*>(ast);
		string res;
		res += addr + " [label=\"GlobalVarDef\"]\n";
		printCounter++;
		res += addr + " -> " + to_string(printCounter) + " [label=\"name\"]\n";
		res += to_string(printCounter) + " [label=\"" + temp->getName() + "\"]\n";
		printCounter++;
		res += addr + " -> " + to_string(printCounter) + " [label=\"type\"]\n";
		res += printType(temp->getType(), to_string(printCounter));
		return res;
	}
	case AT_STMT: {
		const auto stmt = dynamic_cast<Statement*>(ast);
		return printStatement(stmt);
	}
	case AT_FUNC: {
		const auto func = dynamic_cast<FuncDef*>(ast);
		string res;
		res += addr + " [label=\"FunctionDefinition\"]\n";
		res += addr + " -> " + ADDR(func->getProto()) + " [label=\"proto\"]\n";
		res += addr + " -> " + ADDR(func->getBody()) + " [label=\"body\"]\n";
		res += printAST(func->getProto());
		res += printStatement(func->getBody());
		return res;
	}
	case AT_PROTO:
	{
		const auto proto = dynamic_cast<FuncProto*>(ast);
		string res;
		res += addr + " [label=\"FunctionPrototype\"]\n";
		printCounter++;
		res += addr + " -> " + to_string(printCounter) + " [label=\"name\"]\n";
		res += to_string(printCounter) + " [label=\"" + proto->getName() + "\"]\n";
		printCounter++;
		res += addr + " -> " + to_string(printCounter) + " [label=\"returnType\"]\n";
		res += printType(proto->getReturnType(), to_string(printCounter));
		for (const auto& i : proto->getParam()) {
			printCounter++;
			res += addr + " -> " + to_string(printCounter) + " [label=\"param:" + i.second + "\"]\n";
			res += printType(i.first, to_string(printCounter));
		}
		return res;
	}
	default: 
		return "";
	}
#undef ADDR
}

void sgs_backend::printContentInDot(const Content& content, const string& filename) {
	printCounter = 0;
    string res = "\
digraph g {\n\
node[shape = box, fontname = \"Fira Code Light\"]\n\
edge[fontname = \"Fira Code Light\", splines = line]\n\
\
";
	for (auto&& x : content) {
		res += printAST(x);
	}
	res += "}\n";
	std::ofstream fout(filename);
	fout << res << std::endl;
}

sgs_backend::SType* sgs_backend::getBinopType(BINOP op, SType* lhs, SType* rhs, Context& context) {
	// return lhs;
	switch (op) {
	case AND:
	case OR:
	case GT:
	case LT:
		return context.getBoolType();
	case ADD:
	case SUB:
	case MUL:
	case DIV: {
		assert(lhs->getLevel() == Types::BASIC_TYPE);
		assert(rhs->getLevel() == Types::BASIC_TYPE);
		const auto ls = dynamic_cast<SBasicType*>(lhs);
		const auto rs = dynamic_cast<SBasicType*>(rhs);
		if (ls->getBasicType() == BasicType::FLOAT) {
			return lhs;
		}
		if (ls->getBasicType() == BasicType::INTEGER || rs->getBasicType() == BasicType::INTEGER) {
			return context.getIntType();
		}
		if (ls->getBasicType() == BasicType::CHAR || rs->getBasicType() == BasicType::CHAR) {
			return context.getCharType();
		}
		return lhs;
	}
	default:
		return nullptr;
	}
}
