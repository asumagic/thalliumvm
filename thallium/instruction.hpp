#ifndef THALLIUMVM_INSTRUCTION_HPP
#define THALLIUMVM_INSTRUCTION_HPP

#include <cstdint>
#include <array>
#include "serializer.hpp"

namespace thallium
{
	/**
	 * Typed class enum of ThalliumVM instruction opcodes
	 */
	enum class Opcode : uint8_t
	{
		/**
		 * <code>mov rsrc rdst</code>
		 *
		 * copies the content of rsrc to rdst<br>
		 * <i>argument</i>:<br>
		 * - 0..15 : source register<br>
		 * - 16..31 : destination register
		 */
		mov = 0,

		/**
		 * <code>imm i rdst</code>
		 *
		 * copies the immediate value into rdst<br>
		 * <i>argument</i>:<br>
		 * - 0..31 : immediate value<br>
		 * - 32..47 : destination register
		 */
		imm = 1,

		/**
		 * <code>mget raddr rdst</code>
		 *
		 * copies the memory value pointed by raddr into rdst<br>
		 * <i>argument</i>:<br>
		 * - 0..15 : address register<br>
		 * - 16..31 : destination register
		 */
		mget = 2,

		/**
		 * <code>mset raddr rsrc</code>
		 *
		 * copies rsrc into the memory value pointed by raddr<br>
		 * <i>argument</i>:<br>
		 * - 0..15 : address register<br>
		 * - 16..31 : source register
		 */
		mset = 3,

		/**
		 * <code>teq r1 r2</code>
		 *
		 * test for equality<br>
		 * sets the TEST flag if r1 == r2<br>
		 * <i>argument</i>:<br>
		 * - 0..15 : register #1<br>
		 * - 16..31 : register #2
		 */
		teq = 4,

		/**
		 * <code>tgt r1 r2</code>
		 *
		 * sets the TEST flag if r1 > r2<br>
		 * <i>argument</i>:<br>
		 * - 0..15 : register #1<br>
		 * - 16..31 : register #2
		 */
		tgt = 5,

		/**
		 * <code>tlt r1 r2</code>
		 *
		 * sets the TEST flag if r1 < r2<br>
		 * <i>argument</i>:<br>
		 * - 0..15 : register #1<br>
		 * - 16..31 : register #2
		 */
		tlt = 6,

		/**
		 * <code>cjmp i</code>
		 *
		 * conditional jump<br>
		 * if the TEST flag is set, effectively loads the immediate into the ip register
		 * <i>argument</i>:<br>
		 * - 0..31 : immediate address
		 */
		cjmp = 7,

		/**
		 * <code>cjmpr rdst</code><br>
		 *
		 * conditional jump to register<br>
		 * if the TEST flag is set, effectively loads rdst into the ip register<br>
		 * <i>argument</i>:<br>
		 * - 0..15 : address register
		 */
		cjmpr = 8,

		/**
		 * <code>call i</code><br>
		 *
		 * function call<br>
		 * pushes the next instruction's address onto the stack and jumps to the function pointed by the immediate
		 * <i>argument</i>:<br>
		 * - 0..31 : immediate address
		 */
		call = 9,

		/**
		 * <code>callr r</code><br>
		 *
		 * function call to register address<br>
		 * pushes the next instruction's address onto the stack and jumps to the function pointed by the register
		 * <i>argument</i>:<br>
		 * - 0..15 : address register
		 */
		callr = 10,

		/**
		 * <code>sbit r i v</code>
		 *
		 * sets the bit in r at index i to v<br>
		 * <i>argument</i>:<br>
		 * - 0..15 : operand register<br>
		 * - 16..20 : bit index<br>
		 * - 21 : value
		 */
		sbit = 11,

		/**
		 * <code>gbit rsrc rdst i</code>
		 *
		 * sets rdst to the bit in rsrc at index i
		 * <i>argument</i>:<br>
		 * - 0..15 : source register<br>
		 * - 16..31 : destination register<br>
		 * - 32..36 : bit index
		 */
		gbit = 12,

		/**
		 * <code>shr rsrc rdst roff</code>
		 *
		 * bitshifts rsrc by roff bits to the right and stores the result in rdst
		 * <i>argument</i>:<br>
		 * - 0..15 : source register
		 * - 16..31 : destination register
		 * - 32..47 : bitshift amount register
		 */
		shr = 13,

