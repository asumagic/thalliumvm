#include <vector>
#include <cstddef>
#include <tuple>
#include <iostream>
#include <string>
#include "vm.hpp"
#include "error.hpp"

namespace thallium
{
	VM::VM(const size_t memory_size) : _memory(memory_size) {}

	void VM::import_program(const std::vector<Instruction> program)
	{
		const size_t tprogram_size = program.size() * Instruction::size();

		// make sure the program fits in memory
		tassert(tprogram_size <= _memory.size(),
				TimeOfError::Preload, ErrorType::Fatal,
				"the program may not fit in memory.\n");

		// copy the program to the memory
		auto m_it = begin(_memory);
		for (const Instruction& i : program)
		{
			const auto serialized = i.serialize();
			std::move(begin(serialized), end(serialized), m_it);
			m_it += Instruction::size();
		}

		_regs[SPRegisters::sp] = static_cast<uint32_t>(tprogram_size);
	}

	void VM::run()
	{
		bool running = true;
		while (running)
		{
			vmreg_t& ip = _regs[SPRegisters::ip];
			const Opcode op = static_cast<Opcode>(_memory[ip]);
            const uint64_t argument = deserialize_type<uint64_t>(begin(_memory) + ip + 1);

			switch (op)
			{
				case Opcode::mov: {
					const auto darg = decode<uint16_t, uint16_t>(argument);
					_regs[std::get<1>(darg)] = _regs[std::get<0>(darg)]; // copy the source into the destination
				} break;

				case Opcode::imm:break;
				case Opcode::mget:break;
				case Opcode::mset:break;
				case Opcode::teq:break;
				case Opcode::tgt:break;
				case Opcode::tlt:break;
				case Opcode::cjmp:break;
				case Opcode::cjmpr:break;
				case Opcode::call:break;
				case Opcode::callr:break;
				case Opcode::sbit:break;
				case Opcode::gbit:break;
				case Opcode::shr:break;
				case Opcode::shl:break;
				case Opcode::inc:break;
				case Opcode::dec:break;
				case Opcode::uadd:break;
				case Opcode::usub:break;
				case Opcode::umul:break;
				case Opcode::udiv:break;
				case Opcode::umod:break;
				case Opcode::dout:break;
				case Opcode::din:break;

				default:
					error(TimeOfError::Runtime, ErrorType::Fatal, "illegal instruction");
					break;
			}

			ip += Instruction::size();
			running = false;
		}
	}
}
