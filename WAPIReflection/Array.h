#pragma once
#include "Symbol.h"
#include "Type.h"

namespace WAPIReflection {
	class Array : public Symbol
	{
	public:
		Array(const Symbol& type);

		Type elementType() const;
		std::size_t length() const;
	};
}