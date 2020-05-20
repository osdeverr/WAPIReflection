#include "Method.h"
#include "Type.h"

WAPIReflection::Method::Method(const Symbol& sym) : Symbol(sym)
{
	if (hasInfo<TI_GET_ADDRESS>())
		mpCode = (void*)queryInfo<std::uint64_t, TI_GET_ADDRESS>();
	else
		mpCode = nullptr;
}

WAPIReflection::FuncSignature WAPIReflection::Method::signature() const
{
	return FuncSignature(Symbol(mpOwnerAssembly, mModBase, queryInfo<std::uint32_t, TI_GET_TYPE>()));
}

void* WAPIReflection::Method::rawCodePtr() const
{
	return mpCode;
}

bool WAPIReflection::Method::isPure() const
{
	return (mpCode == nullptr);
}
