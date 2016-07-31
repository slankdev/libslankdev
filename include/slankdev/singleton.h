
#pragma once

#include <stdio.h>
#include <string.h>
#include <stddef.h>


namespace slankdev {
    



template <typename T>
class singleton {
protected:
    singleton() {}
    ~singleton() {}
    singleton(const singleton&) = delete;
    singleton& operator=(const singleton&) = delete;

public:
    static T& instance()
    {
        static T instnc;
        return instnc;
    }
};



// template <typename T>
// class unsafe_singleton {
// protected:
//     static bool _inited;
//     unsafe_singleton() {}
//     ~unsafe_singleton() {}
//     unsafe_singleton(const unsafe_singleton&) = delete;
//     unsafe_singleton& operator=(const unsafe_singleton&) = delete;
//
// public:
//     static T& _instance()
//     {
//         static T t;
//         return t;
//     }
//     template <typename... Arg>
//     static T& instance(Arg... args)
//     {
//         T& t = _instance();
//         if (!_inited) {
//             t.init(args...);
//             _inited = true;
//         }
//         return t;
//     }
//     bool inited()
//     {
//         return _inited;
//     }
// };
// template <typename T>
// bool unsafe_singleton<T>::_inited = false;
//
//
// class test : public unsafe_singleton<test> {
//     friend unsafe_singleton<test>;
//     private:
//     test() {puts("cnst");}
//     ~test() {puts("dst");}
//     void init()         {puts("init(void)");}
//     void init(int)      {puts("init(int)");};
//     void init(double, int) {puts("init(double, int)");};
// };
//
//
// int main()
// {
//     test& ada = test::instance(1.1, 2);
//     test& t2a = test::instance();
// }



} /* namespace slankdev */
