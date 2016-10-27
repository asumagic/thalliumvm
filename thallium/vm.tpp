#ifndef THALLIUM_VM_TPP
#define THALLIUM_VM_TPP

#include "vm.hpp"

namespace thallium
{
	template<typename... Types>
	std::tuple<Types...> VM::decode(uint64_t argument)
	{
		std::tuple<Types...> ret;
		decode_consume<0, 0>(ret, argument);
		return ret;
	}

	template<size_t TupleIndex, size_t BinOffset, typename TupleT, std::enable_if_t<TupleIndex >= std::tuple_size<TupleT>::value>*>
	void VM::decode_consume(TupleT&, const uint64_t) {}

	template<size_t TupleIndex, size_t BinOffset, typename TupleT, std::enable_if_t<TupleIndex < std::tuple_size<TupleT>::value>*>
	void VM::decode_consume(TupleT& t, const uint64_t argument)
	{
		// Typedef the element type
		typedef std::tuple_element_t<TupleIndex, TupleT> element_t;

		// Get the element we have to process
		element_t& elem = std::get<TupleIndex>(t);

		// Compute different sizes
		const size_t elem_bits = sizeof(element_t) * 8;
		constexpr size_t argument_size = sizeof(argument) * 8;

		constexpr size_t next_index = TupleIndex + 1;
		constexpr size_t next_offset = BinOffset + elem_bits;

		// Compute how many bits will be crushed on the left
		constexpr size_t crush_count = (argument_size - elem_bits - BinOffset);

		// Push out the bits on the left, stick to the right border
		elem = (argument << crush_count) >> (BinOffset + crush_count);

		// Consume the next index
		decode_consume<next_index, next_offset, TupleT>(t, argument);
	}
}

#endif
