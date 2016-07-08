
#pragma once

#include <stdio.h>
#include <string.h>
#include <stddef.h>


namespace slankdev {
    



template <typename T>
class singleton final
{
private:
    singleton();
    ~singleton();
    singleton(const singleton&) = delete;
    singleton& operator=(const singleton&) = delete;

public:
    static T& instance()
    {
        static T instnc;
        return instnc;
    }
};



} /* namespace slankdev */
