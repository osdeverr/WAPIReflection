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

		std::vector<Method> methods() const;
		std::vector<Field> fields() const;
	private:
		const Class* mpBaseClass;
	};
}