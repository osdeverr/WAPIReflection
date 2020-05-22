#include "Array.h"

WAPIReflection::Array::Array(const Symbol& type) : Symbol(type)
{
}

WAPIReflection::Type WAPIReflection::Array::elementType() const
{
	return Type(mpOwnerAssembly, mModBase, queryInfo<std::uint32_t, TI_GET_TYPE>());
}

std::size_t WAPIReflection::Array::length() const
{
	return queryInfo<std::size_t, TI_GET_COUNT>();
}
