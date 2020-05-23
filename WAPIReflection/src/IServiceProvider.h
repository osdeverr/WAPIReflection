#pragma once
#include <wapirefl/Type.h>
#include <wapirefl/Object.h>

namespace DICore {
	struct IServiceProvider
	{
		virtual ~IServiceProvider() {}

		virtual WAPIReflection::Object GetServiceByType(WAPIReflection::Type serviceType) const = 0;

		template<typename TService>
		TService* GetService() const
		{
			static_assert(std::is_class_v<TService>, "Service providers should only by used with classes");

			auto obj = GetServiceByType(typeof<TService>());
			if (obj.isNull())
				return nullptr;

			return &obj.as<TService>();
		}
	};
}