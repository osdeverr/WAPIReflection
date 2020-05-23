#include "Assembly.h"
#include <system_error>
#include <iostream>

bool WAPIReflection::Assembly::sSymInitialized = false;

void WAPIReflection::Assembly::TryInit()
{
	if (!sSymInitialized)
	{
		if (!SymInitialize(GetCurrentProcess(), NULL, true))
			throw std::system_error(
				std::error_code(
					GetLastError(),
					std::system_category()
				),
				"SymInitialize failed"
			);

		sSymInitialized = true;
	}
}

void WAPIReflection::Assembly::LoadTypes()
{
	struct ETBNUserData {
		Assembly* pAssembly;
		std::vector<Type>* pTypes;
	} userData = { this, &this->mAssemblyTypes };

	BOOL result = SymEnumTypesByName(
		GetCurrentProcess(),
		mModuleBase,
		"*!*",
		[](PSYMBOL_INFO pSymInfo,
			ULONG SymbolSize,
			PVOID UserContext) -> BOOL {
				ETBNUserData data = *(ETBNUserData*)UserContext;

				if (pSymInfo->Tag == (ULONG) SymTag::Typedef)
					return TRUE;

				//if (std::string(pSymInfo->Name).find_first_of("__") == 0)
				//	return TRUE;

				//std::cout << pSymInfo->Name << " " << std::hex << pSymInfo->Tag << std::dec << std::endl;
				data.pTypes->push_back(Type(data.pAssembly, pSymInfo->ModBase, pSymInfo->TypeIndex));

				return TRUE;
		}, &userData);

	if (!result)
		throw std::system_error(
			std::error_code(
				GetLastError(),
				std::system_category()
			),
			"SymEnumTypesByName failed"
		);
}

WAPIReflection::Assembly::Assembly()
{
	TryInit();
	mModuleBase = 0; // local module

	LoadTypes();
}

WAPIReflection::Assembly::Assembly(const std::string& fileName)
{
	TryInit();

	auto hMod = LoadLibraryA(fileName.c_str());

	mModuleBase = SymLoadModule(GetCurrentProcess(), NULL, fileName.c_str(), NULL, (DWORD) hMod, 0);
	if (!mModuleBase)
		throw std::system_error(
			std::error_code(
				GetLastError(),
				std::system_category()
			),
			"SymLoadModule failed"
		);

	LoadTypes();
}

const WAPIReflection::Type* WAPIReflection::Assembly::findType(const std::uint32_t id)
{
	auto it = std::find_if(
		mAssemblyTypes.begin(),
		mAssemblyTypes.end(),
		[id](Type& type) -> bool {
			return type.id() == id;
		}
	);

	if (it == mAssemblyTypes.end())
		return nullptr;

	return &*it;
}

const WAPIReflection::Type* WAPIReflection::Assembly::findType(const std::string& name)
{
	auto it = std::find_if(
		mAssemblyTypes.begin(),
		mAssemblyTypes.end(),
		[name](Type& type) -> bool {
			return type.name() == name;
		}
	);

	if (it == mAssemblyTypes.end())
		return nullptr;

	return &*it;
}

const std::vector<WAPIReflection::Type>& WAPIReflection::Assembly::getAllTypes()
{
	return mAssemblyTypes;
}

void WAPIReflection::Assembly::forEachType(std::function<void(const Symbol&)> callback)
{
	struct ETBNUserData {
		Assembly* pAssembly;
		std::function<void(const Symbol&)> callback;
	} userData = { this, callback };

	BOOL result = SymEnumTypesByName(
		GetCurrentProcess(),
		mModuleBase,
		"*!*",
		[](PSYMBOL_INFO pSymInfo,
			ULONG SymbolSize,
			PVOID UserContext) -> BOOL {
				const ETBNUserData& data = *(ETBNUserData*)UserContext;

				if (pSymInfo->Tag == (ULONG)SymTag::Typedef)
					return TRUE;

				data.callback(Symbol(data.pAssembly, pSymInfo->ModBase, pSymInfo->TypeIndex));

				return TRUE;
		}, &userData);

	if (!result)
		throw std::system_error(
			std::error_code(
				GetLastError(),
				std::system_category()
			),
			"SymEnumTypesByName failed"
		);
}

WAPIReflection::Assembly& WAPIReflection::Assembly::local()
{
	static Assembly sLocalAssembly = Assembly();
	return sLocalAssembly;
}
