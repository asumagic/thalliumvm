#include "register.hpp"

namespace thallium
{
	Registers::Registers(size_t gp) : _memory(gp) {}

	vmreg_t &Registers::operator[](const SPRegisters s)
	{
		return _memory[static_cast<size_t>(s)];
	}

	vmreg_t &Registers::operator[](const size_t index)
	{
		return _memory[index];
	}

	void Registers::set_flag(const Flags flag, const bool value)
	{
		_memory[static_cast<size_t>(SPRegisters::fl)] = static_cast<uint32_t>(value) << static_cast<uint32_t>(flag);
	}

	bool Registers::get_flag(const Flags flag)
	{
		return (_memory[static_cast<size_t>(SPRegisters::fl)] >> static_cast<uint32_t>(flag)) & 0b1;
	}
}

