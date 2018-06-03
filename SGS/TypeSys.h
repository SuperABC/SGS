#pragma once
#include <vector>
#include <string>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <cstddef>
#include <map>

using namespace llvm;

namespace sgs_backend {

	using std::vector;
	using std::string;
	using std::pair;
	using llvm::Type;
	using std::map;

	enum class Types {
		BASIC_TYPE,
		ARRAY_TYPE,
		TUPLE_TYPE
	};

	class Context;

	class SType {
		Types level;
		friend class Context;
	public:
        SType(const SType&) = delete;
        SType(SType&&) = delete;
        SType& operator = (const SType&) = delete;
        SType& operator = (SType&&) = delete;
        explicit SType(Types level) : level(level) {}
		virtual ~SType() = default;
		Types getLevel() const { return level; }
		virtual Type* toLLVMType(LLVMContext& context, const map<string, Type*>& typeReference) const {
			return nullptr;
		}
	};

	enum class BasicType {
		INTEGER, // int
		FLOAT, // double
		BOOLEAN, // bool
		CHAR, // char
	};

	class SBasicType : public SType {
		friend class Context;
		BasicType type;
		explicit SBasicType(BasicType type) : SType(Types::BASIC_TYPE), type(type) {}
	public:
		BasicType getBasicType() const { return type; }
		Type* toLLVMType(LLVMContext& context, const map<string, Type*>& typeReference) const override {
			if (type == BasicType::INTEGER) {
				return Type::getInt32Ty(context);
			} else if (type == BasicType::FLOAT) {
				return Type::getFloatTy(context);
			} else if (type == BasicType::CHAR){
				return Type::getInt8Ty(context);
			} else if (type == BasicType::BOOLEAN) { 
				return Type::getInt1Ty(context);
			}
			return nullptr;
		}
	};

	class SArrayType : public SType {
		friend class Context;
		SType* type;
		size_t count;
		SArrayType(SType* type, size_t count) : SType(Types::ARRAY_TYPE), type(type), count(count) {}
	public:
		size_t getCount() const {
			return count;
		}
		SType* getElementType() const { return type; }
		Type* toLLVMType(LLVMContext& context, const map<string, Type*>& typeReference) const override {
			return ArrayType::get(type->toLLVMType(context, typeReference), count);
		}
	};

	class STupleType : public SType {

		friend class Context;

		std::vector<std::pair<std::string, SType*>> types;
		string name;
		STupleType(vector<pair<string, SType*>> types, string name) : SType(Types::TUPLE_TYPE), types(std::move(types)), name(std::move(name)) {}
	public:
		auto& getTypes() const {
			return types;
		}
		const string& getName() const {
			return name;
		}
		SType* getElemType(const string& str) const {
			for (const auto& x : types) {
				if (x.first == str) return x.second;
			}
			return nullptr;
		}
		Type* toLLVMType(LLVMContext& context, const map<string, Type*>& typeReference) const override {
			if (typeReference.find(name) != typeReference.end()) {
				return typeReference.find(name)->second;
			}
			vector<Type*> res;
			for (auto && type : types) {
				res.push_back(type.second->toLLVMType(context, typeReference));
			}
			return StructType::create(context, res, name);
		}
	};

	class Environment {
		Environment* parent;
		map<string, Value*> bindings;
	public:
		explicit Environment(Environment* parent = nullptr) : parent(parent) {}
		Value* operator [] (const string& str) {
			auto temp = this;
			while (temp) {
				if (temp->bindings.find(str) != temp->bindings.end()) {
					return temp->bindings[str];
				}
				temp = temp->parent;
			}
			return nullptr;
		}
		Value* find(const string& str) {
			return operator[](str);
		}
		map<string, Value*>& getBindings() {
			return bindings;
		}
		
		void insert(const string& str, Value* val) {
			bindings[str] = val;
		}

		static Environment* derive(Environment* env) {
			return new Environment(env);
		}
	};
 //
	string typeToString(SType* tp);

	class Context {
		map<string, SType*> typeRef;
	public:
		SType* getIntType() {
			if (typeRef.find("int") != typeRef.end()) {
				return typeRef.find("int")->second;
			}
			return typeRef["int"] = new SBasicType(BasicType::INTEGER);
 		}
		
		SType* getCharType() {
			if (typeRef.find("char") != typeRef.end()) {
				return typeRef.find("char")->second;
			}
			return typeRef["char"] = new SBasicType(BasicType::CHAR);
		}

		SType* getBoolType() {
			if (typeRef.find("bool") != typeRef.end()) {
				return typeRef.find("bool")->second;
			}
			return typeRef["bool"] = new SBasicType(BasicType::BOOLEAN);
		}

		SType* getFloatType() {
			if (typeRef.find("float") != typeRef.end()) {
				return typeRef.find("float")->second;
			}
			return typeRef["float"] = new SBasicType(BasicType::FLOAT);
		}

		SType* getArrayType(SType* elementType, size_t count) {
			auto* type = new SArrayType(elementType, count);
			const string res = typeToString(type);
			if (typeRef.find(res) != typeRef.end()) {
				delete type;
				return typeRef.find(res)->second;
			}
			return typeRef[res] = type;
		}

		SType* getTupleType(const vector<pair<string, SType*>>& elements, const string& name) {
			auto* type = new STupleType(elements, name);
			const string res = typeToString(type);
			if (typeRef.find(res) != typeRef.end()) {
				delete type;
				return typeRef.find(res)->second;
			}
			return typeRef[res] = type;
		}

		~Context() {
			for (auto&& x : typeRef) {
				delete x.second;
			}
		}
	};

	// typedef SType* TypeObj;

	bool sameType(SType* t1, SType* t2);

	Type* getParamType(SType* t, LLVMContext& context, const map<string, Type*>& typeReference);

}
