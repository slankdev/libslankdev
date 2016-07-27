
#pragma once

#include <stdio.h>
#include <iostream>



namespace slankdev {


#if 0 //FOR DEBUG
static void print_mbuf_links(const struct rte_mbuf* head)
{

    if (!head) {
        printf("[nil]\n");
        return ;
    }

    while (head->next) {
        printf("[%p]->", head);
        head = head->next;
    }
    printf("[nil]\n");
}
#endif




template <typename T>
class allocator {
    public:
        T* allocate(size_t size)
        {
            return (T*)malloc(size);
        }
        void deallocate(T* ptr)
        {
            free(ptr);
        }
};


/*
 * If you want to generate queue-instance.
 * the class that is specified as T must have
 * member variable 'next'.
 */
template <typename T, class Allocator=slankdev::allocator<T>>
class queue {
    private:

        T* get_tail()
        {
            T* node = head;
            if (!node)
                return nullptr;

            while (node->next)
                node = node->next;
            return node;
        }

        Allocator allocer;
        T* head;
    public:
        queue() : head(nullptr) {}
        ~queue()
        {

            T* link = head;
            while (link) {
                T* next = link->next;
                allocer.deallocate(link);
                link = next;
            }
    
        }
        bool empty()
        {
            return (!head);
        }
        size_t size()
        {
            size_t cnt = 0;
            T* node = head;
            while (node) {
                cnt++;
                node = node->next;
            }
            return cnt;
        }
        T* front()
        {
            return head;
        }
        // T* back()
        // {
        //     return get_tail();
        // }
        void push(T* node) /* insert to tail */
        {
            T* tail = get_tail();
            if (tail) {
                tail->next = node;
            } else {
                head = node;
            }
        }
        T* pop() /* free head */
        {
            T* node = head;
            head = node->next;
            node->next = nullptr;
            return node;
        }
};






} /* namespace slankdev */
