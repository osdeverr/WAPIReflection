#pragma once
#include "Symbol.h"
#include "Assembly.h"

#include "Value.h"

#include <stdexcept>

namespace WAPIReflection {
	class Class;

	class Field : public Symbol
	{
	public:
		Field(const Class* pClass, const Symbol& sym);

		const Class& parentClass() const;
		const Type& type() const;

		std::uint32_t offset() const;

		template<class Class>
		Value<Class> bind(Class& obj) const
		{
			return Value<Class>((char*) &obj + offset(), type());
		}

		template<typename Class, typename T>
		T& getValue(Class& thisPtr) const
		{
			if (typeof<Class>() != parentClass())
				throw std::invalid_argument("Field::value<Class, T>(): wrong class type used");

			if (typeof<T>() != type())
				throw std::invalid_argument("Field::value<Class, T>(): wrong field type used " + typeof<T>().name() + " and " + type().name());

			return *(T*) (((char*)&thisPtr) + offset());
		}
	private:
		const Class* mpClass;
		Type mFieldType;
	};
}