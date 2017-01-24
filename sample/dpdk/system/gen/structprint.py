#! /usr/bin/python

import re

def enum2str(header, type, prefix):
    finding = True
    with open(header, 'r') as f:
        for line in f.read().split('\n'):
            if finding:
                if line.startswith('enum ' + type):
                    print('inline const char* {0}2str({1} e)'.format(type, type))
                    print('{')
                    print('    switch (e) {')
                    finding = False
            else:
                if line.startswith('}'):
                    print('        default: return "UNKNOWN_ERROR";')
                    print('}')
                    break
                else:
                    elem = line.split()[0].strip(',')
                    if elem.startswith(prefix):
                        print('        case {elem}: return "{str}";'.format(elem=elem, str=elem[len(prefix):]))
                    else:
                        print('something error + ' + line)
                        break



def type2fmt(type):
    if   (type == "uint8_t"  ): return "%u"
    elif (type == "uint16_t" ): return "%u"
    elif (type == "uint32_t" ): return "%u"
    elif (type == "uint64_t" ): return "%lu"

    elif (type == "uint8_t*" ): return "%p"
    elif (type == "uint16_t*"): return "%p"
    elif (type == "uint32_t*"): return "%p"
    elif (type == "uint64_t*"): return "%p"

    elif (type == "int"      ): return "%d"
    elif (type == "char"     ): return "%c"
    elif (type == "short"    ): return "%d"
    elif (type == "long"     ): return "%ld"

    elif (type == "size_t"   ): return "%zd"

    else: return "?????"




def struct2str(header, struct):
    finding = True
    with open(header, 'r') as f:
        for line in f.read().split('\n'):
            if finding:
                if line.startswith('struct ' + struct):
                    print('inline void print(const struct {1}* raw, const char* name="", size_t depth=0)'
                            .format(struct, struct))
                    print("    printf_depth(depth, \"{0} %s {{\", name);".format(struct));
                    print('')
                    print('    depth++')
                    finding = False
            else:
                if line.startswith('}'):
                    print('    depth--')
                    print('')
                    print("    printf_depth(depth, \"}\");");
                    print('}')
                    break
                else:
                    # print(line)
                    elems = line.split()
                    print(elems)

                    # type = line.split()[0]
                    # if (type == "struct"):
                    #     type = line.split()[1]
                    #     elem = line.split()[2]
                    # else:
                    #     elem = line.split()[1].strip(';')
                    # print("    printf_depth(depth, \"{0:25s} : {1:4s} \\n\", raw->{2:20s});"
                    #         .format(elem, type2fmt(type), elem))



# struct rte_ring_debug_stats {
# 	uint64_t enq_success_bulk; /**< Successful enqueues number. */
# 	uint64_t enq_success_objs; /**< Objects successfully enqueued. */
# 	uint64_t enq_quota_bulk;   /**< Successful enqueues above watermark. */
# 	uint64_t enq_quota_objs;   /**< Objects enqueued above watermark. */
# 	uint64_t enq_fail_bulk;    /**< Failed enqueues number. */
# 	uint64_t enq_fail_objs;    /**< Objects that failed to be enqueued. */
# 	uint64_t deq_success_bulk; /**< Successful dequeues number. */
# 	uint64_t deq_success_objs; /**< Objects successfully dequeued. */
# 	uint64_t deq_fail_bulk;    /**< Failed dequeues number. */
# 	uint64_t deq_fail_objs;    /**< Objects that failed to be dequeued. */
# } __rte_cache_aligned;

# inline void print(const struct rte_eth_fc_conf* raw, const char* name="", size_t depth=0)
# {
#     printf_depth(depth, "rte_eth_fc_conf %s {\n", name);
#
#     depth++;
#     printf_depth(depth, "high_water         : %u \n", raw->high_water);
#     printf_depth(depth, "low_water          : %u \n", raw->low_water );
#     printf_depth(depth, "pause_time         : %u \n", raw->pause_time);
#     printf_depth(depth, "send_xon           : %u \n", raw->send_xon  );
#     printf_depth(depth, "mode               : %s \n", rte_eth_fc_mode2str(raw->mode));
#     printf_depth(depth, "mac_ctrl_frame_fwd : %u \n", raw->mac_ctrl_frame_fwd);
#     printf_depth(depth, "autoneg            : %u \n", raw->autoneg           );
#     depth--;
#
#     printf_depth(depth, "}\n");
# }


# enum2str('rte_ring.h', 'rte_ring_queue_behavior', 'RTE_RING_QUEUE_')
struct2str('header.h', 'test_struct')



