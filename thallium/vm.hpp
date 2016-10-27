#ifndef THALLIUMVM_VM_HPP
#define THALLIUMVM_VM_HPP

#include <tuple>
#include <vector>
#include "instruction.hpp"
#include "register.hpp"

namespace thallium
{
	class VM
	{
	public:
		/**
		 * VM constructor, which initializes the memory size.
		 */
		VM(const size_t memory_size = 0);

		/**
		 * Decode instruction arguments into individual unsigned types
		 * \example auto decoded = decode<uint16_t, uint32_t, uint8_t, uint8_t>(someargument);
		 * \param argument Argument to decode
		 * \return Tuple of decoded arguments
		 */
		template<typename... Types>
		std::tuple<Types...> decode(uint64_t argument);

		/**
		 * Imports a program into the VM memory.
		 * \param program The ThalliumVM program to load
		 */
		void import_program(const std::vector<Instruction> program);

		/**
		 * Runs the program
		 */
		void run();

	private:
		/**
		 * Fallback for decode_consume when there aren't anything left in the tuple to consume
		 */
		template<size_t TupleIndex, size_t BinOffset, typename TupleT, std::enable_if_t<TupleIndex >= std::tuple_size<TupleT>::value>* = nullptr>
		void decode_consume(TupleT&, const uint64_t);

		/**
		 * Consume-decode a part of the instruction into a single tuple entry, and consume the next one subsequently
		 * \param TupleIndex Current tuple index to decode
		 * \param BinOffset Binary offset in the argument
		 * \param TupleT Tuple type (implicitly deduced)
		 * \param t Tuple
		 * \param argument Argument to decode
		 */
		template<size_t TupleIndex, size_t BinOffset, typename TupleT, std::enable_if_t<TupleIndex < std::tuple_size<TupleT>::value>* = nullptr>
		void decode_consume(TupleT& t, const uint64_t argument);

		std::vector<uint8_t> _memory;
		Registers _regs;
	};
}

#include "vm.tpp"

#endif
