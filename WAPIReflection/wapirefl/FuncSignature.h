#pragma once
#include "Type.h"

namespace WAPIReflection {
	class FuncSignature : public Symbol
	{
	public:
		FuncSignature(const Symbol& sym);

		Type returnType() const;
		std::vector<Type> parameters() const;
	};
}