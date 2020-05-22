#pragma once
#include "Symbol.h"
#include <string>
#include <map>

namespace WAPIReflection {
	class Enum : public Symbol
	{
	public:
		Enum(const Symbol& type);

		std::map<std::string, uint64_t> enumValues() const;
	};
}