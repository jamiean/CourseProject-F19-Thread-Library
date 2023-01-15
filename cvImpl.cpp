#ifndef _CVIMPL_CPP
#define _CVIMPL_CPP

#include "cv.h"
#include "mutex.h"
#include "mutexImpl.cpp"
#include <iostream>
#include <queue>

using namespace std;

class cv::impl {
public: 
	impl() {}

	void add_to_sleeping_queue(unsigned int thread) {
		sleeping_threads_.push(thread);
	}

	bool sleeping_queue_is_empty() {
		return sleeping_threads_.empty();
	}

	unsigned int sleeping_queue_front() {
		return sleeping_threads_.front();
	}

	void sleeping_queue_pop() {
		sleeping_threads_.pop();
	}

private:
	queue<unsigned int> sleeping_threads_;
};

#endif /* _CVIMPL_CPP */