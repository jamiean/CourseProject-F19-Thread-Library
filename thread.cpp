#ifndef _THREAD_CPP
#define _THREAD_CPP

#include "thread.h"
#include <cstdlib>
#include <queue>
#include <unordered_map>
#include <ucontext.h>
#include "threadImpl.cpp"
#include <ucontext.h>
#include <cassert>
#include "interrupt_enabler.h"

//Only for debugging
#include <iostream>

using namespace std;


typedef void (*thread_wrapped_startfunc_t) (void (*)(void*),void*);


thread::thread(thread_startfunc_t start_func, void* arg){

    // disable interrupt, enable when object goes out of scope
    interrupt_enabler enable;

    // free finished threads
    freethreads();

    try {
        impl_ptr = new impl();
    } catch (exception e) {
        throw bad_alloc();
    }

    // wrap start_func with function wrapper
    thread_wrapped_startfunc_t wrapped_start_func = impl_ptr->threadwrapper;

    
    // Direct the new thread to start by calling wrapped_start_func(arg1).
    try {
        makecontext(impl_ptr->ucontext_ptr, (void (*)()) wrapped_start_func, 2, start_func, arg);
    } catch (exception e) {
        throw bad_alloc();
    }

    // Map thread context to unique thread id
    thread_context_map[thread_id_count] = impl_ptr->ucontext_ptr;

    // Add Current Thread to ready thread queue, increment the global Thread Id
    ready_threads.push(thread_id_count++);

}


thread::~thread() {
    delete impl_ptr;
}

void thread::join() {
    // disable interrupt, enable when object goes out of scope
    interrupt_enabler enable;

    // If thread joined, return
    if(non_joinable_threads.count(impl_ptr->thread_id)) {
        return;
    }

    join_queue[impl_ptr->thread_id].push(cur_thread);

    // Paused the current thread and wait for joining to finish
    if (!ready_threads.empty()) {
        saveandswitch();
    } else {
        cpu::interrupt_enable_suspend();
    }
}

void thread::yield(){
    // disable interrupt, enable when object goes out of scope
    interrupt_enabler enable;
        
    // When yielding, look for the next thread to run
    // Save the current thread and swap in the next thread
    if(!ready_threads.empty()) {

        ready_threads.push(cur_thread);

        saveandswitch();
        
        // When switching threads, check for threads to free
        freethreads();
    }

    //If there is no ready thread, do nothing and continue running current thread
    return;
}

#endif /* _THREAD_CPP */