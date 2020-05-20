#include "Method.h"
#include "Type.h"

WAPIReflection::Method::Method(const Symbol& sym) : Symbol(sym)
{
	mpCode = (void*)queryInfo<std::uint64_t, TI_GET_ADDRESS>();
}

WAPIReflection::FuncSignature WAPIReflection::Method::signature() const
{
	return FuncSignature(Symbol(mpOwnerAssembly, mModBase, queryInfo<std::uint32_t, TI_GET_TYPE>()));
}
