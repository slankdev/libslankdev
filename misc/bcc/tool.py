#!/usr/bin/env python2
# from __future__ import print_function
from bcc import BPF
from time import strftime

bpf_text="""
#include <uapi/linux/ptrace.h>
int malloc_ret() {
    bpf_trace_printk("slankdev\\n");
    return 0;
};
int free_ret() {
    bpf_trace_printk("yukaribonk\\n");
    return 0;
};
"""

# program = '/home/vagrant/git/libslankdev/misc/bcc/a.out'
program = '/home/vagrant/git/libvty/a.out'
b = BPF(text=bpf_text)
b.attach_uretprobe(name=program, sym='malloc', fn_name='malloc_ret')
b.attach_uretprobe(name=program, sym='free', fn_name='free_ret')

print("%-9s %-6s %s" % ("TIME", "PID", "COMMAND"))
while 1:
    try:
        (task, pid, cpu, flags, ts, msg) = b.trace_fields()
    except ValueError:
        continue
    print("%-9s %-6d %s" % (strftime("%H:%M:%S"), pid, msg))


