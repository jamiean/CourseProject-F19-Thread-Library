CC=g++ -g -Wall

# List of source files for your thread library
THREAD_SOURCES=cpu.cpp cv.cpp mutex.cpp thread.cpp cpuImpl.cpp cvImpl.cpp mutexImpl.cpp threadImpl.cpp interrupt_enabler.cpp

# Generate the names of the thread library's object files
THREAD_OBJS=${THREAD_SOURCES:.cpp=.o}

TEST_EXECUTABLES= test-thread-3 test-thread-1 test7 test6 test0 test-mutex-1 test-mutex-2 test-error-1 test-error-2 test-style-2 test-style-3 test-style-3-join test-style-4 test-style-5 test-style-6

all: libthread.o ${TEST_EXECUTABLES}

# Compile the thread library and tag this compilation
libthread.o: ${THREAD_OBJS}
	./autotag.sh
	ld -r -o $@ ${THREAD_OBJS}

# Compile an application program
test7: test7.cpp libthread.o libcpu.o
	${CC} -o $@ $^ -ldl -pthread

test5: test5.cpp libthread.o libcpu.o
	${CC} -o $@ $^ -ldl -pthread

test6: test6.cpp libthread.o libcpu.o
	${CC} -o $@ $^ -ldl -pthread

test0: test0.cpp libthread.o libcpu.o
	${CC} -o $@ $^ -ldl -pthread

test-mutex-1: test-mutex-1.cpp libthread.o libcpu.o
	${CC} -o $@ $^ -ldl -pthread

test-mutex-2: test-mutex-2.cpp libthread.o libcpu.o
	${CC} -o $@ $^ -ldl -pthread

test-error-1: test-error-1.cpp libthread.o libcpu.o
	${CC} -o $@ $^ -ldl -pthread

test-error-2: test-error-2.cpp libthread.o libcpu.o
	${CC} -o $@ $^ -ldl -pthread

test-style-2: test-style-2.cpp libthread.o libcpu.o
	${CC} -o $@ $^ -ldl -pthread

test-style-3: test-style-3.cpp libthread.o libcpu.o
	${CC} -o $@ $^ -ldl -pthread

test-style-3-join: test-style-3-join.cpp libthread.o libcpu.o
	${CC} -o $@ $^ -ldl -pthread

test-style-4: test-style-4.cpp libthread.o libcpu.o
	${CC} -o $@ $^ -ldl -pthread

test-style-6: test-style-6.cpp libthread.o libcpu.o
	${CC} -o $@ $^ -ldl -pthread

test-style-5: test-style-5.cpp libthread.o libcpu.o
	${CC} -o $@ $^ -ldl -pthread

test-thread-1: test-thread-1.cpp libthread.o libcpu.o
	${CC} -o $@ $^ -ldl -pthread

test-thread-3: test-thread-3.cpp libthread.o libcpu.o
	${CC} -o $@ $^ -ldl -pthread

# Generic rules for compiling a source file to an object file
%.o: %.cpp
	${CC} -c $<
%.o: %.cc
	${CC} -c $<

clean:
	rm -f ${THREAD_OBJS} libthread.o app ${TEST_EXECUTABLES}
