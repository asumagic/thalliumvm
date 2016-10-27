#include "thallium/vm.hpp"

int main()
{
    using namespace thallium;
	VM vm{1024};
	vm.import_program({{Opcode::mov, 0x0000000000020001}});
    vm.run();
	return 0;
}
