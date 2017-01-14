


#pragma once
#include "types.h"


namespace dpdk {


using function = int(*)(void*);
using func_arg = void*;
class Cpu {
public:
	uint8_t lcore_id;
	function func;
	func_arg arg;

	Cpu() : lcore_id(0), func(nullptr), arg(nullptr) {}

	void boot(uint8_t id)
	{
        printf("boot lcore%u ...", id);
		lcore_id = id;
        printf("done\n");
	}
	void configure() {}
	void launch()
	{
		rte_eal_remote_launch(func, arg, lcore_id);
	}
};


} /* namespace dpdk */


