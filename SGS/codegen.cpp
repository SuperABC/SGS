#include "codegen.h"
#include <iostream>

void sgs_backend::builtinFuncInit() {
	// intToChar :: Int => Char
	FunctionType* intToChar = FunctionType::get(Type::getInt8Ty(theContext), {Type::getInt32Ty(theContext)}, false);
	funcReference["intToChar"] = Function::Create(intToChar, GlobalValue::ExternalLinkage, "intToChar");

	// charToInt :: Char => Int
	FunctionType* charToInt = FunctionType::get(Type::getInt32Ty(theContext), {Type::getInt8Ty(theContext)}, false);
	funcReference["charToInt"] = Function::Create(charToInt, GlobalValue::ExternalLinkage, "charToInt");

	// intToFloat :: Int => Float
	FunctionType* intToFloat = FunctionType::get(Type::getFloatTy(theContext), {Type::getInt32Ty(theContext)}, false);
	funcReference["intToFloat"] = Function::Create(intToFloat, GlobalValue::ExternalLinkage, "intToFloat");

	// floatToInt :: Float => Int
	FunctionType* floatToInt = FunctionType::get(Type::getInt32Ty(theContext), {Type::getFloatTy(theContext)}, false);
	funcReference["floatToInt"] = Function::Create(floatToInt, GlobalValue::ExternalLinkage, "floatToInt");

	// intToChar :: Int => Char
	FunctionType* intToBool = FunctionType::get(Type::getInt1Ty(theContext), {Type::getInt32Ty(theContext)}, false);
	funcReference["intToBool"] = Function::Create(intToBool, GlobalValue::ExternalLinkage, "intToBool");

	// charToInt :: Char => Int
	FunctionType* boolToInt = FunctionType::get(Type::getInt32Ty(theContext), {Type::getInt1Ty(theContext)}, false);
	funcReference["boolToInt"] = Function::Create(boolToInt, GlobalValue::ExternalLinkage, "boolToInt");

	// printNum :: Int => ()
	FunctionType* printNum = FunctionType::get(Type::getInt32Ty(theContext), {Type::getInt32Ty(theContext)}, false);
	funcReference["printNum"] = Function::Create(printNum, GlobalValue::ExternalLinkage, "printNum");

	// printFloat :: Float => ()
	FunctionType* printFloat = FunctionType::get(Type::getInt32Ty(theContext), { Type::getFloatTy(theContext) }, false);
	funcReference["printFloat"] = Function::Create(printFloat, GlobalValue::ExternalLinkage, "printFloat");

	// readNum :: () => Int
	FunctionType* readNum = FunctionType::get(Type::getInt32Ty(theContext), false);
	funcReference["readNum"] = Function::Create(readNum, GlobalValue::ExternalLinkage, "readNum");

	// printStr :: String => ()
	FunctionType* printStr = FunctionType::get(Type::getInt32Ty(theContext), {Type::getInt8PtrTy(theContext)}, false);
	funcReference["printStr"] = Function::Create(printStr, GlobalValue::ExternalLinkage, "printStr");

	// readStr :: String => ()
	FunctionType* readStr = FunctionType::get(Type::getInt32Ty(theContext), {Type::getInt8PtrTy(theContext)}, false);
	funcReference["readStr"] = Function::Create(readStr, GlobalValue::ExternalLinkage, "readStr");

	FunctionType* newline = FunctionType::get(Type::getInt32Ty(theContext), {}, false);
	funcReference["newline"] = Function::Create(newline, GlobalValue::ExternalLinkage, "newline");

	FunctionType* strcpy = FunctionType::get(Type::getInt8PtrTy(theContext), { Type::getInt8PtrTy(theContext) , Type::getInt8PtrTy(theContext) }, false);
	funcReference["strcpy"] = Function::Create(strcpy, GlobalValue::ExternalLinkage, "strcpy");

	FunctionType* getchar = FunctionType::get(Type::getInt32Ty(theContext), {}, false);
	funcReference["getchar"] = Function::Create(getchar, GlobalValue::ExternalLinkage, "getchar");

	FunctionType* putchar = FunctionType::get(Type::getInt32Ty(theContext), { Type::getInt32Ty(theContext) }, false);
	funcReference["putchar"] = Function::Create(putchar, GlobalValue::ExternalLinkage, "putchar");

	builtInFuncs = "\n\
@printNum.constStr = constant [3 x i8] c\"%d\\00\", align 1\n\
@printStr.constStr = constant[3 x i8] c\"%s\\00\", align 1\n\
@printFloat.constStr = constant[3 x i8] c\"%f\\00\", align 1\n\
declare i32 @printf(i8*, ...)\n\
declare i32 @scanf(i8*, ...)\n\
declare i32 @putchar(i32)\n\
declare i32 @getchar()\n\
declare i8* @strcpy(i8*, i8*)\n\
\n\
define i8 @intToChar(i32) {\n\
	%2 = trunc i32 %0 to i8\n\
	ret i8 %2\n\
}\n\
\n\
define i32 @charToInt(i8) {\n\
	%2 = alloca i8, align 1\n\
	store i8 %0, i8* %2, align 1\n\
	%3 = load i8, i8* %2, align 1\n\
	%4 = sext i8 %3 to i32\n\
	ret i32 %4\n\
}\n\
\n\
define i1 @intToBool(i32) {\n\
	%2 = icmp eq i32 %0, 0\n\
	ret i1 %2\n\
}\n\
\n\
define i32 @boolToInt(i1) {\n\
	%2 = sext i1 %0 to i32\n\
	ret i32 %2\n\
}\n\
\n\
define float @intToFloat(i32) {\n\
	%2 = sitofp i32 %0 to float\n\
	ret float %2\n\
}\n\
\n\
define i32 @floatToInt(float) {\n\
	%2 = fptosi float %0 to i32\n\
	ret i32 %2\n\
}\n\
\n\
define i32 @printNum(i32) {\n\
	%2 = call i32(i8*, ...) @printf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printNum.constStr, i32 0, i32 0), i32 %0)\n\
	ret i32 0\n\
}\n\
define i32 @printFloat(float) {\n\
	%2 = fpext float %0 to double\n\
	%3 = call i32(i8*, ...) @printf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printFloat.constStr, i32 0, i32 0), double %2)\n\
	ret i32 0\n\
}\n\
\n\
define i32 @printStr(i8*) {\n\
	%2 = call i32(i8*, ...) @printf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printStr.constStr, i32 0, i32 0), i8* %0)\n\
	ret i32 0\n\
}\n\
\n\
define i32 @readNum() {\n\
	%1 = alloca i32, align 4\n\
	%2 = call i32(i8*, ...) @scanf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printNum.constStr, i32 0, i32 0), i32* %1)\n\
	%3 = load i32, i32* %1, align 4\n\
	ret i32 %3\n\
}\n\
define i32 @newline() {\n\
	call i32 @putchar(i32 10)\n\
	ret i32 0\n\
}\n\
\n\
define i32 @readStr(i8*)  {\n\
	%2 = call i32(i8*, ...) @scanf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printStr.constStr, i32 0, i32 0), i8* %0)\n\
	ret i32 0\n\
}\n";
}

