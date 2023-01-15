//
// Created by Ziqi Zhang on 10/16/19.
//
//

#include <iostream>
#include <cstdlib>
#include "thread.h"

using namespace std;

int g = 0;

mutex mutex1;
mutex mutex2;

void hold_lock_1(void *a)
{
    mutex1.lock();
    cout << "thread 1 holding the lock" << endl;
    thread::yield();

    mutex1.unlock();
}

void hold_lock_2(void *a)
{
    try {
        mutex1.unlock();
    } catch (exception e) {
        cout << "ouch" << endl;
    }
    thread::yield();
    try {
        mutex2.unlock();
    } catch (exception e) {
        cout << "ouch" << endl;
    }
    mutex1.lock();
    cout << "thread 2 holding the lock" << endl;
    mutex1.unlock();
    thread t1 ( (thread_startfunc_t) hold_lock_1, (void *) "child thread 01");
    thread::yield();


}

void parent(void *a)
{
    intptr_t arg = (intptr_t) a;

    thread t1 ( (thread_startfunc_t) hold_lock_1, (void *) "child thread 01");
    thread t2 ( (thread_startfunc_t) hold_lock_2, (void *) "child thread 02");

    mutex1.lock();
    mutex1.unlock();
}

int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
}
