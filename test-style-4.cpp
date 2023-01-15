/*
 * Tests style 04: use cv without while loop
 */
#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

int g = 0;

mutex mutex1;
cv cv1;
bool status;

void change_status(void *a)
{
    mutex1.lock();
    cout << "changing the status_" << endl;
    status = false;
    cv1.signal();

    mutex1.unlock();
}

void check_status(void *a)
{
    mutex1.lock();

    if (status) {
        cout << "checking staus" << endl;
        cv1.wait(mutex1);
    }
    cout << "status_ changed" << endl;

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
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
}
