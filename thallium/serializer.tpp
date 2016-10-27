#ifndef THALLIUMVM_SERIALIZER_TPP
#define THALLIUMVM_SERIALIZER_TPP

#include "serializer.hpp"
#include "instruction.hpp"

namespace thallium
{
	template<typename T, typename It, std::enable_if_t<std::is_integral<T>::value>*>
	void serialize_type(T t, It it)
	{
		const size_t isz = sizeof(T);
		for (size_t i = 0; i < isz; ++i)
		{
			*(it + i) = (t >> (i * 8)) & 0xFF;
		}
	}

	template<typename T, typename It, std::enable_if_t<std::is_integral<T>::value>*>
	T deserialize_type(It it)
	{
		T result = 0;
		const size_t isz = sizeof(T);

		for (size_t i = 0; i < isz; ++i)
		{
			result |= static_cast<uint64_t>(*(it + i)) << (i * 8ull);
		}

		return result;
	}
}

#endif
