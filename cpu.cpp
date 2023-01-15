#ifndef _CPU_CPP
#define _CPU_CPP

#include "cpu.h"
#include "cpuImpl.cpp"
#include <queue>
#include <unordered_map>
#include <cassert>
#include <unordered_set>
#include <ucontext.h>


// only for debugging
#include <iostream>

using namespace std;

/* Shared global variables in the library */

// Ready threads queue, whenever a thread is or put into ready state,
// it is added to the queue
queue<unsigned int> ready_threads;

// Global thread Id, uniquely identify a thread, starting from index 0
unsigned int thread_id_count = 0;

// mapping between thread id and corresponding thread context
unordered_map<unsigned int, ucontext_t*> thread_context_map;

// The current thread running on the CPU
unsigned int cur_thread = 0;

// A set of finished threads that are waitng to be destroyed
queue<unsigned int> finished_thread;

// Stack of finished thread to delete
unordered_map<unsigned int,char*> stacks_to_delete;

// non joinable threads(finished but in scope) 
unordered_set<unsigned int> non_joinable_threads;

// join queue on a thrad
unordered_map<unsigned int, queue<unsigned int>> join_queue;

//  function to free threads
void freethreads() {
    while(!finished_thread.empty()) {
        unsigned int to_delete = finished_thread.front();
        finished_thread.pop();
        delete[] stacks_to_delete[to_delete];
        stacks_to_delete.erase(to_delete);
        delete thread_context_map[to_delete];
        thread_context_map.erase(to_delete);
    }
}

// save the current thread and switch to next thread
void saveandswitch() {
    unsigned int next_thread = ready_threads.front();
    ready_threads.pop();
    unsigned int thread_to_save = cur_thread;
    cur_thread = next_thread;
    swapcontext(thread_context_map[thread_to_save], thread_context_map[next_thread]);
}



/*
 * cpu::init() initializes a CPU.  It is provided by the thread library
 * and called by the infrastructure.  After a CPU is initialized, it
 * should run user threads as they become available.  If func is not
 * nullptr, cpu::init() also creates a user thread that executes func(arg).
 *
 * On success, cpu::init() should not return to the caller.
 */


void cpu::init(thread_startfunc_t start_func, void * arg){
    assert_interrupts_disabled();

    // Initialize time interruption handler in interrupt vector table
    // Implementation in cppImpl.cpp
    try {
        impl_ptr = new impl;
    } catch(exception e) {
        cpu::interrupt_enable();
        throw bad_alloc();
    }
    interrupt_handler_t interrupt_handler_time = impl_ptr->interrupt_handler_time;
    interrupt_vector_table[0] = interrupt_handler_time;

    // if func is not nullptr, creates a user thread that executes func(arg)
    if(start_func) {
        cpu::interrupt_enable();
        thread StartThread((thread_startfunc_t) start_func, (void *) arg);
        cpu::interrupt_disable();
        
        assert(ready_threads.size() == 1);
        cur_thread = ready_threads.front();
        ready_threads.pop();

        assert(cur_thread == 0); 
        assert(ready_threads.empty());

        // Start running the first thread
        setcontext(thread_context_map[cur_thread]);
    }
    
}

#endif /* _CPU_CPP */