#!/usr/bin/env python3

import os
import sys
from cmd import Cmd
from subprocess import check_output

class CLI(Cmd):
    prompt = 'cli> '
    target = ''
    intro  = ''

    def __init__(self, target):
        target = int(target)
        assert isinstance(target, int), 'Oh..'
        self.target = target
        self.intro = "monitoring to {}".format(self.target)
        Cmd.__init__(self)

    def do_reset(self, arg):
        os.kill(self.target, 12)

    def do_show(self, arg):
        os.kill(self.target, 10)

    def do_switchdetail(self, arg):
        os.kill(self.target, 15)

    def do_switchlog(self, arg):
        os.kill(self.target, 21)

    def do_dump(self, arg):
        os.kill(self.target, 23)

    def do_quit(self, arg):
        print("bye...")
        return True;

def pidof(name):
    ss = check_output(["pidof",name])
    return int(ss)

target=pidof('a.out')
CLI(target).cmdloop()

