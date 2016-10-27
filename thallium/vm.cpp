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

			// @TODO Use C++17's structured bindings for switch cases

			switch (op)
			{
			case Opcode::mov: {
				const auto darg = decode<uint16_t, uint16_t>(argument);
				_regs[std::get<1>(darg)] = _regs[std::get<0>(darg)];
			} break;

			case Opcode::imm: {
				const auto darg = decode<uint32_t, uint16_t>(argument);
				_regs[std::get<1>(darg)] = std::get<0>(darg);
			} break;

			case Opcode::mget: {
				const auto darg = decode<uint16_t, uint16_t>(argument);
				_regs[std::get<1>(darg)] = deserialize_type<uint32_t>(begin(_memory) + _regs[std::get<0>(darg)]);
			} break;

			case Opcode::mset: {
				const auto darg = decode<uint16_t, uint16_t>(argument);
				serialize_type(_regs[std::get<1>(darg)], begin(_memory) + _regs[std::get<0>(darg)]);
			} break;

			case Opcode::teq: {
				const auto darg = decode<uint16_t, uint16_t>(argument);
				_regs.set_flag(Flags::Test, _regs[std::get<0>(darg)] == _regs[std::get<1>(darg)]);
			} break;

			case Opcode::tgt: {
				const auto darg = decode<uint16_t, uint16_t>(argument);
				_regs.set_flag(Flags::Test, _regs[std::get<0>(darg)] > _regs[std::get<1>(darg)]);
			} break;

			case Opcode::tlt: {
				const auto darg = decode<uint16_t, uint16_t>(argument);
				_regs.set_flag(Flags::Test, _regs[std::get<0>(darg)] < _regs[std::get<1>(darg)]);
			} break;

			case Opcode::cjmp: {
				const auto darg = decode<uint32_t>(argument);
				if (_regs.get_flag(Flags::Test))
				{
					_regs[SPRegisters::ip] = std::get<0>(darg);
					goto skip_ip_update;
				}
			} break;

			case Opcode::cjmpr: {
				const auto darg = decode<uint32_t>(argument);
				if (_regs.get_flag(Flags::Test))
				{
					_regs[SPRegisters::ip] = _regs[std::get<0>(darg)];
					goto skip_ip_update;
				}
			} break;

				/*
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
				case Opcode::din:break;*/

			default: {
				error(TimeOfError::Runtime, ErrorType::Fatal, "illegal instruction : memory[" + std::to_string(ip) + "] has unknown opcode " + std::to_string(_memory[ip]));
			} break;
			}

			ip += Instruction::size();
			skip_ip_update: {}
		}
	}
}
