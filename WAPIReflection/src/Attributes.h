#pragma once
#include <vector>
#include <wapirefl/Object.h>

namespace attribs {
	struct Attribute {
		virtual ~Attribute() {}
	protected:
		Attribute() {}
	};

	struct KY : Attribute {};

	class AttributeImpl
	{
	public:
		template<typename TAttrib>
		AttributeImpl(const TAttrib& value)
			: mpAttrib(new TAttrib(value)), mType(typeof<TAttrib>())
		{
			static_assert(std::is_base_of_v<Attribute, TAttrib>, "Only types derived from attrib::Attribute can be used as class attributes");
		}

		const WAPIReflection::Type& type() const {
			return mType;
		}

		const Attribute& get() const
		{
			return *mpAttrib;
		}
	private:
		WAPIReflection::Type mType;
		std::shared_ptr<Attribute> mpAttrib;
	};

	using List = std::vector<AttributeImpl>;
}
