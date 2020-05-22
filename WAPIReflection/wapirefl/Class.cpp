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

	for (auto& child : children())
	{
		if (child.tag() == SymTag::Function)
		{
			// Ignore compiler-generated __ prefix stuff
			if(child.name().find("__") != 0)
				mMethods.push_back(child);
		}

		if (child.tag() == SymTag::Data)
			if (child.hasInfo<TI_GET_OFFSET>())
				mFields.push_back(Field(this, child));
	}
}

WAPIReflection::Class::~Class()
{
	//if (mpBaseClass)
	//	delete mpBaseClass;
}

const WAPIReflection::Class* WAPIReflection::Class::baseClass() const
{
	return mpBaseClass;
}

const std::vector<WAPIReflection::Method>& WAPIReflection::Class::methods() const
{
	return mMethods;
}

const std::vector<WAPIReflection::Field>& WAPIReflection::Class::fields() const
{
	return mFields;
}

WAPIReflection::Object WAPIReflection::Class::construct(const std::vector<Object>& params) const
{
	for (auto& m : mMethods)
	{
		if (m.name() == name() && !m.isPure()) // ctor
		{
			auto sig = m.signature();
			if (sig.parameters().size() != params.size())
				continue;

			bool typesMatch = true;
			size_t i = 0;
			for (auto& param : sig.parameters())
			{
				if (param != params[i].type())
				{
					typesMatch = false;
					break;
				}

				i++;
			}

			if (typesMatch)
			{

			}
		}
	}

	throw std::exception("Ctor not found!");
}

const WAPIReflection::Method* WAPIReflection::Class::findMethod(const std::string& name) const
{
	for (auto& m : mMethods)
		if (m.name() == name)
			return &m;

	return nullptr;
}

const WAPIReflection::Field* WAPIReflection::Class::findField(const std::string& name) const
{
	for (auto& m : mFields)
		if (m.name() == name)
			return &m;

	return nullptr;
}

std::string WAPIReflection::Class::baseClassName() const
{
	return mpBaseClass->name();
}
