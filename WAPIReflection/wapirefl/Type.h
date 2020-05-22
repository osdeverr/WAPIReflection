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
		Array,
		Pointer,
		Class,
		Enum
	};

	class Enum;
	class Class;
	class Pointer;
	class Array;
	class ValueType;

	class Type : public Symbol
	{
	public:
		Type(const Symbol& sym);
		Type(Assembly* pOwner, std::uint64_t moduleBase, std::uint32_t typeID);

		virtual std::string name() const override;
		TypeKind kind() const;
		std::size_t size() const;

		const Enum asEnum() const;
		const Class asClass() const;
		const ValueType asValueType() const;
		const Pointer asPointer() const;
		const Array asArray() const;

		void dump(int tabs = 0) const;
	};
}
