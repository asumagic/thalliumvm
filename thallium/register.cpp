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
}

