#include "Field.h"

WAPIReflection::Field::Field(const Symbol& sym) : Symbol(sym)
{
}

WAPIReflection::Type WAPIReflection::Field::type() const
{
	return Type(mpOwnerAssembly, mModBase, queryInfo<std::uint32_t, TI_GET_TYPE>());
}

std::uint32_t WAPIReflection::Field::offset() const
{
	return queryInfo<std::uint32_t, TI_GET_OFFSET>();
}
