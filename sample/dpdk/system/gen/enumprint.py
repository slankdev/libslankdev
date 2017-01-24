#! /usr/bin/python

import re
import sys

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




enum2str(sys.argv[1], sys.argv[2], sys.argv[3])



