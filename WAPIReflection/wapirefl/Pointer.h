#pragma once
#include "Symbol.h"
#include "Type.h"

namespace WAPIReflection {
	class Pointer : public Symbol
	{
	public:
		Pointer(const Symbol& type);

		Type pointee() const;
	};
}