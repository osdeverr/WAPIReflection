#include "Pointer.h"

WAPIReflection::Pointer::Pointer(const Symbol& type) : Symbol(type)
{
}

WAPIReflection::Type WAPIReflection::Pointer::pointee() const
{
	return Type(mpOwnerAssembly, mModBase, queryInfo<std::uint32_t, TI_GET_TYPE>());
}
