#ifndef _THREADIMPL_CPP
#define _THREADIMPL_CPP

#include "thread.h"

#include <queue>
#include <unordered_map>
#include <cassert>
#include <unordered_set>
#include <ucontext.h>

using namespace std;

// Imported global variables shared in the library
// Declaration in cpu.cpp
extern queue<unsigned int> ready_threads;
extern unsigned int thread_id_count;
extern unordered_map<unsigned int, ucontext_t*> thread_context_map;
extern unsigned int cur_thread;
extern queue<unsigned int> finished_thread;
extern unordered_map<unsigned int,char*> stacks_to_delete;
extern unordered_set<unsigned int> non_joinable_threads;
extern unordered_map<unsigned int, queue<unsigned int>> join_queue;
extern void freethreads();
extern void saveandswitch();

using namespace std;

// Only for debugging
#include <iostream>

class thread::impl {
public:
    impl() {

        // Allocate new stack when initializing a new thread
        stack = new char[STACK_SIZE];

        ucontext_ptr = new ucontext_t();
        ucontext_ptr->uc_stack.ss_sp = stack;
        ucontext_ptr->uc_stack.ss_size = STACK_SIZE;
        ucontext_ptr->uc_stack.ss_flags = 0;
        ucontext_ptr->uc_link = nullptr;

        // set thread class variables for cleanup purposes
        thread_id = thread_id_count;
        stacks_to_delete[thread_id] = stack;
    }

    ~impl() {

        // remove current thread from non-joinable because it is out of scope
        non_joinable_threads.erase(thread_id);
    }

    // wrap around the user function and update status_ when user function finishes
    static void threadwrapper(thread_startfunc_t start_func, void* arg) {
        
        // Enable interrupt before starting user function
        cpu::interrupt_enable();

        // user code portion
        start_func(arg);

        // Disable interrupt when updating thread status_, and swapping in the next thread
        cpu::interrupt_disable();

        // denote that the thread finishes(not the thread object)
        finished_thread.push(cur_thread);

        // denote the thread is no longer joinable
        non_joinable_threads.insert(cur_thread);

        // releasing threads waiting for this thread and removing thread entry
        if(join_queue.count(cur_thread)) {
            while(!join_queue[cur_thread].empty()) {
                ready_threads.push(join_queue[cur_thread].front());
                join_queue[cur_thread].pop();
            }
            join_queue.erase(cur_thread);
        }

        
        if(!ready_threads.empty()) {
            cur_thread = ready_threads.front();
            ready_threads.pop();
            setcontext(thread_context_map[cur_thread]);
        } else {
            // Suspend the CPU if the current thread finishes and there is no runnable thread
            cpu::interrupt_enable_suspend();
        }
    }

    ucontext_t* ucontext_ptr;
    char *stack;
    unsigned int thread_id;
};

#endif /* _THREADIMPL_CPP */