void sgs_backend::codegenInit() {
	typeReference.clear();
	funcReference.clear();
	delete theModule;
	globalEnv = Environment::derive(nullptr);
	theModule = new Module("wtf", theContext);
}

Value* sgs_backend::exprCodegen(Expression* exp, Environment* env) {
	switch (exp->getExpType()) {
	case ET_BINOP: {
		const auto bexp = dynamic_cast<BinopExp*>(exp);
		Value* lhs = exprCodegen(bexp->getLeft(), env);
		Value* rhs = exprCodegen(bexp->getRigth(), env);
		if (!lhs || !rhs) {
			std::cerr << "Translation Error : when translating <binop-expr> " << std::endl;
			return nullptr;
		}
		if (lhs->getType()->isPointerTy()) {
			lhs = builder.CreateLoad(lhs, "load.val");
		}
		if (rhs->getType()->isPointerTy()) {
			rhs = builder.CreateLoad(rhs, "load.val");
		}
		switch (bexp->getOp()) {
		case AND:
			return builder.CreateAnd(lhs, rhs, "and.res");
		case OR:
			return builder.CreateOr(lhs, rhs, "or.res");
		case ADD:
			if (lhs->getType()->isFloatTy() && rhs->getType()->isFloatTy()) {
				return builder.CreateFAdd(lhs, rhs, "fadd.res");
			}
			if (lhs->getType()->isIntegerTy()) {
                integerTypeExtension(lhs, rhs);
				return builder.CreateAdd(lhs, rhs, "add.res");
			}
			return nullptr;
		case SUB:
			if (lhs->getType()->isFloatTy()) {
				return builder.CreateFSub(lhs, rhs, "fsub.res");
			}
			if (lhs->getType()->isIntegerTy()) {
                integerTypeExtension(lhs, rhs);
				return builder.CreateSub(lhs, rhs, "sub.res");
			}
			return nullptr;
		case MUL:
			if (lhs->getType()->isFloatTy() && rhs->getType()->isFloatTy()) {
				return builder.CreateFMul(lhs, rhs, "fmul.res");
			}
			if (lhs->getType()->isIntegerTy()) {
                integerTypeExtension(lhs, rhs);
                return builder.CreateMul(lhs, rhs, "mul.res");
			}
			return nullptr;
		case DIV:
			if (lhs->getType()->isFloatTy() && rhs->getType()->isFloatTy()) {
				return builder.CreateFDiv(lhs, rhs, "fdiv.res");
			}
			if (lhs->getType()->isIntegerTy()) {
                integerTypeExtension(lhs, rhs);
                return builder.CreateExactSDiv(lhs, rhs, "div.res");
			}
			return nullptr;
		case GT:
			if (lhs->getType()->isFloatTy() && rhs->getType()->isFloatTy()) {
				return builder.CreateFCmp(CmpInst::Predicate::FCMP_OGT, lhs, rhs, "fgt.res");
			}
			if (lhs->getType()->isIntegerTy()) {
                integerTypeExtension(lhs, rhs);
                return builder.CreateICmp(CmpInst::Predicate::ICMP_SGT, lhs, rhs, "gt.res");
			}
			return nullptr;
		case LT:
			if (lhs->getType()->isFloatTy() && rhs->getType()->isFloatTy()) {
				return builder.CreateFCmp(CmpInst::Predicate::FCMP_OLT, lhs, rhs, "flt.res");
			}
			if (lhs->getType()->isIntegerTy()) {
                integerTypeExtension(lhs, rhs);
                return builder.CreateICmp(CmpInst::Predicate::ICMP_SLT, lhs, rhs, "lt.res");
			}
			return nullptr;
		default:
			std::cerr << "wtf ??" << std::endl;
		}
	}
	case ET_LITERAL: {
		const auto lit = dynamic_cast<LiteralExp*>(exp);
		switch (lit->getBType()) {
		case BasicType::INTEGER:
		{
			const auto intlit = dynamic_cast<IntLiteral*>(lit);
			return Constant::getIntegerValue(lit->getResType()->toLLVMType(theContext, typeReference), APInt(32, intlit->getValue()));
		}
		case BasicType::FLOAT:
		{
			const auto ftlit = dynamic_cast<FloatLiteral*>(lit);
			return ConstantFP::get(theContext, APFloat(ftlit->getValue()));
		}
		case BasicType::BOOLEAN:
		{
			const auto boolit = dynamic_cast<BoolLiteral*>(lit);
			return Constant::getIntegerValue(lit->getResType()->toLLVMType(theContext, typeReference), APInt(1, boolit->getValue()));
		}
		case BasicType::CHAR: {
			const auto charlit = dynamic_cast<CharLiteral*>(lit);
			return Constant::getIntegerValue(lit->getResType()->toLLVMType(theContext, typeReference), APInt(8, charlit->getValue()));
		}
		default:
			std::cerr << "wtf ???" << std::endl;
		}
	}
	case ET_IDENT: {
		const auto idexp = dynamic_cast<IdExp*>(exp);
		Value* id = env->find(idexp->getName());
		return id;
	}
	case ET_VISIT: {
		const auto visitexp = dynamic_cast<VisitExp*>(exp);
		Value* array = exprCodegen(visitexp->getArray(), env); // int **
		Value* index = exprCodegen(visitexp->getIndex(), env);
		if (index->getType()->isPointerTy()) {
			index = builder.CreateLoad(index, "load");
		}
		if (index->getType()->isIntegerTy(32)) {
			array = builder.CreateLoad(array, "array.load");
			return builder.CreateInBoundsGEP(array, {index}, "visit");
		}
		return nullptr;
	}  
	case ET_CALL: {
		const auto callexp = dynamic_cast<CallExp*>(exp);
		Function* fun = funcReference[callexp->getFunction()];
		Type* funTy1 = dyn_cast<PointerType>(fun->getType())->getElementType();
	    auto* funTy = dyn_cast<FunctionType>(funTy1);
		vector<Value*> params;
		int counter = 0;
		for (auto&& x : callexp->getParam()) {
			auto temp = exprCodegen(x, env);
			if (temp->getType()->isPointerTy() && x->getResType()->getLevel() != Types::TUPLE_TYPE) {
				temp = builder.CreateLoad(temp, "load");
			}
			if (funTy->getParamType(counter)->isIntegerTy() &&
				temp->getType()->isIntegerTy()) {
				const auto lt = dyn_cast<IntegerType>(funTy->getParamType(counter));
				const auto rt = dyn_cast<IntegerType>(temp->getType());
				if (lt->getBitWidth() < rt->getBitWidth()) {
					temp = builder.CreateTrunc(temp, lt, "param.trunc");
				} else if (lt->getBitWidth() > rt->getBitWidth()) {
					temp = builder.CreateSExt(temp, lt, "param.sext");
				}
			}
            counter++;
			params.push_back(temp);
		}
		return builder.CreateCall(fun, params, "call.res");
	}
	case ET_ACCESS: {
		const auto accexp = dynamic_cast<AccessExp*>(exp);
		const string name = accexp->getMember();
		Value* obj = exprCodegen(accexp->getObject(), env);
		SType* ty = accexp->getObject()->getResType();
		auto* tp = dynamic_cast<STupleType*>(ty);
		size_t i = 0;
		for (; i < tp->getTypes().size(); i++) {
			if (tp->getTypes()[i].first == name) {
				break;
			}
		}
		auto* resType = dynamic_cast<STupleType*>(tp)->getElemType(name)->toLLVMType(theContext, typeReference);
		if (i == tp->getTypes().size()) {
			std::cerr << "Can't find member " << name << std::endl;
			return nullptr;
		}
		if (obj->getType()->isPointerTy()) {
			if (obj->getType()->getPointerElementType()->isPointerTy()) {
				obj = builder.CreateLoad(obj, "access.load");
			}
		}
		if (resType->isArrayTy()) { 
			// we shall deal with array separately
			/** A simple solution to the problem of implicit casting between array and pointer :
			 *  struct A {int b[10];}
			 *  struct A a;
			 *  
			 *  what if we visit the member b of A ?
			 *  Its type will be int[10], which is hard to fit in our type and sematic system,
			 *  so we declare another pointer here, to replace the usage of b
			 *  
			 *  a.b[0] = 1; 
			 *  ==>
			 *  int* t = a.b;
			 *  t[0] = 1;
			 */
		    auto* aryTy = dyn_cast<ArrayType>(resType);
			IRBuilder<> tempBuilder(&builder.GetInsertBlock()->getParent()->getEntryBlock(), builder.GetInsertBlock()->getParent()->getEntryBlock().begin());
			Value* mid = tempBuilder.CreateAlloca(aryTy->getElementType()->getPointerTo(0), nullptr, "array.temp");
			Value* res = builder.CreateInBoundsGEP(obj, {
				Constant::getIntegerValue(Type::getInt32Ty(theContext), APInt(32, 0)),
				Constant::getIntegerValue(Type::getInt32Ty(theContext), APInt(32, i))
				}, "access.res"); // res is an array pointer
			res = builder.CreateInBoundsGEP(res, { Constant::getIntegerValue(Type::getInt32Ty(theContext), APInt(32, 0)),
				Constant::getIntegerValue(Type::getInt32Ty(theContext), APInt(32, 0)) });
			builder.CreateStore(res, mid);
			return mid;
		} else {
			return builder.CreateInBoundsGEP(obj, {
				Constant::getIntegerValue(Type::getInt32Ty(theContext), APInt(32, 0)),
				Constant::getIntegerValue(Type::getInt32Ty(theContext), APInt(32, i))
				}, "access.res");
		}
	}
	default:
		std::cerr << "What the fuck??" << std::endl;
		return nullptr;
	}
}

