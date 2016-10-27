#include "thallium/vm.hpp"

int main()
{
    using namespace thallium;
	VM vm{128};
	vm.import_program({
		{Opcode::imm,  0x0000000AFFFFFFFF}, // imm FFFFFFFF %r10
		{Opcode::push, 0x000000000000000A}, // push %r10
		{Opcode::pop,  0x000000000000000B}, // pop %r11
		{Opcode::teq,  0x00000000000B000A}, // teq %r10 %r11
		{Opcode::cjmp, 8 * Instruction::size()}, // cjmp somewhere
		{Opcode::__PLACEHOLDER_EXIT, 0}
	});
    vm.run();
	return 0;
}
