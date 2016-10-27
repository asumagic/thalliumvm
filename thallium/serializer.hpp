#ifndef THALLIUMVM_SERIALIZER_HPP
#define THALLIUMVM_SERIALIZER_HPP

#include <type_traits>

namespace thallium
{
	/**
	 * Serialize an integer type and write to an iterator
	 * \param t Value to serialize
	 * \param i Iterator to uint8_t
	 */
	template<typename T, typename It, std::enable_if_t<std::is_integral<T>::value>* = nullptr>
	void serialize_type(T t, It it);

	/**
	 * Deserialize from an iterator and write to an integer type
	 * \param i Iterator to uint8_t
	 */
	template<typename T, typename It, std::enable_if_t<std::is_integral<T>::value>* = nullptr>
	T deserialize_type(It it);
}

#include "serializer.tpp"

#endif
