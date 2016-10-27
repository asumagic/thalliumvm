#include "thallium/vm.hpp"

int main()
{
    using namespace thallium;
	VM vm{1024};
	vm.import_program({{Opcode::mov, 0x0000000000000201}});
    vm.run();
	return 0;
}
