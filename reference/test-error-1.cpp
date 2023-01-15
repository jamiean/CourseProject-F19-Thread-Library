/*
 * Tests style 02: signaling w/out holding the lock
 */
#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

int g = 0;

mutex mutex1;
mutex mutex2;
cv cv1;
bool status;

void change_status(void *a)
{
    mutex1.unlock();
    cout << "changing the status_" << endl;
    status = false;
    cv1.signal();
}

void check_status(void *a)
{
    mutex1.lock();

    while (status) {
        cout << "checking staus" << endl;
        cv1.wait(mutex1);
    }

    mutex1.unlock();
}

void parent(void *a)
{
    intptr_t arg = (intptr_t) a;

    mutex1.lock();
    status = true;
    cout << "parent called with arg " << arg << endl;
    mutex1.unlock();

    thread t1 ( (thread_startfunc_t) check_status, (void *) "child thread 01");
    thread t2 ( (thread_startfunc_t) change_status, (void *) "child thread 02");
}

int main()
{
    cpu::boot((thread_startfunc_t) parent, (void *) 100, 0);
}
