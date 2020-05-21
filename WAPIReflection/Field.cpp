#include "Field.h"
#include "Class.h"

WAPIReflection::Field::Field(const Class* pClass, const Symbol& sym)
	: mpClass(pClass), Symbol(sym),
	mFieldType(Type(mpOwnerAssembly, mModBase, queryInfo<std::uint32_t, TI_GET_TYPE>()))
{
}

const WAPIReflection::Class& WAPIReflection::Field::parentClass() const
{
	return *mpClass;
}

const WAPIReflection::Type& WAPIReflection::Field::type() const
{
	return mFieldType;
}

std::uint32_t WAPIReflection::Field::offset() const
{
	return queryInfo<std::uint32_t, TI_GET_OFFSET>();
}