		/**
		 * <code>shl rsrc rdst roff</code>
		 *
		 * bitshifts rsrc by roff bits to the left and stores the result in rdst
		 * <i>argument</i>:<br>
		 * - 0..15 : source register
		 * - 16..31 : destination register
		 * - 32..47 : bitshift amount register
		 */
		shl = 14,

		/**
		 * <code>inc r</code>
		 *
		 * increments r
		 * <i>argument</i>:<br>
		 * - 0..15 : operand register
		 */
		inc = 15,

		/**
		 * <code>dec r</code>
		 *
		 * decrements r
		 * <i>argument</i>:<br>
		 * - 0..15 : operand register
		 */
		dec = 16,

		/**
		 * <code>uadd r1 r2 rdst</code>
		 *
		 * r1 + r2
		 *
		 * adds r1 to r2 and stores the result in rdst
		 * <i>argument</i>:<br>
		 * - 0..15 : register #1<br>
		 * - 16..31 : register #2<br>
		 * - 32..47 : destination register
		 */
		uadd = 17,

		/**
		 * <code>usub r1 r2 rdst</code>
		 *
		 * r1 - r2
		 *
		 * subtracts r2 from r1 and stores the result in rdst
		 * <i>argument</i>:<br>
		 * - 0..15 : register #1<br>
		 * - 16..31 : register #2<br>
		 * - 32..47 : destination register
		 */
		usub = 18,

		/**
		 * <code>umul r1 r2 rdst</code>
		 *
		 * r1 * r2
		 *
		 * multiplies r1 with r2 and stores the result in rdst
		 * <i>argument</i>:<br>
		 * - 0..15 : register #1<br>
		 * - 16..31 : register #2<br>
		 * - 32..47 : destination register
		 */
		umul = 19,

		/**
		 * <code>udiv r1 r2 rdst</code>
		 *
		 * r1 / r2
		 *
		 * divides r1 by r2 and stores the result in rdst<br>
		 * division by zero is undefined behavior.
		 * the result is truncated.
		 * <i>argument</i>:<br>
		 * - 0..15 : register #1<br>
		 * - 16..31 : register #2<br>
		 * - 32..47 : destination register
		 */
		udiv = 20,

		/**
		 * <code>umod r1 r2 rdst</code>
		 *
		 * r1 % r2<br>
		 * r1 mod r2
		 *
		 * calculates the modulo of r1 by r2 and stores the result in rdst<br>
		 * division by zero is undefined behavior.
		 * - 0..15 : register #1<br>
		 * - 16..31 : register #2<br>
		 * - 32..47 : destination register
		 */
		umod = 21,

		/**
		 * <code>dout rsz</code>
		 *
		 * sends rsz bytes from dbdata to dbport.<br>
		 * <i>argument</i>:<br>
		 * - 0..15 : size register
		 */
		dout = 22,

		/**
		 * <code>din rsz rrecv</code>
		 *
		 * receives rsz bytes from dbport to dbdata.<br>
		 * the actual amount of received bytes is stored in rrecv, since fewer bytes than excepted can be received.<br>
		 * if more bytes than excepted are to be received, they will be kept in the queue.<br>
		 * <i>argument</i>:<br>
		 * - 0..15 : size register
		 * - 16..31 : received register
		 */
		din = 23,
	};

	/**
	 * Structure which holds a Thallium instruction
	 *
	 * Holds a 64-bit argument which is used by multiple operand instructions.
	 * An instruction is always 16 + 64 bits.
	 *
	 * This struct shall be stored 'as is' in memory.<br>
	 * The location of the first instruction only needs to be pointed by ip.<br>
	 * The program is typically located below the stack, before the first stack elements.
	 */
	struct Instruction
	{
		Opcode opcode;

		/**
		 * Argument for a Thallium instruction
		 *
		 * 64-bit argument size.
		 */
		uint64_t argument;

		/**
		 * Returns the size of one ThalliumVM instruction
		 * \return Size of one ThalliumVM instruction
		 */
		constexpr static size_t size()
		{
			return sizeof(opcode) + sizeof(argument);
		}

		/**
		 * Serialize an instruction into an array of uint8_t
		 * \return Serialized instruction
		 */
		auto serialize() const
		{
			std::array<uint8_t, size()> serialized;
			serialize_type(static_cast<uint8_t>(opcode), begin(serialized));
			serialize_type(argument, begin(serialized) + 1);
			return serialized;
		}
	};
}

#endif
