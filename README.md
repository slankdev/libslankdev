# libslankdev

[![Build Status](https://travis-ci.org/slankdev/libslankdev.svg?branch=master)](https://travis-ci.org/slankdev/libslankdev)

A misc library on C++11. This lib is "Header-Only-Library". So user don't need
to build my lib and link one. If you want a part of libslankdev, you must only
copy header file and include header file.
(of course, you must take care of dependency)



## Components

Pure C++ Wrapper

- intfd
- socketfd
- filefd
- basic exception
- format string
- singlethon
- telnet
- system function

Extra Lib Wrapper

- libpcap wrapper
- ncurses wrapper

## Requirement

- libpcap-dev
- libncurses5-dev

```
$ sudo apt install libpcap-dev libncurses5-dev
```


## Usage

To install, run these command only

```
$ git clone http://github.com/slankdev/libslankdev.git
$ cd libslankdev
$ make                // check syntax
$ sudo make install   // install
```

To uninstall...

```
$ sudo make uninstall
```


## Licence

MIT

