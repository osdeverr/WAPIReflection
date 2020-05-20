#include "Symbol.h"
#include "Type.h"
#include <iostream>

WAPIReflection::Symbol::Symbol(Assembly* pOwner, std::uint64_t moduleBase, std::uint32_t symbolID) : mpOwnerAssembly(pOwner), mModBase(moduleBase), mSymbolID(symbolID)
{
}

WAPIReflection::SymTag WAPIReflection::Symbol::tag() const
{
	return queryInfo<SymTag, TI_GET_SYMTAG>();
}

std::uint32_t WAPIReflection::Symbol::id() const
{
	return queryInfo<std::uint32_t, TI_GET_SYMINDEX>();
}

std::string WAPIReflection::Symbol::name() const
{
	try {
		auto pName = queryInfo<WCHAR*, TI_GET_SYMNAME>();

		std::wstring wsName(pName);
		return std::string(wsName.begin(), wsName.end());
	}
	catch (...)
	{
		return "<no name>";
	}
}

std::vector<WAPIReflection::Symbol> WAPIReflection::Symbol::children() const
{
	std::vector<Symbol> result;
	try {
		auto childCount = queryInfo<std::size_t, TI_GET_CHILDRENCOUNT>();
		if (!childCount) return result;

		TI_FINDCHILDREN_PARAMS* pParams = (TI_FINDCHILDREN_PARAMS*)malloc(sizeof(TI_FINDCHILDREN_PARAMS) + (childCount - 1) * sizeof(ULONG));
		pParams->Count = childCount;
		pParams->Start = 0;

		queryInfo<TI_FINDCHILDREN>(pParams);

		for (ULONG i = 0; i < pParams->Count; i++)
		{
			result.push_back(Symbol(mpOwnerAssembly, mModBase, pParams->ChildId[i]));
			
			//std::cout << pParams->ChildId[i] << ":" << tag << std::endl;
		}

		free(pParams);
		return result;
	}
	catch (...)
	{
		return result;
	}
}
