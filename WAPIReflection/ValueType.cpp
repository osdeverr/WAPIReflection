#include "ValueType.h"

WAPIReflection::ValueType::ValueType(const Symbol& type) : Symbol(type)
{
}

WAPIReflection::BasicType WAPIReflection::ValueType::underlyingType() const
{
	return queryInfo<BasicType, TI_GET_BASETYPE>();
}
