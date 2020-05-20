#include "Class.h"
#include "Type.h"

WAPIReflection::Class::Class(const Symbol& type) : Symbol(type)
{
	mpBaseClass = nullptr;

	for (auto& child : children())
	{
		if (child.tag() == SymTag::BaseClass)
			mpBaseClass = new Class(Type(mpOwnerAssembly, mModBase, child.queryInfo<std::uint32_t, TI_GET_TYPE>()));
	}
}

WAPIReflection::Class::~Class()
{
	if (mpBaseClass)
		delete mpBaseClass;
}

const WAPIReflection::Class* WAPIReflection::Class::baseClass() const
{
	return mpBaseClass;
}

std::vector<WAPIReflection::Method> WAPIReflection::Class::methods() const
{
	std::vector<Method> result;

	for (auto& child : children())
		if (child.tag() == SymTag::Function)
			result.push_back(child);

	return result;
}

std::vector<WAPIReflection::Field> WAPIReflection::Class::fields() const
{
	std::vector<Field> result;

	for (auto& child : children())
		if (child.tag() == SymTag::Data)
			if (child.hasInfo<TI_GET_OFFSET>())
				result.push_back(child);

	return result;
}
