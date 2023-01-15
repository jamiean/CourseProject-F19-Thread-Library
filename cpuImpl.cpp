#ifndef _CPUIMPL_CPP
#define _CPUIMPL_CPP

#include "cpu.h"
#include <queue>
#include <unordered_map>
#include <cassert>
#include <unordered_set>
#include <ucontext.h>
#include "interrupt_enabler.h"

using namespace std;

// only for debugging
#include <iostream>

// Imported global variables shared in the library
// Declaration in cpu.cpp
extern queue<unsigned int> ready_threads;
extern unsigned int thread_id_count;
extern unordered_map<unsigned int, ucontext_t*> thread_context_map;
extern queue<unsigned int> finished_thread;
extern unsigned int cur_thread;
extern unordered_map<unsigned int,char*> stacks_to_delete;
extern void freethreads();
extern void saveandswitch();

class cpu::impl {
public:
    static void interrupt_handler_time(void) {

        // disable interrupt, enable when object goes out of scope
        interrupt_enabler enable;
        
        // When interrupted, look for the next thread to run
        // Save the current thread and swap in the next thread
        if(!ready_threads.empty()) {
            ready_threads.push(cur_thread);

            // switch to other threads
            saveandswitch();
            
            // free finished threads
            freethreads();
        }

        //If there is no ready thread, do nothing and continue running current thread
        return;
    }
};

#endif /* _CPUIMPL_CPP */