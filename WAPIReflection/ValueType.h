#pragma once
#include "Symbol.h"
#include "Type.h"
#include "BasicType.h"

namespace WAPIReflection {
	class ValueType : public Symbol
	{
	public:
		ValueType(const Symbol& type);

		BasicType underlyingType() const;

		template<typename T>
		void assign(void* pData, const T& value) const
		{
			auto t = underlyingType();

			switch (t)
			{
			case BasicType::Char:
				*(char*)pData = (char)value;
				break;
			case BasicType::WChar:
				*(wchar_t*)pData = (wchar_t)value;
				break;
			case BasicType::Int:
				*(signed int*)pData = (signed int)value;
				break;
			case BasicType::UInt:
				*(unsigned int*)pData = (unsigned int)value;
				break;
			case BasicType::Float:
				*(float*)pData = (float)value;
				break;
			case BasicType::Bool:
				*(bool*)pData = (bool)value;
				break;
			case BasicType::Long:
				*(signed long*)pData = (signed long)value;
				break;
			case BasicType::ULong:
				*(unsigned long*)pData = (unsigned long)value;
				break;

			default:
				throw std::exception("Unimplemented ValueType assignment");
			}
		}

		template<typename T>
		T convert(void* pData) const
		{
			auto t = underlyingType();

			switch (t)
			{
			case BasicType::Char:
				return *(char*)pData;
			case BasicType::WChar:
				return *(wchar_t*)pData;
			case BasicType::Int:
				return *(signed int*)pData;
			case BasicType::UInt:
				return *(unsigned int*)pData;
			case BasicType::Float:
				return *(float*)pData;
			case BasicType::Bool:
				return *(bool*)pData;
			case BasicType::Long:
				return *(signed long*)pData;
			case BasicType::ULong:
				return *(unsigned long*)pData;

			default:
				throw std::exception("Unimplemented ValueType conversion");
			}
		}
	};
}