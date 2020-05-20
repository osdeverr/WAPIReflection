#include "Enum.h"

WAPIReflection::Enum::Enum(const Symbol& type) : Symbol(type)
{
}

std::map<std::string, uint64_t> WAPIReflection::Enum::enumValues() const
{
	std::map<std::string, uint64_t> result;
	for (auto& child : children())
		result[child.name()] = child.queryInfo<VARIANT, TI_GET_VALUE>().uiVal;

	return result;
}
