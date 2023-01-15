// This test is used for many threads joining on a same thread
#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

int g = 0;

mutex mutex1;
mutex mutex2;
thread* ptr1;
thread* ptr2;
thread* ptr3;
thread* ptr4;

cv cv1;

void loop(void *a)
{
    char *id = (char *) a;
    int i;

    mutex1.lock();
    cout << "loop called with id " << id << endl;

    for (i=0; i<5; i++, g++) {
	    cout << id << ":\t" << i << "\t" << g << endl;
        mutex1.unlock();
	    thread::yield();
        mutex1.lock();
    }
    cout << id << ":\t" << i << "\t" << g << endl;
    mutex1.unlock();
}

void join1(void *a) {
    char *id = (char *) a;
    cout << id << endl;
    thread::yield();
    thread::yield();
    ptr1->join();
    thread::yield();
    cout << id << endl;
    thread::yield();
    ptr2->join();
    thread::yield();
    cout << id << endl;
    thread::yield();
    ptr1->join();
    thread::yield();
    cout << id << endl;
    thread::yield();
    ptr2->join();
    thread::yield();
    cout << id << endl;

}

void join2(void *a) {
    char *id = (char *) a;
    cout << id << endl;

    ptr3->join();
	thread::yield();
    cout << id << endl;
    ptr3->join();
    cout << id << endl;
}

void join3(void *a) {
    char *id = (char *) a;
    cout << id << endl;

    ptr3->join();
    cout << id << endl;
    ptr4->join();
	thread::yield();
    cout << id << endl;
    ptr3->join();
    cout << id << endl;
    cv1.signal();
}


void parent(void *a)
{
    intptr_t arg = (intptr_t) a;

    mutex1.lock();
    cout << "parent called with arg " << arg << endl;
    

    thread t1 ((thread_startfunc_t) loop, (void *) "child thread1");
    thread t2 ((thread_startfunc_t) loop, (void *) "child thread2");
    thread t3 ((thread_startfunc_t) join1, (void *) "child thread3");

    thread t4 ((thread_startfunc_t) join2, (void *) "child thread4");
    thread t5 ((thread_startfunc_t) join3, (void *) "child thread5");

    ptr1 = &t1;
    ptr2 = &t2;
    ptr4 = &t4;
    ptr3 = &t3;

    
    mutex1.unlock();
    mutex2.lock();
    cv1.wait(mutex2);
}

int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, true, true, 0);
}