#pragma once
#include "Assembly.h"
#include "Type.h"

namespace WAPIReflection {
	struct IObjectImpl
	{
		virtual ~IObjectImpl() {}

		virtual Type type() = 0;
		virtual void* data() = 0;
	};

	template<typename T>
	class ObjectImpl : public IObjectImpl
	{
	public:
		ObjectImpl(const T& val)
			: mValue(val) {}

		Type type() {
			return typeof<T>();
		}

		void* data() {
			return &mValue;
		}

	private:
		T mValue;
	};
}