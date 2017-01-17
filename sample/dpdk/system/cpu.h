


#pragma once
#include <string>
#include "mempool.h"
#include "log.h"


namespace dpdk {


using function = int(*)(void*);
using func_arg = void*;

class Cpu {
public:
	const uint8_t lcore_id;
    const std::string name;
	function func;
	func_arg arg;

	Cpu(uint8_t id) :
        lcore_id(id),
        name("lcore" + std::to_string(id)),
        func(nullptr),
        arg(nullptr)
    {
        kernel_log(SYSTEM, "boot  %s ... done\n", name.c_str());
    }
    ~Cpu() { rte_eal_wait_lcore(lcore_id); }
	void launch()
	{
        kernel_log(SYSTEM, "%s lanching ... \n", name.c_str());
		rte_eal_remote_launch(func, arg, lcore_id);
	}
};


} /* namespace dpdk */