Value* sgs_backend::stmtCodegen(Statement* stmt, Environment* env, BasicBlock* cont, BasicBlock* bk) {
	switch (stmt->getStmtType()) {
	case ST_ASSIGN:
	{
		const auto ass = dynamic_cast<AssignStmt*>(stmt);

		if (ass->getRigth()->getExpType() == ET_CONSTR) { // deal with string assignment seperately
			Value* lhs = exprCodegen(ass->getLeft(), env);
			string conStr = dynamic_cast<ConstString*>(ass->getRigth())->getStr();
			Type* strType = ArrayType::get(Type::getInt8Ty(theContext), conStr.length() + 1);
			Constant* constStr = ConstantDataArray::getString(theContext, conStr);
			GlobalVariable* constStrv = new GlobalVariable(
				*theModule,
				strType,
				true,
				GlobalValue::PrivateLinkage,
				constStr,
				conStr + ".str"
				);
			Value* strPtr = builder.CreateLoad(lhs, "str.ptr");
			Value* conStrPtr = builder.CreateInBoundsGEP(constStrv,
				{ Constant::getIntegerValue(Type::getInt32Ty(theContext), APInt(32, 0)) ,Constant::getIntegerValue(Type::getInt32Ty(theContext), APInt(32, 0)) },
				"const.str.ptr");
			return builder.CreateCall(funcReference["strcpy"], { strPtr, conStrPtr });
		}
		
		Value* lhs = exprCodegen(ass->getLeft(), env);
		Value* rhs = exprCodegen(ass->getRigth(), env);
		if (rhs->getType()->isPointerTy()) {
			rhs = builder.CreateLoad(rhs, "load.val");
		}
		if (!lhs || !rhs) {
			std::cerr << "Translation error : at AssignStmt" << std::endl;
			return nullptr;
		}
		Type* type = dyn_cast<PointerType>(lhs->getType())->getElementType();
		if (type->isIntegerTy() && rhs->getType()->isIntegerTy()) {
		    auto* tr = dyn_cast<IntegerType>(rhs->getType());
		    auto* tl = dyn_cast<IntegerType>(type);
			if (tr->getBitWidth() < tl->getBitWidth()) { // need sext
				rhs = builder.CreateSExt(rhs, tl, "sext.temp");
			} else if (tr->getBitWidth() > tl->getBitWidth()) { // need trunc
				rhs = builder.CreateTrunc(rhs, tl, "trunc.temp");
			}
			return builder.CreateStore(rhs, lhs);
		}
		if (type == rhs->getType()) {
			// ptrType
			return builder.CreateStore(rhs, lhs);
		}
		return nullptr;
	}
	case ST_IF: {
		Function* fun = builder.GetInsertBlock()->getParent();
		const auto ifs = dynamic_cast<IfStmt*>(stmt);
		Value* cond = exprCodegen(ifs->getCond(), env);
		if (!cond) {
			std::cerr << "Translation error : at IfStmt, condition translation is failed" << std::endl;
			return nullptr;
		}
		if (cond->getType()->isPointerTy()) {
			cond = builder.CreateLoad(cond, "if.cond.load");
		}
		BasicBlock* taken = BasicBlock::Create(theContext, "if.take", fun);
		BasicBlock* untaken = BasicBlock::Create(theContext, "if.fail");
		BasicBlock* merge = BasicBlock::Create(theContext, "if.merge");
		builder.CreateCondBr(cond, taken, untaken);
		builder.SetInsertPoint(taken);
		stmtCodegen(ifs->getPass(), env, cont, bk);
		builder.CreateBr(merge);

		fun->getBasicBlockList().push_back(untaken);
		builder.SetInsertPoint(untaken);
		stmtCodegen(ifs->getFail(), env, cont, bk);
		const auto res = builder.CreateBr(merge);

		fun->getBasicBlockList().push_back(merge);
		builder.SetInsertPoint(merge);

		return res;
	}
	case ST_WHILE: {
		const auto wstmt = dynamic_cast<WhileStmt*>(stmt);
		Function* fun = builder.GetInsertBlock()->getParent();
		BasicBlock* whileCond = BasicBlock::Create(theContext, "while.cond");
		BasicBlock* whileBody = BasicBlock::Create(theContext, "while.body");
		BasicBlock* whileMerge = BasicBlock::Create(theContext, "while.merge");
		builder.CreateBr(whileCond);
		fun->getBasicBlockList().push_back(whileCond);
		builder.SetInsertPoint(whileCond);
		Value* cond = exprCodegen(wstmt->getCond(), env);
		builder.CreateCondBr(cond, whileBody, whileMerge);
		fun->getBasicBlockList().push_back(whileBody);
		builder.SetInsertPoint(whileBody);
		stmtCodegen(wstmt->getBody(), env, whileCond, whileMerge);
		const auto res = builder.CreateBr(whileCond);
		fun->getBasicBlockList().push_back(whileMerge);
		builder.SetInsertPoint(whileMerge);
		return res;
	}
	case ST_RETURN: {
		const auto ret = dynamic_cast<ReturnStmt*>(stmt);
		Value* res;
		if (ret->getRetVal()) {
			const auto exp = exprCodegen(ret->getRetVal(), env);
			if (exp->getType()->isPointerTy()) {
				const auto res2 = builder.CreateLoad(exp, "ret.load");
				res = builder.CreateRet(res2);
			} else {
				res = builder.CreateRet(exp);
			}
		} else {
			res = builder.CreateRetVoid();
		}
		return res;
	}
	case ST_BREAK: {
		return builder.CreateBr(bk);
	}
	case ST_CONTINUE: {
		return builder.CreateBr(cont);
	}
	case ST_BLOCK: {
		const auto block = dynamic_cast<BlockStmt*>(stmt);
		Environment* new_env = Environment::derive(env);
		Value* res = nullptr;
		for (auto&& x : block->getContent()) {
			res = stmtCodegen(x, new_env, cont, bk);
		}
		return res;
	}
	case ST_EXP: {
		const auto exp = dynamic_cast<ExpStmt*>(stmt);
		return exprCodegen(exp->getExp(), env);
	}
	case ST_VARDEF: {
		const auto vardef = dynamic_cast<VarDefStmt*>(stmt);
		IRBuilder<> tempBuilder(&builder.GetInsertBlock()->getParent()->getEntryBlock(), builder.GetInsertBlock()->getParent()->getEntryBlock().begin());
		Value* res = tempBuilder.CreateAlloca(vardef->getVarType()->toLLVMType(theContext, typeReference), nullptr, vardef->getName());
		Type* type = vardef->getVarType()->toLLVMType(theContext, typeReference);
		if (type->isArrayTy()) { // we shall use the array type with its element pointer
			const auto* atype = dyn_cast<ArrayType>(type);
			IRBuilder<> tempBuilder2(&builder.GetInsertBlock()->getParent()->getEntryBlock(), builder.GetInsertBlock()->getParent()->getEntryBlock().begin());
			Value* res2 = tempBuilder2.CreateAlloca(PointerType::get(atype->getElementType(), 0), nullptr, vardef->getName() + ".ptr");
			res = builder.CreateInBoundsGEP(res, { 
				Constant::getIntegerValue(Type::getInt32Ty(theContext), APInt(32, 0)),
				Constant::getIntegerValue(Type::getInt32Ty(theContext), APInt(32, 0)) 
			});
			builder.CreateStore(res, res2);
			res = res2;
		}
		env->insert(vardef->getName(), res);
		if (vardef->getInitValue()) {
			builder.CreateStore(exprCodegen(vardef->getInitValue(), env), res);
		}
		return res;
	}
	default:
		std::cerr << "Unexpected statment type???" << std::endl;
		return nullptr;
	}
}


 Value* sgs_backend::codegen(AST* ast) {
	switch (ast->astType) {
	case AT_TYPEDEF:
	{
		const auto typeDef = dynamic_cast<TypeDef*>(ast);
		typeReference[typeDef->getName()] = typeDef->getDecType()->toLLVMType(theContext, typeReference);
		return nullptr;
	}
	case AT_STMT:
		return stmtCodegen(dynamic_cast<Statement*>(ast), Environment::derive(globalEnv), nullptr, nullptr);
	case AT_FUNC:
	{
		const auto funDef = dynamic_cast<FuncDef*>(ast);
		FunctionType* funType = funDef->getProto()->getLLVMType(theContext, typeReference);
		Function* fun = Function::Create(funType, GlobalValue::ExternalLinkage, funDef->getProto()->getName(), theModule);
		funcReference[funDef->getProto()->getName()] = fun;
		BasicBlock* funBB = BasicBlock::Create(theContext, "entry", fun);
		builder.SetInsertPoint(funBB);
		auto* env = Environment::derive(globalEnv);
		auto iter = fun->args().begin();
		for (const auto& x : funDef->getProto()->getParam()) {
			if (x.first->getLevel() != Types::TUPLE_TYPE) {
				const auto temp = builder.CreateAlloca(iter->getType(), 0, nullptr, x.second);
				env->insert(x.second, temp);
				builder.CreateStore(iter, temp);
				iter++;
			} else {
				iter->setName(x.second);
				env->insert(x.second, iter);
				iter++;
			}
		}
		return stmtCodegen(funDef->getBody(), env, nullptr, nullptr);
	}
	case AT_PROTO:
	{
		const auto funProto = dynamic_cast<FuncProto*>(ast);
		FunctionType* funTy = funProto->getLLVMType(theContext, typeReference);
		return Function::Create(funTy, GlobalValue::CommonLinkage, funProto->getName(), theModule);
	}
	case AT_GLBVARDEF: {
		const auto glbVarDef = dynamic_cast<GlobalVarDef*>(ast);
		auto* tp = glbVarDef->getType();
		if (tp->getLevel() == Types::BASIC_TYPE) {
			switch (dynamic_cast<SBasicType*>(tp)->getBasicType()) {
			case BasicType::INTEGER: {
			    const auto temp = new GlobalVariable(
					*theModule,
					Type::getInt32Ty(theContext), 
					false, 
					GlobalValue::CommonLinkage,
					Constant::getIntegerValue(Type::getInt32Ty(theContext), APInt(32, 0)), 
					glbVarDef->getName());
				globalEnv->getBindings()[glbVarDef->getName()] = temp;
				return temp;
			}
			case BasicType::FLOAT: {
			    const auto temp = new GlobalVariable(*theModule,
					Type::getFloatTy(theContext), false, GlobalValue::CommonLinkage,
					ConstantFP::get(Type::getFloatTy(theContext), 0.0), glbVarDef->getName());
				globalEnv->insert(glbVarDef->getName(), temp);
				return temp;
			}
			case BasicType::BOOLEAN: {
				return globalEnv->getBindings()[glbVarDef->getName()] = new GlobalVariable(*theModule,
					Type::getInt1Ty(theContext), false, GlobalValue::CommonLinkage,
					Constant::getIntegerValue(Type::getInt1Ty(theContext), APInt(1, 0)), glbVarDef->getName());
			}
			case BasicType::CHAR: {
				return globalEnv->getBindings()[glbVarDef->getName()] = new GlobalVariable(*theModule,
					Type::getInt8Ty(theContext), false, GlobalValue::CommonLinkage,
					Constant::getIntegerValue(Type::getInt8Ty(theContext), APInt(8, 0)), glbVarDef->getName());
			}
			default: 
				std::cerr << "What the fuck???" << std::endl;
			} 
		} else if (tp->getLevel() == Types::ARRAY_TYPE){ // complex type global variable definition

			const auto aryTp = dynamic_cast<SArrayType*>(tp);

			Constant* res = new GlobalVariable(
				*theModule,
				aryTp->toLLVMType(theContext, typeReference),
				false, 
				GlobalValue::CommonLinkage,
				ConstantAggregateZero::get(aryTp->toLLVMType(theContext, typeReference)),
				glbVarDef->getName() + ".array");

			Constant* get = ConstantExpr::getInBoundsGetElementPtr(
				aryTp->toLLVMType(theContext, typeReference),
				res, vector<Constant*>({
					Constant::getIntegerValue(Type::getInt32Ty(theContext), APInt(32, 0)) ,
					Constant::getIntegerValue(Type::getInt32Ty(theContext), APInt(32, 0))
					}));

		    const auto temp = new GlobalVariable(
				*theModule,
				aryTp->getElementType()->toLLVMType(theContext, typeReference)->getPointerTo(0),
				false, 
				GlobalValue::InternalLinkage, 
				get, 
				glbVarDef->getName());
			return globalEnv->getBindings()[glbVarDef->getName()] = temp;
		} else {
			const auto tTp = dynamic_cast<STupleType*>(tp);
			return globalEnv->getBindings()[glbVarDef->getName()] = 
				new GlobalVariable(
					*theModule,
					tTp->toLLVMType(theContext, typeReference), 
					false, 
					GlobalValue::CommonLinkage,
					ConstantAggregateZero::get(tTp->toLLVMType(theContext, typeReference)), glbVarDef->getName());
		}
	}
	default: ;
	}
	return nullptr;
}

void sgs_backend::totalTranslation(const Content& cont, const string& filename) {
	codegenInit();
	builtinFuncInit();
	for (const auto& x : cont) codegen(x);
	std::error_code ec;
	raw_fd_ostream stream(filename, ec, sys::fs::OpenFlags());
	stream << builtInFuncs; 
	theModule->print(stream, nullptr);
}
