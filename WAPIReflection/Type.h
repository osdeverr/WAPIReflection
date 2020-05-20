#pragma once
#include <Windows.h>
#include <DbgHelp.h>
#include <cstdint>
#include <string>
#include <system_error>

#include "Symbol.h"

namespace WAPIReflection {
	enum class TypeKind
	{
		Unknown,
		Function,
		ValueType,
		Pointer,
		Class,
		Enum
	};

	class Enum;
	class Class;

	class Type : public Symbol
	{
	public:
		Type(const Symbol& sym);
		Type(Assembly* pOwner, std::uint64_t moduleBase, std::uint32_t typeID);

		virtual std::string name() const override;
		TypeKind kind() const;

		const Enum asEnum() const;
		const Class asClass() const;

		void dump(int tabs = 0) const;
	};
}
