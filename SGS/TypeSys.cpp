#include "TypeSys.h"

namespace sgs_backend {
	bool sameType(SType* t1, SType* t2) {
		if (t1->getLevel() != t2->getLevel()) return false;
		switch (t1->getLevel()) {
		case Types::BASIC_TYPE:
		{
			const auto bt = dynamic_cast<SBasicType*>(t1);
			const auto bt2 = dynamic_cast<SBasicType*>(t2);
            if (bt == bt2) return true;
		    return !(bt->getBasicType() == BasicType::FLOAT || bt2->getBasicType() == BasicType::FLOAT);
		}
		case Types::ARRAY_TYPE:
		{
			const auto ap = dynamic_cast<SArrayType*>(t1);
			const auto ap2 = dynamic_cast<SArrayType*>(t2);
			return ap->getElementType() == ap2->getElementType();
		}
		case Types::TUPLE_TYPE:
		{
            return t1 == t2;
		}
		default:
			return false;
		}
	}

	Type* getParamType(SType* t, LLVMContext& context, const map<string, Type*>& typeReference){
		if (t->getLevel() == Types::BASIC_TYPE) {
			return t->toLLVMType(context, typeReference);
		}
		if (t->getLevel() == Types::ARRAY_TYPE) {
			return PointerType::get(dynamic_cast<SArrayType*>(t)->getElementType()->toLLVMType(context, typeReference), 0);
		}
		return PointerType::get(t->toLLVMType(context, typeReference), 0);
	}

	string typeToString(SType* tp) {
		switch (tp->getLevel()) {
		case Types::BASIC_TYPE:
		{
			const auto bt = dynamic_cast<SBasicType*>(tp);
			switch (bt->getBasicType()) {
			case BasicType::INTEGER: return "int";
			case BasicType::FLOAT: return "float";
			case BasicType::BOOLEAN: return "bool";
			case BasicType::CHAR: return "char";
			default:
				return "";
			}
		}
		case Types::ARRAY_TYPE:
		{
			const auto at = dynamic_cast<SArrayType*>(tp);
			return "[" + typeToString(at->getElementType()) + " x " + std::to_string(at->getCount()) + "]";
		}
		case Types::TUPLE_TYPE:
		{
			const auto tt = dynamic_cast<STupleType*>(tp);
			string res = tt->getName() + "{";
			res += typeToString(tt->getTypes()[0].second);
			for (size_t i = 1; i < tt->getTypes().size(); i++) {
				res += "," + typeToString(tt->getTypes()[i].second);
			}
			return res + "}";
		}
		default:
			return "";
		}
	}


}
