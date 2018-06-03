#include "TypeSys.h"

namespace sgs_backend {
	bool sameType(SType* t1, SType* t2) {
		if (t1->getLevel() != t2->getLevel()) return false;
		switch (t1->getLevel()) {
		case Types::BASIC_TYPE:
		{
			const auto bt = dynamic_cast<SBasicType*>(t1);
			const auto bt2 = dynamic_cast<SBasicType*>(t2);
			return bt->getBasicType() == bt2->getBasicType();
		}
		case Types::ARRAY_TYPE:
		{
			const auto ap = dynamic_cast<SArrayType*>(t1);
			const auto ap2 = dynamic_cast<SArrayType*>(t2);
			return sameType(ap->getElementType(), ap2->getElementType());
		}
		case Types::TUPLE_TYPE:
		{
			const auto tp = dynamic_cast<STupleType*>(t1);
			const auto tp2 = dynamic_cast<STupleType*>(t2);
			if (tp->getTypes().size() != tp2->getTypes().size()) return false;
			for (size_t i = 0; i < tp->getTypes().size(); i++) {
				if (!sameType(tp->getTypes()[i].second, tp2->getTypes()[i].second) && tp->getTypes()[i].first == tp2->getTypes()[i].first) {
					return false;
				}
			}
			return true;
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
