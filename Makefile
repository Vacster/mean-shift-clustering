CXX = g++
CFLAGS = -g --std=c++11
INCLUDE = -I/usr/include/python2.7
LIBS = -lpython2.7
OMP = -DOMP=true -fopenmp
VISUAL = -DMS_VISUAL=true
SRCS = mean_shift.cpp
TEST_SRCS = test.cpp
OBJS = $(SRCS:.cpp=.o)
TEST_OBJS = $(TEST_SRCS:.cpp=.o)
TEST_VISUAL = test_visual.o
TEST_OMP = test_omp.o

all: $(TEST_OBJS) $(TEST_VISUAL) $(TEST_OMP);

$(TEST_OBJS): $(OBJS)
	$(CXX) $(CFLAGS) $(addprefix src/test/,$(TEST_SRCS) main.cpp) $(addprefix bin/,$^) -o bin/test.o $(LIBS) 

$(TEST_VISUAL): $(OBJS)
	$(CXX) $(CFLAGS) $(addprefix src/test/,$(TEST_SRCS) main.cpp) $(addprefix bin/,$^) -o bin/$@ $(INCLUDE) $(LIBS) $(VISUAL)

$(TEST_OMP) : $(OBJS)
	$(CXX) $(CFLAGS) $(addprefix src/test/,$(TEST_SRCS) main.cpp) $(addprefix bin/,$^) -o bin/$@ $(INCLUDE) $(LIBS) $(OMP) $(VISUAL)

$(OBJS):
	$(CXX) $(CFLAGS) -c $(addprefix src/,$(SRCS)) -o bin/$@ $(INCLUDE) $(LIBS)

clean:
	rm bin/*