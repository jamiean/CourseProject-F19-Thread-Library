// This test is intended for joining functionalities, including both basic and advanced
#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

int g = 0;

mutex mutex1;
cv cv1;

void loop(void *a)
{   
    char *id = (char *) a;
    int i;
    cout << "loop called with id " << id << endl;
    mutex1.lock();
    for (i=0; i<3; i++, g--) {
        cout << id << ":\t" << i << "\t" << g << endl;
        while (g <= 0) {
            cv1.wait(mutex1);
        }
        cv1.signal();
    }
    mutex1.unlock();
}


void loop_cv(void *a) 
{
    char *id = (char *) a;
    int i;
    mutex1.lock();
    cout << "loop called with id " << id << endl;
    for (i=0; i<3; i++, g++) {
         cout << id << ":\t" << i << "\t" << g << endl;
        while (g > 0) {
            cv1.wait(mutex1);
        }
        cv1.signal();
    }
    mutex1.unlock();
}

void parent(void *a)
{
    intptr_t arg = (intptr_t) a;

    cout << "parent called with arg " << arg << endl;


    thread t1 ( (thread_startfunc_t) loop_cv, (void *) "broadcast thread 1");
    loop( (void *) "wait thread 1");
}

int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
}
