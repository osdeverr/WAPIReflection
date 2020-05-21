#pragma once
#include "Type.h"
#include "Pointer.h"
#include "ValueType.h"
#include "Array.h"

namespace WAPIReflection {
	template<class Class = void>
	class Value
	{
	public:
		Value(void* pValue, const Type& fieldType)
			: mpValue(pValue), mType(fieldType) {}

		template<typename T>
		Value& operator=(const T& val)
		{
			assignRecursive(mType, val);
			return *this;
		}

		template<typename T>
		operator T()
		{
			return convertRecursive<T>(mType);
		}

		Value operator[](const size_t index)
		{
			auto& arr = mType.asArray();
			if (index >= arr.length())
				throw std::out_of_range("operator[] out of array bounds");

			return Value((char*)mpValue + arr.elementType().size() * index, arr.elementType());
		}

	private:
		void* mpValue;
		Type mType;

		template<typename T>
		void assignRecursive(const Type& type, const T& value)
		{
			if (type.kind() == TypeKind::ValueType)
				type.asValueType().assign(mpValue, value);
			else if (type.kind() == TypeKind::Pointer)
				assignRecursive(type.asPointer().pointee(), value);
			else
				throw std::invalid_argument("Value can only assign value and pointer types, but was provided with " + std::to_string((int)type.kind()));
		}

		template<typename T>
		T convertRecursive(const Type& type)
		{
			if (type.kind() == TypeKind::ValueType)
				return type.asValueType().convert<T>(mpValue);
			else if (type.kind() == TypeKind::Pointer)
				return convertRecursive<T>(type.asPointer().pointee());
			else
				throw std::invalid_argument("Value can only convert value and pointer types, but was provided with " + std::to_string((int)type.kind()));
		}
	};
}