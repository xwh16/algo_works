CC=g++
CPPFLAGS=-g
LDFLAGS=-g

task1: task1.o
	g++ $(LDFLAGS) -o task1.out task1.o $(LDLIBS)

task2: task2.o
	g++ $(LDFLAGS) -o task2.out task2.o $(LDLIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

clean:
	$(RM) *.o