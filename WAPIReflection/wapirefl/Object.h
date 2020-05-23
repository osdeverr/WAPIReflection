#pragma once
#include "ObjectImpl.h"
#include "Type.h"

#include <memory>
#include <stdexcept>

namespace WAPIReflection {
	class Object
	{
	public:
		template<typename T>
		Object(const T& val) : mpImpl(new ObjectImpl<T>(val))
		{}

		Object(const Type& type, void* pData) : mpImpl(new KnownObjectImpl(type, pData))
		{}

		Object() : mpImpl(nullptr) {}

		bool isNull() const {
			return mpImpl == nullptr;
		}

		Type type() const {
			if (isNull())
				return typeof(nullptr);

			return mpImpl->type();
		}

		const void* data() const {
			if (isNull())
				throw std::exception("Trying to access the data of a null object");

			return mpImpl->data();
		}

		void* data() {
			if (isNull())
				throw std::exception("Trying to access the data of a null object");

			return mpImpl->data();
		}

		template<typename T>
		T* as() {
			auto tType = typeof<T>();
			if (type() != tType)
				throw std::invalid_argument("This object cannot be cast to the type specified.");

			return (T*)data();
		}

		template<typename T>
		const T* as() const {
			auto tType = typeof<T>();
			if (type() != tType)
				throw std::invalid_argument("This object cannot be cast to the type specified.");

			return (T*)data();
		}

		template<typename T>
		explicit operator T() {
			return *as<T>();
		}

		template<typename T>
		explicit operator T() const {
			return *as<T>();
		}
	private:
		IObjectImpl* mpImpl;
	};
}