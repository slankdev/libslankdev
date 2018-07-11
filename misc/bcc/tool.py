#!/usr/bin/env python2
# from __future__ import print_function
from bcc import BPF
from time import strftime
import sys

bpf_text="""
#include <uapi/linux/ptrace.h>
int cb_malloc() {
    u32 pid = bpf_get_current_pid_tgid();
    if (pid != PID) return 0;
    bpf_trace_printk("malloc pid %u\\n", pid);
    return 0;
};
int cb_free() {
    u32 pid = bpf_get_current_pid_tgid();
    if (pid != PID) return 0;
    bpf_trace_printk("free pid %u\\n", pid);
    return 0;
};
"""

if len(sys.argv) < 2:
    print("USAGE: prog PID")
    exit()
pid = sys.argv[1]

bpf_text = bpf_text.replace('PID', pid)
b = BPF(text=bpf_text)
# prog='/home/vagrant/git/libvty/libvty.a'
# prog='/home/vagrant/git/libvty/a.out'
prog='c'
b.attach_uprobe(name=prog, sym='malloc', fn_name='cb_malloc')
b.attach_uprobe(name=prog, sym='free', fn_name='cb_free')

print("%-9s %-6s %s" % ("TIME", "PID", "COMMAND"))
while 1:
    try:
        (task, pid, cpu, flags, ts, msg) = b.trace_fields()
    except ValueError:
        continue
    print("%-9s %-6d %s" % (strftime("%H:%M:%S"), pid, msg))


