CC=g++ -g -Wall

# List of source files for your disk scheduler
SCHEDULER_SOURCES=scheduler.cpp

# Generate the names of the disk scheduler's object files
SCHEDULER_OBJS=${SCHEDULER_SOURCES:.cpp=.o}

TEST_EXECUTABLES=test-error-1 test-style-2 test-style-4 test-style-3-join test-style-5

all: ${TEST_EXECUTABLES}

# Compile the disk scheduler and tag this compilation
scheduler: ${SCHEDULER_OBJS} libthread.o
	${CC} -o $@ $^ -ldl -pthread

example: example.o libthread.o
	${CC} -o $@ $^ -ldl -pthread

test-error-1: test-error-1.cpp libthread.o
	${CC} -o $@ $^ -ldl -pthread

test-cv-1: test-cv-1.cpp libthread.o
	${CC} -o $@ $^ -ldl -pthread

test-style-2: test-style-2.cpp libthread.o
	${CC} -o $@ $^ -ldl -pthread

# test-style-3: test-style-3.cpp libthread.o
# 	${CC} -o $@ $^ -ldl -pthread

test-style-3-join: test-style-3-join.cpp libthread.o
	${CC} -o $@ $^ -ldl -pthread

test-style-4: test-style-4.cpp libthread.o
	${CC} -o $@ $^ -ldl -pthread

# test-style-6: test-style-6.cpp libthread.o
# 	${CC} -o $@ $^ -ldl -pthread

test-style-5: test-style-5.cpp libthread.o
	${CC} -o $@ $^ -ldl -pthread

# Generic rules for compiling a source file to an object file
%.o: %.cpp
	${CC} -c $<
%.o: %.cc
	${CC} -c $<

clean:
	rm -f ${SCHEDULER_OBJS} scheduler ${TEST_EXECUTABLES}
