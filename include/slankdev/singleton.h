
#pragma once

#include <stdio.h>
#include <string.h>
#include <stddef.h>


namespace slankdev {
    


template <typename T>
class simple_singleton {
protected:
    simple_singleton() {}
    ~simple_singleton() {}
    simple_singleton(const simple_singleton&) = delete;
    simple_singleton& operator=(const simple_singleton&) = delete;

public:
    static T& instance()
    {
        static T instnc;
        return instnc;
    }
};




template <typename T>
class pointer_singleton {
protected:
    static T* point;
    static bool inited;

    pointer_singleton() {}
    pointer_singleton(const pointer_singleton&) = delete;
    pointer_singleton& operator=(const pointer_singleton&) = delete;
public:
    static T* instance()
    {
        if (inited == false) {
            point = instance<>();
        }
        return point;
    }
    template <typename... Arg>
    static T* instance(Arg... args)
    {
        if (inited) {
            fprintf(stderr, "FATAL: reinit... (x.x)\n");
            exit(-1);
        }
        point = new T(args...);
        inited = true;
        return  point;
    }
};
template <typename T>
T* pointer_singleton<T>::point = nullptr;
template <typename T>
bool pointer_singleton<T>::inited = false;




/*
 * TODO 
 * This class is unsafe..
 * I couldn't implement cleverly.
 */

#if 0
template <typename T>
class reference_singleton {
protected:
    static bool inited;
    reference_singleton() {}
    reference_singleton(const reference_singleton&) = delete;
    reference_singleton& operator=(const reference_singleton&) = delete;

public:
    static T& instance()
    {
        if (inited == false) {
            T& ref = instance<>();
        }
        static T t;
        return t;
    }
    template <typename... Arg>
    static T& instance(Arg... args)
    {
        T& t = instance();
        if (inited == false) {
            t._constructor(args...);
            inited = true;
        } else {
            printf("try to reinit\n");
            exit(-1);
        }
        return t;
    }
};
template <typename T>
bool reference_singleton<T>::inited = false;
#endif



} /* namespace slankdev */
