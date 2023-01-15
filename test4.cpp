// This test is intended for joining functionalities, including both basic and advanced
#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

int g = 0;

// mutex mutex1;
// cv cv1;

void loop(void *a)
{   
    char *id = (char *) a;
    int i;
    cout << "loop called with id " << id << endl;
    for (i=0; i<5; i++, g++) {
	    cout << id << ":\t" << i << "\t" << g << endl;
	    thread::yield();
    }
}

void parent(void *a)
{
    intptr_t arg = (intptr_t) a;

    cout << "parent called with arg " << arg << endl;

    thread t1 ( (thread_startfunc_t) loop, (void *) "child thread 1");
    t1.join();
    thread t2 ( (thread_startfunc_t) loop, (void *) "child thread 2");
    loop( (void *) "parent thread");
}

int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, true, true, 0);
}
