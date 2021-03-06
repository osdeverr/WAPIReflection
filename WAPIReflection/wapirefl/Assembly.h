#pragma once
#include <Windows.h>
#include <DbgHelp.h>
#include <string>
#include <vector>
#include <functional>

#include "Type.h"

namespace WAPIReflection {
	class Assembly
	{
	public:
		Assembly(const std::string& fileName);

		const Type* findType(const std::uint32_t id);
		const Type* findType(const std::string& name);

		const std::vector<Type>& getAllTypes();
		
		void forEachType(std::function<void(const Symbol&)> callback);

		static Assembly& local();
	private:
		Assembly(); // local
		void TryInit();
		void LoadTypes();

		ULONG64 mModuleBase;

		std::vector<Type> mAssemblyTypes;

		static bool sSymInitialized;
	};

	namespace Detail
	{
		/*
		 * Erase all Occurrences of given substring from main string.
		 */
		inline void eraseAllSubStr(std::string & mainStr, const std::string & toErase)
		{
			size_t pos = std::string::npos;

			// Search for the substring in string in a loop untill nothing is found
			while ((pos = mainStr.find(toErase)) != std::string::npos)
			{
				// If found then erase it from string
				mainStr.erase(pos, toErase.length());
			}
		}

		template<typename T>
		struct StructWithType
		{
			T* pValue;
		};

		template<typename T>
		const Type& GetTypeFromLocalAssembly()
		{
			using RawType = typename std::remove_pointer<T>::type;

			std::string name_raw = typeid(RawType).name();

			// Trim the class/struct/enum prefix
			eraseAllSubStr(name_raw, "class ");
			eraseAllSubStr(name_raw, "struct ");
			eraseAllSubStr(name_raw, "enum ");

			auto pType = Assembly::local().findType(name_raw);
			if(!pType)
				throw std::exception("The specified type does not exist in this assembly.");

			return *pType;
		}

		template<typename T>
		const Type FindLocalType()
		{
			// This should never fail.
			[[maybe_unused]] StructWithType<T> s;
			return GetTypeFromLocalAssembly<StructWithType<T>>().
				asClass().
				findField("pValue")->type().
				asPointer().pointee();
		}
	}
}

template<typename T>
const WAPIReflection::Type typeof()
{
	return WAPIReflection::Detail::FindLocalType<T>();
}

template<typename T>
const WAPIReflection::Type typeof(const T& value)
{
	return WAPIReflection::Detail::FindLocalType<T>();
}
