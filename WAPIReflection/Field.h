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
		Value bind(Class& obj) const
		{
			return Value((char*) &obj + offset(), type());
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

	// A helper to simplify binding fields
	template<class Class>
	Value bind_field(Class& obj, const std::string& field)
	{
		auto t = typeof<Class>();

		if(t.kind() != TypeKind::Class)
			throw std::invalid_argument("Type " + t.name() + " is not a class");

		auto cl = t.asClass();
		auto f = cl.findField(field);
		if (f)
			return f->bind(obj);
		else
			throw std::invalid_argument("Field " + field + " can't be found");
	}
}