#ifndef _CV_CPP
#define _CV_CPP


#include "cpu.h"
#include <iostream>
#include <queue>
#include <unordered_map>
#include "cv.h"
#include "cvImpl.cpp"
#include <ucontext.h>
#include "interrupt_enabler.h"
#include <assert.h>

using namespace std;

extern queue<unsigned int> ready_threads;
extern unsigned int thread_id_count;
extern unordered_map<unsigned int, ucontext_t*> thread_context_map;
extern unsigned int cur_thread;
extern queue<unsigned int> finished_thread;
extern unordered_map<unsigned int,char*> stacks_to_delete;
extern void saveandswitch();

cv::cv(){
    try {
        impl_ptr = new impl();
    } catch (exception e) {
        throw bad_alloc();
    }
}

cv::~cv(){
    delete impl_ptr;
}

void cv::wait(mutex& mut) {
    {
		// disable interrupt, enable when object goes out of scope
        interrupt_enabler enable;

        assert_interrupts_disabled();

        // release the lock on current thread and SLEEP it
        try {
            mut.impl_ptr->unlock_helper();
        } catch (const runtime_error &e) {
            throw runtime_error("thread tried to release mutex it didn't own");
        }

        impl_ptr->add_to_sleeping_queue(cur_thread);

        // Give to the next thread, swap context
        // If no ready thread, do nothing (creates deadlock)
        if (!ready_threads.empty()) {
			saveandswitch();

            assert_interrupts_disabled();
        } else {
            cpu::interrupt_enable_suspend();
        }
    }

    assert_interrupts_enabled();

    // Only previous part need atomiciy, not this part
    mut.lock();
}


void cv::signal(){
    // disable interrupt, enable when object goes out of scope
    interrupt_enabler enable;

	// if there's a sleeping thread, aquire lock and awake it (push to ready thread)
	if (!impl_ptr->sleeping_queue_is_empty()) {
		ready_threads.push(impl_ptr->sleeping_queue_front());
		impl_ptr->sleeping_queue_pop();
	}
}


void cv::broadcast(){
    // disable interrupt, enable when object goes out of scope
    interrupt_enabler enable;

	// Awake all sleeping threads (push to ready thread)
	while (!impl_ptr->sleeping_queue_is_empty()) {
        ready_threads.push(impl_ptr->sleeping_queue_front());
		impl_ptr->sleeping_queue_pop();
	}
}

#endif /* _CV_CPP */