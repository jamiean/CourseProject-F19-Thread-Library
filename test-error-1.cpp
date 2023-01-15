/*
 * Tests style 02: signaling w/out holding the lock
 */
#include <iostream>
#include <cstdlib>
#include "thread.h"

using namespace std;

int g = 0;

mutex mutex1;
mutex mutex2;
cv cv1;
bool status;

void change_status(void *a)
{
    try {
        mutex1.unlock();
    } catch (const runtime_error& e) {
        cout << "ouch change_status mutex 1" << endl;
    }
    mutex1.lock();
    cout << "changing status" << endl;
    status = false;
    cv1.signal();
    mutex1.unlock();
}

void change_status_2(void *a)
{
    try {
        mutex2.unlock();
    } catch (const runtime_error& e) {
        cout << "ouch change_status_2 mutex 2" << endl;
    }
    mutex2.lock();
    cout << "changing status" << endl;
    status = false;
    cv1.signal();
    mutex2.unlock();
}

void check_status(void *a)
{
    mutex1.lock();

    while (status) {
        cout << "checking status" << endl;
        cv1.wait(mutex1);
    }
    cout << "I have reached here \n";

    mutex1.unlock();
}

void check_status_2(void *a)
{
    mutex2.lock();

    while (status) {
        cout << "checking status" << endl;
        try {
            cv1.wait(mutex1);
        } catch(const runtime_error& e) {
            cout << "ouch check_status_2 CV 1" << endl;
        }
        try {
            cv1.wait(mutex2);
        } catch(const runtime_error& e) {
            cout << "ouch check_status_2 CV 2" << endl;
        }
    }

    mutex2.unlock();
}

void parent(void *a)
{
    intptr_t arg = (intptr_t) a;

    mutex1.lock();
    status = true;
    cout << "parent called with arg " << arg << endl;
    mutex1.unlock();

    thread t1 ( (thread_startfunc_t) check_status, (void *) "child thread 01");
    thread::yield();
    thread t3 ( (thread_startfunc_t) check_status_2, (void *) "child thread 01");
    thread::yield();
    thread t2 ( (thread_startfunc_t) change_status, (void *) "child thread 02");
    thread::yield();
    thread t4 ( (thread_startfunc_t) change_status_2, (void *) "child thread 02");
    thread::yield();
}

int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
}
