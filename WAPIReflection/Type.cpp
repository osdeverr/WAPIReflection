#include "Type.h"
#include "Assembly.h"

#include <iostream>
#include <map>

#include "BasicType.h"

#include "Class.h"
#include "Enum.h"

WAPIReflection::Type::Type(const Symbol& sym) : Symbol(sym)
{
}

WAPIReflection::Type::Type(Assembly* pOwner, std::uint64_t moduleBase, std::uint32_t typeID)
	: Symbol(pOwner, moduleBase, typeID)
{
}

std::string WAPIReflection::Type::name() const
{
	static std::map<BasicType, std::string> primitiveTypenames = {
		{BasicType::NoType, "<invalid>"},
		{BasicType::Void, "void"},
		{BasicType::Char, "char"},
		{BasicType::WChar, "wchar_t"},
		{BasicType::Int, "int"},
		{BasicType::UInt, "unsigned int"},
		{BasicType::Float, "float"},
		{BasicType::BCD, "double"},
		{BasicType::Bool, "bool"},
		{BasicType::Long, "long"},
		{BasicType::ULong, "unsigned long"},
		{BasicType::Currency, "CURRENCY"},
		{BasicType::Date, "DATE"},
		{BasicType::Variant, "VARIANT"},
		{BasicType::Complex, "<complex>"},
		{BasicType::Bit, "<bit>"},
		{BasicType::BSTR, "BSTR"},
		{BasicType::Hresult, "HRESULT"}
	};

	switch(kind())
	{
	case TypeKind::Class:
	case TypeKind::Enum:
	{
		auto pName = queryInfo<WCHAR*, TI_GET_SYMNAME>();

		std::wstring wsName(pName);
		std::string name = std::string(wsName.begin(), wsName.end());
		return name;
	}
	case TypeKind::Function:
	{
		Type ret(mpOwnerAssembly, mModBase, queryInfo<std::uint32_t, TI_GET_TYPE>());

		std::string args;

		auto cvec = children();
		size_t i = 0;
		for (auto& c : cvec)
		{
			if (c.tag() == SymTag::FunctionArgType)
			{
				std::uint32_t tid = c.queryInfo<std::uint32_t, TI_GET_TYPE>();
				Type t(mpOwnerAssembly, mModBase, tid);
				args += t.name();
				if (i < cvec.size() - 1)
					args += ", ";

				i++;
			}
		}

		return ret.name() + "(" + args + ")";
	}
	case TypeKind::Pointer:
	{
		Type t(mpOwnerAssembly, mModBase, queryInfo<std::uint32_t, TI_GET_TYPE>());
		std::string ptrName = t.name();
		if (t.kind() == TypeKind::Function)
			ptrName.insert(ptrName.find_first_of('('), "(*)");
		else
			ptrName += " *";
		
		return ptrName;
	}
	case TypeKind::ValueType:
	{
		return primitiveTypenames[queryInfo<BasicType, TI_GET_BASETYPE>()];
	}
	default:
	{
		return "<unknown typename>";
	}
	}
}

WAPIReflection::TypeKind WAPIReflection::Type::kind() const
{
	switch (tag())
	{
	case SymTag::BaseType:
		return TypeKind::ValueType;
	case SymTag::FunctionType:
		return TypeKind::Function;
	case SymTag::UDT:
		return TypeKind::Class;
	case SymTag::Enum:
		return TypeKind::Enum;
	case SymTag::PointerType:
		return TypeKind::Pointer;
	default:
		return TypeKind::Unknown;
	}
}

const WAPIReflection::Enum WAPIReflection::Type::asEnum() const
{
	if (kind() != TypeKind::Enum)
		throw new std::invalid_argument("This type can't be used as an enum");

	return Enum(*this);
}

const WAPIReflection::Class WAPIReflection::Type::asClass() const
{
	if (kind() != TypeKind::Class)
		throw new std::invalid_argument("This type can't be used as a class");

	return Class(*this);
}

