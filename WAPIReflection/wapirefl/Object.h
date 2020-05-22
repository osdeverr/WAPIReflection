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

		Type type() const { return mpImpl->type(); }
		const void* data() const { return mpImpl->data(); }
		void* data() { return mpImpl->data(); }

		template<typename T>
		explicit operator T() const {
			if (type() != typeof<T>())
				throw std::invalid_argument("This object cannot be cast to the type specified.");

			return *(T*)data();
		}
	private:
		IObjectImpl* mpImpl;
	};
}