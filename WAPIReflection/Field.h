#pragma once
#include "Symbol.h"
#include "Type.h"

namespace WAPIReflection {
	class Field : public Symbol
	{
	public:
		Field(const Symbol& sym);

		Type type() const;

		std::uint32_t offset() const;
	};
}