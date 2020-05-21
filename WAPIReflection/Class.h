#pragma once
#include "Symbol.h"
#include "Method.h"
#include "Field.h"

namespace WAPIReflection {
	class Class : public Symbol
	{
	public:
		Class(const Symbol& type);
		~Class();

		const Class* baseClass() const;

		const std::vector<Method>& methods() const;
		const std::vector<Field>& fields() const;

		const Method* findMethod(const std::string& name) const;
		const Field* findField(const std::string& name) const;

	private:
		const Class* mpBaseClass;
		std::vector<Method> mMethods;
		std::vector<Field> mFields;

		std::string baseClassName() const;
	};
}