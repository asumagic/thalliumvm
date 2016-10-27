#include "thallium/vm.hpp"

int main()
{
    using namespace thallium;
	VM vm{1024};
	vm.import_program({
		{Opcode::imm, 0x0000000AFFFFFFFF},
		{Opcode::mov, 0x00000000000B000A},
		{Opcode::__PLACEHOLDER_EXIT, 0}
	});
    vm.run();
	return 0;
}
