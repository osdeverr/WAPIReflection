#include "FuncSignature.h"

WAPIReflection::FuncSignature::FuncSignature(const Symbol& sym) : Symbol(sym)
{
}

WAPIReflection::Type WAPIReflection::FuncSignature::returnType() const
{
	return Type(mpOwnerAssembly, mModBase, queryInfo<std::uint32_t, TI_GET_TYPE>());
}

std::vector<WAPIReflection::Type> WAPIReflection::FuncSignature::parameters() const
{
	std::vector<Type> result;

	for (auto& child : children())
		if (child.tag() == SymTag::FunctionArgType)
			result.push_back(Type(mpOwnerAssembly, mModBase, child.queryInfo<std::uint32_t, TI_GET_TYPE>()));

	return result;
}
