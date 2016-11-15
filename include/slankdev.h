
#pragma once

#include <slankdev/exception.h>
#include <slankdev/net/protocol.h>
#include <slankdev/filefd.h>
#include <slankdev/intfd.h>
#include <slankdev/log.h>
#include <slankdev/queue.h>
#include <slankdev/singleton.h>
#include <slankdev/system.h>
#include <slankdev/util.h>

#warning "This header should not be included"


namespace slankdev {


uint8_t arp_raw[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x08, 0x06, 0x00, 0x01,
    0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
    0x00, 0x00 };




}
