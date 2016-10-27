#ifndef THALLIUMVM_REGISTER_HPP
#define THALLIUMVM_REGISTER_HPP

#include <cstdint>
#include <cstddef>
#include <vector>

namespace thallium
{
	typedef uint32_t vmreg_t;

	/**
	 * Typed enum class of specific-purpose ThalliumVM registers
	 */
	enum class SPRegisters : uint16_t
	{
		/**
		 * Instruction pointer: Address to the current instruction.
		 *
		 * Will be written to by the jump instructions, or directly by the user.
		 */
		ip = 0,

		/**
		 * Stack pointer: Points to the <b>top element</b> of the stack.
		 */
		sp = 1,

		/**
		 * Data bus port: Refers to the port used/to be used in the current/next data transaction.
		 */
		dbport = 2,

		/**
		 * Data bus data pointer: Refers to the memory address of the first element in the transaction buffer.
		 */
		dbdata = 3,

		/**
		  * Flag register
		  */
		fl = 4,

		// 3 reserved registers

		total = 8
	};

	/**
	 * Enum class of ThalliumVM flags in the FL register
	 */
	enum class Flags
	{
		/**
		  * TEST flag
		  *
		  * Holds the result of the last comparison operation
		  */
		Test = 0,
	};

	/**
	 * ThalliumVM Registers class, which holds the specific-purpose and general-purpose registers.
	 */
	class Registers
	{
	public:
		/**
		 * Constructor for <code>Register</code>.
		 * \param gp General purpose register count
		 */
		Registers(size_t gp = sp_count() + gp_count());

		/**
		 * Subscript operator for specific-purpose registers
		 * \param index Thallium specific-purpose register index
		 * \return Reference to a Thallium specific-purpose register
		 */
		vmreg_t& operator[](const SPRegisters s);

		/**
		 * Subscript operator for both specific and special purpose.<br>
		 * General-purpose registers begin at index SPRegisters::total.
		 * \param index Thallium register index
		 * \return Reference to a Thallium register
		 * \throws std::out_of_range
		 */
		vmreg_t& operator[](const size_t index);

		/**
		 * Sets a flag in the FL register to a given value.
		 * \param flag Flag to set
		 * \param value New state of the flag
		 */
		void set_flag(const Flags flag, const bool value);

		/**
		 * Returns a flag in the FL register.
		 * \param flag Flag to get
		 * \return Flag at given index
		 */
		bool get_flag(const Flags flag);

		/**
		 * \return Thallium specific purpose registers available
		 */
		constexpr static uint16_t sp_count()
		{
			return static_cast<uint16_t>(SPRegisters::total);
		}

		/**
		 * \return ThalliumVM general purpose registers available
		 */
		constexpr static uint16_t gp_count()
		{
			return 256;
		}

	private:
		std::vector<vmreg_t> _memory;
	};
}

#endif