void WAPIReflection::Type::dump(int tabs) const
{
	std::map<BasicType, std::string> primitives = {
		{BasicType::NoType, "<invalid>"},
		{BasicType::Void, "void"},
		{BasicType::Char, "char"},
		{BasicType::WChar, "wchar_t"},
		{BasicType::Int, "int"},
		{BasicType::UInt, "unsigned int"},
		{BasicType::Float, "float"},
		{BasicType::BCD, "double"},
		{BasicType::Bool, "bool"},
		{BasicType::Long, "long"},
		{BasicType::ULong, "unsigned long"},
		{BasicType::Currency, "CURRENCY"},
		{BasicType::Date, "DATE"},
		{BasicType::Variant, "VARIANT"},
		{BasicType::Complex, "<complex>"},
		{BasicType::Bit, "<bit>"},
		{BasicType::BSTR, "BSTR"},
		{BasicType::Hresult, "HRESULT"}
	};

	if (tag() == SymTag::BaseType)
	{
		std::cout << "Base type: '" << primitives[queryInfo<BasicType, TI_GET_BASETYPE>()] << "'" << std::endl;
	}
	else if (tag() == SymTag::Enum)
	{
		for (auto& child : children())
		{
			std::cout << child.name() << "=" << child.queryInfo<VARIANT, TI_GET_VALUE>().uiVal << std::endl;
		}
	}
	else if (tag() == SymTag::PointerType)
	{
		std::cout << "Pointer to ";
		std::uint32_t tid = queryInfo<std::uint32_t, TI_GET_TYPE>();
		Type t(mpOwnerAssembly, mModBase, tid);
		t.dump(tabs + 1);
	}
	else if (tag() == SymTag::FunctionType)
	{
		std::cout << "Function: " << name() << std::endl;
		for (auto& c : children())
		{
			if (c.tag() == SymTag::FunctionArgType)
			{
				std::uint32_t tid = c.queryInfo<std::uint32_t, TI_GET_TYPE>();
				Type t(mpOwnerAssembly, mModBase, tid);
				for (int i = 0; i < tabs; i++) std::cout << '\t';
				std::cout << "Parameter: ";
				t.dump(tabs + 1);
			}
		}
	}
	else if (tag() == SymTag::UDT)
	{
		std::cout << "Class: " << name() << std::endl;
		for (auto& child : children())
		{
			SymTag childTag = child.queryInfo<SymTag, TI_GET_SYMTAG>();
			if (childTag == SymTag::BaseClass)
			{
				std::uint32_t tid = child.queryInfo<std::uint32_t, TI_GET_TYPE>();
				const Type* pType = mpOwnerAssembly->FindType(tid);
				if (pType)
				{
					for (int i = 0; i < tabs; i++) std::cout << '\t';
					std::cout << "\tBase Class: ";
					pType->dump(tabs + 1);
				}
			}
			else if (childTag == SymTag::Function)
			{
				for (int i = 0; i < tabs; i++) std::cout << '\t';

				std::cout << "\tMethod: " << child.name();

				std::cout << std::endl;

				for (auto& c : child.children())
				{
					if (c.tag() == SymTag::Data)
					{
						std::uint32_t tid = c.queryInfo<std::uint32_t, TI_GET_TYPE>();
						Type t(mpOwnerAssembly, mModBase, tid);
						for (int i = 0; i < tabs; i++) std::cout << '\t';
						std::cout << "\t\tArgument: " << t.name() << std::endl;
					}
				}
			}
			else if (childTag == SymTag::Data)
			{
				std::uint32_t tid = child.queryInfo<std::uint32_t, TI_GET_TYPE>();
				Type t(mpOwnerAssembly, mModBase, tid);
				for (int i = 0; i < tabs; i++) std::cout << '\t';
				std::cout << "\tField: " << child.name() << std::endl;
				//t.dump(tabs + 1);
			}
			else
			{
				for (int i = 0; i < tabs; i++) std::cout << '\t';
				std::cout << "\tUnknown Child: " << (std::uint32_t) childTag << std::endl;
			}
		}
	}
	else
	{
		std::cout << "Unknown Type Tag: " << (std::uint32_t) tag() << std::endl;
	}
}
