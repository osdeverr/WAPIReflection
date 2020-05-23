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

	class KnownObjectImpl : public IObjectImpl
	{
	public:
		KnownObjectImpl(const Type& t, void* pData) : mType(t), mpObject(pData) {}

		Type type() {
			return mType;
		}

		void* data() {
			return mpObject;
		}

	private:
		Type mType;
		void* mpObject;
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