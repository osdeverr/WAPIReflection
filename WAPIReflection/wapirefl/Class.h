#pragma once
#include "Symbol.h"
#include "Method.h"
#include "Field.h"
#include "Type.h"
#include "Object.h"

namespace WAPIReflection {
	class Class : public Symbol
	{
	public:
		Class(const Symbol& type);
		~Class();

		const Class* baseClass() const;

		const bool isBase(const Class& cl) const;

		const std::vector<Method>& methods() const;
		const std::vector<Field>& fields() const;

		template<class T = void>
		T* constructNoParams() const {
			void* pObj = malloc(Type(*this).size());

			for (auto& m : mMethods)
			{
				if (m.name() == name() && m.signature().parameters().empty() && !m.isPure()) // ctor
				{
					m.invoke<void>(pObj);

					return (T*) pObj;
				}
			}

			return (T*) pObj;
		}

		Object construct(const std::vector<Object>& params) const;

		const Method* findMethod(const std::string& name) const;
		const Field* findField(const std::string& name) const;

	private:
		const Class* mpBaseClass;
		std::vector<Method> mMethods;
		std::vector<Field> mFields;

		std::string baseClassName() const;
	};
}