#ifndef _MUTEX_CPP
#define _MUTEX_CPP

#include "cpu.h"
#include <iostream>
#include <queue>
#include <unordered_map>
#include "mutex.h"
#include "interrupt_enabler.h"
#include "mutexImpl.cpp"
#include <ucontext.h>
#include <assert.h>

using namespace std;

extern queue<unsigned int> ready_threads;
extern unsigned int thread_id_count;
extern unordered_map<unsigned int, ucontext_t*> thread_context_map;
extern unsigned int cur_thread;
extern queue<unsigned int> finished_thread;
extern unordered_map<unsigned int,char*> stacks_to_delete;
extern void saveandswitch();

mutex::mutex(){
    try {
        impl_ptr = new impl();
    } catch (exception e) {
        throw bad_alloc();
    }
}

mutex::~mutex(){
    delete impl_ptr;
}

void mutex::lock(){
    interrupt_enabler disable;

    if (!impl_ptr->get_status()) {
        impl_ptr->set_status(true);
        impl_ptr->set_owner(cur_thread);
    }
    else {
        // current_thread -> waiting queue
        // run the first ready thread (timer interrupt / yield / from lock)
        impl_ptr->add_to_waiting_queue(cur_thread);

        // If no ready thread, do nothing (creates deadlock)
        if (!ready_threads.empty()) {
            saveandswitch();
        } else {
            cpu::interrupt_enable_suspend();
        }
    }
}



void mutex::unlock(){
    interrupt_enabler disable;

    assert_interrupts_disabled();
    
    if (impl_ptr->get_owner() != (long)cur_thread) {
        throw runtime_error("thread tried to release mutex it didn't own");
    }
    impl_ptr->set_status(false);
    impl_ptr->set_owner(-1);

    if (!impl_ptr->waiting_queue_is_empty()) {
        ready_threads.push(impl_ptr->waiting_queue_front());
        impl_ptr->set_owner(impl_ptr->waiting_queue_front());
        impl_ptr->waiting_queue_pop();
        impl_ptr->set_status(true);
    }
}


#endif /* _MUTEX_CPP */