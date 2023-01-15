/*
 * Tests style 03: deadlock
 */
#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

int g = 0;

mutex mutex1;
mutex mutex2;

void hold_lock_1(void *a)
{
    mutex1.lock();
    thread::yield();
    mutex2.lock();
    cout << "holding all locks" << endl;

    mutex2.unlock();
    mutex1.unlock();
}

void hold_lock_2(void *a)
{
    mutex2.lock();
    thread::yield();
    mutex1.lock();
    cout << "holding all locks" << endl;

    mutex1.unlock();
    mutex2.unlock();
}

void parent(void *a)
{
    intptr_t arg = (intptr_t) a;

    mutex1.lock();
    cout << "parent called with arg " << arg << endl;
    mutex1.unlock();

    thread t1 ( (thread_startfunc_t) hold_lock_1, (void *) "child thread 01");
    thread t2 ( (thread_startfunc_t) hold_lock_2, (void *) "child thread 02");
}

int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
}
