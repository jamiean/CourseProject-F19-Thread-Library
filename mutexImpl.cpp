#ifndef _MUTEXIMPL_CPP
#define _MUTEXIMPL_CPP

#include "mutex.h"
#include "cpu.h"
#include <queue>
#include <iostream>

using namespace std;

extern unsigned int cur_thread;
extern queue<unsigned int> ready_threads;

class mutex::impl {
public: 
	impl() {
        status_ = false;
		// invariant: when mutex is not being hold, owner_ = -1
		owner_ = -1;
		is_cv_ = false;
	}

	bool get_status() {
		return status_;
	}

	void set_status(bool val) {
        status_ = val;
	}

	void add_to_waiting_queue(unsigned int thread) {
		waiting_threads_.push(thread);
	}

	bool waiting_queue_is_empty() {
		return waiting_threads_.empty();
	}

    // Because I call it in CV function, we already have atomiciy in CV
    // No need to call disabled and enabled again
    void set_cv_flag(bool value) {
        is_cv_ = value;
	}

	bool get_cv_flag() {
	    return is_cv_;
	}

	long get_owner() {
	    // the owner_ may not be
	    return owner_;
	}

	void unlock_helper() {
        assert_interrupts_disabled();
        if (owner_ != (long)cur_thread) {
            throw runtime_error("thread tried to release mutex it didn't own");
        }
        set_status(false);
        set_owner(-1);

        if (!waiting_queue_is_empty()) {
            ready_threads.push(waiting_queue_front());
            set_owner(waiting_queue_front());
            waiting_queue_pop();
            set_status(true);
        }
	}

	void set_owner(unsigned int thread_id) {
	    owner_ = (long) thread_id;
	}



	unsigned int waiting_queue_front() {
		return waiting_threads_.front();
	}

	void waiting_queue_pop() {
		waiting_threads_.pop();
	}
private:
	queue<unsigned int> waiting_threads_;
	// false is FREE, true is BUSY
	bool status_;
	// if is_cv_ == true, do not enable interrupt after unlock
	bool is_cv_;
	long owner_;
};

#endif /* _MUTEXIMPL_CPP */