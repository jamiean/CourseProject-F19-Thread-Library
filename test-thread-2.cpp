// This test is used for many threads joining on a same thread
#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

int g = 0;

mutex mutex1;
// cv cv1;

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

void parent(void *a)
{
    intptr_t arg = (intptr_t) a;

    mutex1.lock();
    cout << "parent called with arg " << arg << endl;
    mutex1.unlock();

    thread t1 ( (thread_startfunc_t) loop, (void *) "child thread");
    
    thread::yield();
    cout << "parent executing\n";
    thread::yield();
    t1.join();
    cout << "parent executing\n";
    thread::yield();
    cout << "parent executing\n";
    thread::yield();
    cout << "parent executing\n";
    t1.join();
    cout << "parent executing\n";
    thread::yield();
    cout << "parent executing\n";
}

int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, true, true, 0);
}