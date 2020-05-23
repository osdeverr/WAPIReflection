#pragma once
#include <cstdint>
#include <Windows.h>
#include <DbgHelp.h>
#include <system_error>
#include <vector>

#include "SymTag.h"

namespace WAPIReflection {
	class Assembly;
	class Type;

	class Symbol
	{
	public:
		Symbol(Assembly* pOwner, std::uint64_t moduleBase, std::uint32_t symbolID);

		SymTag tag() const;
		std::uint32_t id() const;

		// Made it virtual to allow overrides
		virtual std::string name() const;

		std::vector<Symbol> children() const;

		bool operator==(const Symbol& rhs) const {
			bool eqByIds =
				this->mpOwnerAssembly == rhs.mpOwnerAssembly &&
				this->mModBase == rhs.mModBase &&
				this->mSymbolID == rhs.mSymbolID;
			bool eqByName = false;
			if (hasInfo<TI_GET_SYMNAME>())
				eqByName = this->name() == rhs.name();

			return eqByIds || eqByName;
		}
		bool operator!=(const Symbol& rhs) const {
			return !(*this == rhs);
		}

		template<IMAGEHLP_SYMBOL_TYPE_INFO InfoType>
		bool hasInfo() const
		{
			char dummy[64];
			return SymGetTypeInfo(GetCurrentProcess(), mModBase, mSymbolID, InfoType, dummy);
		}

		template<typename DataType, IMAGEHLP_SYMBOL_TYPE_INFO InfoType>
		DataType queryInfo() const
		{
			DataType data;
			if (!SymGetTypeInfo(GetCurrentProcess(), mModBase, mSymbolID, InfoType, &data))
				throw std::system_error(
					std::error_code(
						GetLastError(),
						std::system_category()
					),
					"SymGetTypeInfo failed"
				);

			return data;
		}

		template<IMAGEHLP_SYMBOL_TYPE_INFO InfoType, typename DataType>
		void queryInfo(DataType* data) const
		{
			if (!SymGetTypeInfo(GetCurrentProcess(), mModBase, mSymbolID, InfoType, data))
				throw std::system_error(
					std::error_code(
						GetLastError(),
						std::system_category()
					),
					"SymGetTypeInfo failed"
				);
		}

	protected:
		Assembly* mpOwnerAssembly;
		std::uint64_t mModBase;
		std::uint32_t mSymbolID;
	};
}