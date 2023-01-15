/*
 * Tests style 03: deadlock by joining itself
 */
#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

int g = 0;

mutex mutex1;
bool status;
thread* t_ptr;

void deadlock(void *a)
{
    while (!status) {}
    t_ptr->join();
    cout << "You shouldn't see this" << endl;
}

void parent(void *a)
{
    intptr_t arg = (intptr_t) a;

    mutex1.lock();
    status = false;
    cout << "parent called with arg " << arg << endl;


    thread t1 ( (thread_startfunc_t) deadlock, (void *) "child thread 01");
    t_ptr = &t1;
    status = true;
    cout << "launched t1" << endl;
    mutex1.unlock();
    thread::yield();
    mutex1.lock();
    mutex1.unlock();
}

int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
}
