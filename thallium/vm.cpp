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
			const vmreg_t init_ip = ip;
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
					_regs[SPRegisters::ip] = std::get<0>(darg);
			} break;

			case Opcode::cjmpr: {
				const auto darg = decode<uint16_t>(argument);
				if (_regs.get_flag(Flags::Test))
					_regs[SPRegisters::ip] = _regs[std::get<0>(darg)];
			} break;

			case Opcode::call: {
				const auto darg = decode<uint32_t>(argument);

				// push sp + 1 to the stack
				vmreg_t& sp = _regs[SPRegisters::sp];
				serialize_type(ip + Instruction::size(), begin(_memory) + sp);
				sp += sizeof(vmreg_t);

				ip = std::get<0>(darg);
			} break;

			case Opcode::callr: {
				const auto darg = decode<uint16_t>(argument);

				// push sp + 1 to the stack
				vmreg_t& sp = _regs[SPRegisters::sp];
				serialize_type(ip + Instruction::size(), begin(_memory) + sp);
				sp += sizeof(vmreg_t);

				ip = _regs[std::get<0>(darg)];
			} break;

			case Opcode::sbit: {
				const auto darg = decode<uint16_t, uint8_t, uint8_t>(argument);

				vmreg_t& r = _regs[std::get<0>(darg)];
				const vmreg_t offset = std::get<1>(darg);
				const uint8_t v = std::get<2>(darg) ? 0 : 1;

				r ^= (v ^ r) & (1 << offset);
			} break;

			case Opcode::gbit: {
				const auto darg = decode<uint16_t, uint8_t>(argument);
				_regs[std::get<1>(darg)] = (_regs[std::get<0>(darg)] >> std::get<1>(darg)) & 0b1;
			} break;

			case Opcode::shr: {
				const auto darg = decode<uint16_t, uint16_t, uint16_t>(argument);
				_regs[std::get<1>(darg)] = _regs[std::get<0>(darg)] >> _regs[std::get<2>(darg)];
			} break;

			case Opcode::shl: {
				const auto darg = decode<uint16_t, uint16_t, uint16_t>(argument);
				_regs[std::get<1>(darg)] = _regs[std::get<0>(darg)] << _regs[std::get<2>(darg)];
			} break;

			case Opcode::inc: {
				const auto darg = decode<uint16_t>(argument);
				++_regs[std::get<0>(darg)];
			} break;

			case Opcode::dec: {
				const auto darg = decode<uint16_t>(argument);
				--_regs[std::get<0>(darg)];
			} break;

			case Opcode::uadd: {
				const auto darg = decode<uint16_t, uint16_t, uint16_t>(argument);
				_regs[std::get<2>(darg)] = _regs[std::get<0>(darg)] + _regs[std::get<1>(darg)];
			} break;

			case Opcode::usub: {
				const auto darg = decode<uint16_t, uint16_t, uint16_t>(argument);
				_regs[std::get<2>(darg)] = _regs[std::get<0>(darg)] - _regs[std::get<1>(darg)];
			} break;


			case Opcode::umul: {
				const auto darg = decode<uint16_t, uint16_t, uint16_t>(argument);
				_regs[std::get<2>(darg)] = _regs[std::get<0>(darg)] * _regs[std::get<1>(darg)];
			} break;

			case Opcode::udiv: {
				const auto darg = decode<uint16_t, uint16_t, uint16_t>(argument);
				if (_regs[std::get<1>(darg) != 0])
					_regs[std::get<2>(darg)] = _regs[std::get<0>(darg)] / _regs[std::get<1>(darg)];
			} break;

			case Opcode::umod: {
				const auto darg = decode<uint16_t, uint16_t, uint16_t>(argument);
				if (_regs[std::get<1>(darg) != 0])
					_regs[std::get<2>(darg)] = _regs[std::get<0>(darg)] % _regs[std::get<1>(darg)];
			} break;

			case Opcode::push: {
				const auto darg = decode<uint16_t>(argument);

				vmreg_t& sp = _regs[SPRegisters::sp];
				serialize_type(std::get<0>(darg), begin(_memory) + sp);
				sp += sizeof(vmreg_t);
			} break;

			case Opcode::pop: {
				const auto darg = decode<uint16_t>(argument);

				vmreg_t& sp = _regs[SPRegisters::sp];
				_regs[std::get<0>(darg)] = deserialize_type<vmreg_t>(begin(_memory) + sp);
				sp -= sizeof(vmreg_t);
			} break;

			case Opcode::__PLACEHOLDER_EXIT: {
				return;
			}

			default: {
				error(TimeOfError::Runtime, ErrorType::Fatal, "illegal instruction : memory[" + std::to_string(ip) + "] has unknown opcode " + std::to_string(_memory[ip]));
			} break;
			}

			if (ip == init_ip)
				ip += Instruction::size();
		}
	}
}
