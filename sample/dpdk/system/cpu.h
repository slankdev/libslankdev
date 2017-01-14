


#pragma once
#include <string>
#include "types.h"
#include "log.h"


namespace dpdk {


using function = int(*)(void*);
using func_arg = void*;
class Cpu {
public:
	uint8_t lcore_id;
	function func;
	func_arg arg;
    std::string name;

	Cpu() : lcore_id(0), func(nullptr), arg(nullptr)
    {
    }
	void boot(uint8_t id)
	{
        name = "lcore" + std::to_string(id);
		lcore_id = id;
        kernel_log(SYSTEM, "boot  %s ... done\n", name.c_str());
	}
	void configure()
    {
        kernel_log(SYSTEM, "configure %s ... done\n", name.c_str());
    }
	void launch()
	{
        kernel_log(SYSTEM, "%s lanching ... \n", name.c_str());
		rte_eal_remote_launch(func, arg, lcore_id);
	}
};


} /* namespace dpdk */


